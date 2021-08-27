/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>


#include "json/json.h"
#include "google_breakpad/processor/basic_source_line_resolver.h"
#include "google_breakpad/processor/call_stack.h"
#include "google_breakpad/processor/code_module.h"
#include "google_breakpad/processor/code_modules.h"
#include "google_breakpad/processor/minidump.h"
#include "google_breakpad/processor/minidump_processor.h"
#include "google_breakpad/processor/process_state.h"
#include "google_breakpad/processor/stack_frame.h"
#include "processor/pathname_stripper.h"


namespace CrashReporter {

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

struct ModuleCompare {
    bool operator() (const CodeModule* aLhs, const CodeModule* aRhs) const {
        return aLhs->base_address() < aRhs->base_address();
    }
};

typedef std::map<const CodeModule*, unsigned int, ModuleCompare> OrderedModulesMap;

// Convert the stack frame trust value into a readable string.

std::string
FrameTrust(const StackFrame::FrameTrust aTrust);
// Convert the result value of the minidump processing step into a readable
// string.

std::string
ResultString(ProcessResult aResult);

// Convert the list of stack frames to JSON and append them to the array
// specified in the |aNode| parameter.

void
ConvertStackToJSON(const ProcessState& aProcessState,
                   const OrderedModulesMap& aOrderedModules,
                   const CallStack *aStack,
    Json::Value& aNode);

// Convert the list of modules to JSON and append them to the array specified
// in the |aNode| parameter.

int
ConvertModulesToJSON(const ProcessState& aProcessState,
                     OrderedModulesMap& aOrderedModules,
    Json::Value& aNode);

// Convert the process state to JSON, this includes information about the
// crash, the module list and stack traces for every thread

void
ConvertProcessStateToJSON(const ProcessState& aProcessState, Json::Value& aRoot);

// Process the minidump file and append the JSON-formatted stack traces to
// the node specified in |aRoot|

bool
ProcessMinidump(Json::Value& aRoot, const string& aDumpFile);

// Open the specified file in append mode

std::ofstream*
OpenAppend(const string& aFilename);
// Update the extra data file by adding the StackTraces field holding the
// JSON output of this program.

void
UpdateExtraDataFile(const string &aDumpPath, const Json::Value& aRoot);

} // namespace CrashReporter
