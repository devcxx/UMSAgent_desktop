/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include "minidump-analyzer.h"

#if defined(XP_WIN32)

#include <windows.h>

#elif defined(XP_UNIX) || defined(XP_MACOSX)

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#endif

#include "MinidumpAnalyzerUtils.h"

#if XP_WIN && HAVE_64BIT_BUILD
#include "MozStackFrameSymbolizer.h"
#endif

namespace CrashReporter {

using std::ios;
using std::ios_base;
using std::hex;
using std::ofstream;
using std::map;
using std::showbase;
using std::string;
using std::stringstream;
using std::wstring;

using google_breakpad::BasicSourceLineResolver;
using google_breakpad::CallStack;
using google_breakpad::CodeModule;
using google_breakpad::CodeModules;
using google_breakpad::Minidump;
using google_breakpad::MinidumpProcessor;
using google_breakpad::PathnameStripper;
using google_breakpad::ProcessResult;
using google_breakpad::ProcessState;
using google_breakpad::StackFrame;

MinidumpAnalyzerOptions gMinidumpAnalyzerOptions;

// Path of the minidump to be analyzed.
string gMinidumpPath;



const char kExtraDataExtension[] = ".extra";

string
ToHex(uint64_t aValue) {
  stringstream output;

  output << hex << showbase << aValue;

  return output.str();
}

// Convert the stack frame trust value into a readable string.

string
FrameTrust(const StackFrame::FrameTrust aTrust) {
  switch (aTrust) {
  case StackFrame::FRAME_TRUST_NONE:
    return "none";
  case StackFrame::FRAME_TRUST_SCAN:
    return "scan";
  case StackFrame::FRAME_TRUST_CFI_SCAN:
    return "cfi_scan";
  case StackFrame::FRAME_TRUST_FP:
    return "frame_pointer";
  case StackFrame::FRAME_TRUST_CFI:
    return "cfi";
  case StackFrame::FRAME_TRUST_PREWALKED:
    return "prewalked";
  case StackFrame::FRAME_TRUST_CONTEXT:
    return "context";
  }

  return "none";
}

// Convert the result value of the minidump processing step into a readable
// string.

string
ResultString(ProcessResult aResult) {
  switch (aResult) {
  case google_breakpad::PROCESS_OK:
    return "OK";
  case google_breakpad::PROCESS_ERROR_MINIDUMP_NOT_FOUND:
    return "ERROR_MINIDUMP_NOT_FOUND";
  case google_breakpad::PROCESS_ERROR_NO_MINIDUMP_HEADER:
    return "ERROR_NO_MINIDUMP_HEADER";
  case google_breakpad::PROCESS_ERROR_NO_THREAD_LIST:
    return "ERROR_NO_THREAD_LIST";
  case google_breakpad::PROCESS_ERROR_GETTING_THREAD:
    return "ERROR_GETTING_THREAD";
  case google_breakpad::PROCESS_ERROR_GETTING_THREAD_ID:
    return "ERROR_GETTING_THREAD_ID";
  case google_breakpad::PROCESS_ERROR_DUPLICATE_REQUESTING_THREADS:
    return "ERROR_DUPLICATE_REQUESTING_THREADS";
  case google_breakpad::PROCESS_SYMBOL_SUPPLIER_INTERRUPTED:
    return "SYMBOL_SUPPLIER_INTERRUPTED";
  default:
    return "";
  }
}

// Convert the list of stack frames to JSON and append them to the array
// specified in the |aNode| parameter.

void
ConvertStackToJSON(const ProcessState& aProcessState,
                   const OrderedModulesMap& aOrderedModules,
                   const CallStack *aStack,
                   Json::Value& aNode)
{
  int frameCount = aStack->frames()->size();
  unsigned int moduleIndex = 0;

  for (int frameIndex = 0; frameIndex < frameCount; ++frameIndex) {
    const StackFrame *frame = aStack->frames()->at(frameIndex);
    Json::Value frameNode;

    if (frame->module) {
      auto itr = aOrderedModules.find(frame->module);

      if (itr != aOrderedModules.end()) {
        moduleIndex = (*itr).second;
        frameNode["module_index"] = moduleIndex;
      }
    }

    frameNode["trust"] = FrameTrust(frame->trust);
    // The 'ip' field is equivalent to socorro's 'offset' field
    frameNode["ip"] = ToHex(frame->instruction);

    aNode.append(frameNode);
  }
}

// Convert the list of modules to JSON and append them to the array specified
// in the |aNode| parameter.

int
ConvertModulesToJSON(const ProcessState& aProcessState,
                     OrderedModulesMap& aOrderedModules,
                     Json::Value& aNode)
{
  const CodeModules* modules = aProcessState.modules();

  if (!modules) {
    return -1;
  }

  // Create a sorted set of modules so that we'll be able to lookup the index
  // of a particular module.
  for (unsigned int i = 0; i < modules->module_count(); ++i) {
    aOrderedModules.insert(
      std::pair<const CodeModule*, unsigned int>(
        modules->GetModuleAtSequence(i), i
      )
    );
  }

  uint64_t mainAddress = 0;
  const CodeModule *mainModule = modules->GetMainModule();

  if (mainModule) {
    mainAddress = mainModule->base_address();
  }

  unsigned int moduleCount = modules->module_count();
  int mainModuleIndex = -1;

  for (unsigned int moduleSequence = 0;
       moduleSequence < moduleCount;
       ++moduleSequence)
  {
    const CodeModule *module = modules->GetModuleAtSequence(moduleSequence);

    if (module->base_address() == mainAddress) {
      mainModuleIndex = moduleSequence;
    }

    Json::Value moduleNode;
    moduleNode["filename"] = PathnameStripper::File(module->code_file());
    moduleNode["code_id"] = PathnameStripper::File(module->code_identifier());
    moduleNode["version"] = module->version();
    moduleNode["debug_file"] = PathnameStripper::File(module->debug_file());
    moduleNode["debug_id"] = module->debug_identifier();
    moduleNode["base_addr"] = ToHex(module->base_address());
    moduleNode["end_addr"] = ToHex(module->base_address() + module->size());

    aNode.append(moduleNode);
  }

  return mainModuleIndex;
}

// Convert the process state to JSON, this includes information about the
// crash, the module list and stack traces for every thread

void
ConvertProcessStateToJSON(const ProcessState& aProcessState, Json::Value& aRoot)
{
  // We use this map to get the index of a module when listed by address
  OrderedModulesMap orderedModules;

  // Crash info
  Json::Value crashInfo;
  int requestingThread = aProcessState.requesting_thread();

  if (aProcessState.crashed()) {
    crashInfo["type"] = aProcessState.crash_reason();
    crashInfo["address"] = ToHex(aProcessState.crash_address());
    crashInfo["time"] = aProcessState.time_date_stamp();


    if (requestingThread != -1) {
      // Record the crashing thread index only if this is a full minidump
      // and all threads' stacks are present, otherwise only the crashing
      // thread stack is written out and this field is set to 0.
      crashInfo["crashing_thread"] =
        gMinidumpAnalyzerOptions.fullMinidump ? requestingThread : 0;
    }
  } else {
    crashInfo["type"] = Json::Value(Json::nullValue);
    // Add assertion info, if available
    string assertion = aProcessState.assertion();

    if (!assertion.empty()) {
      crashInfo["assertion"] = assertion;
    }
  }

  aRoot["crash_info"] = crashInfo;

  // Modules
  Json::Value modules(Json::arrayValue);
  int mainModule = ConvertModulesToJSON(aProcessState, orderedModules, modules);

  if (mainModule != -1) {
    aRoot["main_module"] = mainModule;
  }

  aRoot["modules"] = modules;

  // Threads
  Json::Value threads(Json::arrayValue);
  int threadCount = aProcessState.threads()->size();

  if (!gMinidumpAnalyzerOptions.fullMinidump && (requestingThread != -1)) {
    // Only add the crashing thread
    Json::Value thread;
    Json::Value stack(Json::arrayValue);
    const CallStack* rawStack = aProcessState.threads()->at(requestingThread);

    ConvertStackToJSON(aProcessState, orderedModules, rawStack, stack);
    thread["frames"] = stack;
    threads.append(thread);
   } else {
    for (int threadIndex = 0; threadIndex < threadCount; ++threadIndex) {
      Json::Value thread;
      Json::Value stack(Json::arrayValue);
      const CallStack* rawStack = aProcessState.threads()->at(threadIndex);

      ConvertStackToJSON(aProcessState, orderedModules, rawStack, stack);
      thread["frames"] = stack;
      threads.append(thread);
    }
  }

  aRoot["threads"] = threads;
}

// Process the minidump file and append the JSON-formatted stack traces to
// the node specified in |aRoot|

bool
ProcessMinidump(Json::Value& aRoot, const string& aDumpFile) {
#if XP_WIN && HAVE_64BIT_BUILD
  MozStackFrameSymbolizer symbolizer;
  MinidumpProcessor minidumpProcessor(&symbolizer, false);
#else
  BasicSourceLineResolver resolver;
  // We don't have a valid symbol resolver so we pass nullptr instead.
  MinidumpProcessor minidumpProcessor(nullptr, &resolver);
#endif

  // Process the minidump.
  Minidump dump(aDumpFile);
  if (!dump.Read()) {
    return false;
  }

  ProcessResult rv;
  ProcessState processState;
  rv = minidumpProcessor.Process(&dump, &processState);
  aRoot["status"] = ResultString(rv);

  ConvertProcessStateToJSON(processState, aRoot);

  return true;
}

// Open the specified file in append mode

ofstream*
OpenAppend(const string& aFilename)
{
  ios_base::openmode mode = ios::out | ios::app;

#if defined(XP_WIN)
#if defined(_MSC_VER)
  ofstream* file = new ofstream();
  file->open(UTF8ToWide(aFilename).c_str(), mode);
#else   // GCC
  ofstream* file =
    new ofstream(WideToMBCP(UTF8ToWide(aFilename), CP_ACP).c_str(), mode);
#endif  // _MSC_VER
#else // Non-Windows
  ofstream* file = new ofstream(aFilename.c_str(), mode);
#endif // XP_WIN
  return file;
}

// Update the extra data file by adding the StackTraces field holding the
// JSON output of this program.

void
UpdateExtraDataFile(const string &aDumpPath, const Json::Value& aRoot)
{
  string extraDataPath(aDumpPath);
  int dot = extraDataPath.rfind('.');

  if (dot < 0) {
    return; // Not a valid dump path
  }

  extraDataPath.replace(dot, extraDataPath.length() - dot, kExtraDataExtension);
  ofstream* f = OpenAppend(extraDataPath.c_str());

  if (f->is_open()) {
    Json::FastWriter writer;

    *f << "StackTraces=" << writer.write(aRoot);

    f->close();
  }

  delete f;
}

} // namespace CrashReporter

using namespace CrashReporter;

void
ParseArguments(int argc, char** argv) {
  if (argc <= 1) {
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc - 1; i++) {
    if (strcmp(argv[i], "--full") == 0) {
      gMinidumpAnalyzerOptions.fullMinidump = true;
    } else if ((strcmp(argv[i], "--force-use-module") == 0) && (i < argc - 2)) {
      gMinidumpAnalyzerOptions.forceUseModule = argv[i + 1];
      ++i;
    } else {
      exit(EXIT_FAILURE);
    }
  }

  gMinidumpPath = argv[argc - 1];
}

//int main(int argc, char** argv)
//{
//  ParseArguments(argc, argv);

//  if (!FileExists(gMinidumpPath)) {
//    // The dump file does not exist
//    exit(EXIT_FAILURE);
//  }

//  // Try processing the minidump
//  Json::Value root;
//  if (ProcessMinidump(root, gMinidumpPath)) {
//    UpdateExtraDataFile(gMinidumpPath, root);
//  }

//  exit(EXIT_SUCCESS);
//}
