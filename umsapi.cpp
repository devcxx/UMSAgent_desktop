#include "umsapi.h"
#include "Common/ThreadPool.h"
#include "Common/constants.h"
#include "Common/datamanager.h"
#include "Common/easylogging++.h"
#include "Common/storagesettings.h"
#include "Common/uploader.h"
#include "Common/utility.h"
#include "CrashReporter/minidump-analyzer.h"
#include "UMS/umsmanager.h"
#include "TaskLoop/RunLoop.h"

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (defined(__cplusplus) && __cplusplus >= 201703L)) && defined(__has_include)
#if __has_include(<filesystem>) && (!defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500)
#define GHC_USE_STD_FS
#include <filesystem>
namespace fs = std::filesystem;
#endif
#endif
#ifndef GHC_USE_STD_FS
#include <filesystem.hpp>
namespace fs = ghc::filesystem;
#endif

INITIALIZE_EASYLOGGINGPP
using namespace el;
using namespace UMSAgent;

static DataManager& manager = DataManager::getInstance();
static bool isNewSession;
static std::string device_resolution;
static bool isValidKey = false;

//static ThreadPool pool(std::thread::hardware_concurrency());
static task::Runloop* loop = nullptr;

static void initLogger()
{
    Configurations c;
    c.setToDefault();
    c.setGlobally(el::ConfigurationType::MaxLogFileSize, "2097152");
    c.setGlobally(ConfigurationType::Format, "%datetime %level: %msg");
    c.setGlobally(ConfigurationType::ToFile, "true");
    c.setGlobally(ConfigurationType::ToStandardOutput, "true");
    c.setGlobally(ConfigurationType::Filename, "UMSAgent.log");
    Loggers::reconfigureLogger("default", c);
    Loggers::addFlag(el::LoggingFlag::AutoSpacing);
}

static void registerEvent()
{
    // noop
}

static bool isAppkeyValid(const std::string& appkey)
{
    if (appkey.empty()) {
        return false;
    }
    return true;
}

void UMSApi::onAppStart(const std::string& appKey)
{
    initLogger();
    ApplicationSettings settings;
    if (isAppkeyValid(appKey) && settings.Contains("url")) {
        Constants::kBaseUrl = settings["url"];
        UmsManager::getInstance().appkey = appKey;
        UmsManager::getInstance().init();
        registerEvent();
        manager.appkey = appKey;
        device_resolution = Utility::getResolution();
        isValidKey = true;

        loop = task::Runloop::Create();

    } else {
        LOG(ERROR) << "invalid appkey or url";
    }
}

void UMSApi::bindUserIdentifier(const std::string& userid)
{
    ApplicationSettings settings;
    settings.Set("UserIdentifier", userid);
    settings.Save();
    //post userid
    //    postUserid(userid);
}

void UMSApi::bindApplicationVersion(const std::string& version)
{
    Utility::setApplicationVersion(version);
}

void UMSApi::postClientdata()
{
    if (isValidKey) {
//        pool.enqueue([] {
//            manager.clientDataProceed();
////            manager.allDataProceed();
//        });
        task::Clouser clouser([]{ manager.clientDataProceed();});
        loop->AddRunner(clouser);

    } else {
        LOG(ERROR) << "not valid appkey!";
    }
}

void UMSApi::onEvent(const std::string& event_id, const std::string& pagename)
{
    task::Clouser clouser([event_id, pagename] {
        manager.eventDataProceed(event_id, pagename);
    });
    loop->AddRunner(clouser);
}

void UMSApi::onEvent(const std::string& event_id, const std::string& pagename, int acc)
{
    task::Clouser clouser([event_id, pagename, acc] {
        manager.eventDataProceed(event_id, pagename, "", acc);
    });
    loop->AddRunner(clouser);
}

void UMSApi::onEvent(const std::string event_id, const std::string& pagename, const std::string& label)
{
    task::Clouser clouser([event_id, pagename, label] {
        manager.eventDataProceed(event_id, pagename, label);
    });
    loop->AddRunner(clouser);
}

void UMSApi::onEvent(const std::string& event_id, const std::string& pagename, const std::string& label, int acc)
{
    task::Clouser clouser([event_id, pagename, label, acc] {
        manager.eventDataProceed(event_id, pagename, label, acc);
    });
    loop->AddRunner(clouser);
}

void UMSApi::onPageBegin(const std::string& name)
{
    if (!name.empty()) {
        UmsManager::getInstance().addPageStart(name);
    }
}

void UMSApi::onPageEnd(const std::string& name)
{
    if (!name.empty()) {
        task::Clouser clouser([name]{
            UmsManager::getInstance().addPageEnd(name);
        });
        loop->AddRunner(clouser);
    }
}

void UMSApi::postTag(const std::string& tag)
{
    task::Clouser clouser([tag] {
        manager.tagDataProceed(tag);
    });
    loop->AddRunner(clouser);
}

void UMSApi::postUserid(const std::string& userid)
{
    task::Clouser clouser([userid] {
        manager.useridDataProceed(userid);
    });
    loop->AddRunner(clouser);
}

void UMSApi::postPushid(const std::string& pushid)
{
    task::Clouser clouser([pushid] {
        manager.pushidDataProceed(pushid);
    });
    loop->AddRunner(clouser);
}

void UMSApi::postCrashData(const std::string& dumpdir)
{
    if (!fs::exists(dumpdir))
        return;

    task::Clouser clouser([dumpdir] {
        std::vector<std::string> dumps;
        if (fs::is_directory(dumpdir)) {
            for (auto& p : fs::recursive_directory_iterator(dumpdir)) {
                if (p.path().extension() == ".dmp") {
                    std::string done = p.path().string() + ".json";
                    if (!fs::exists(done))
                        dumps.push_back(p.path().string());
                }
            }
        } else {
            fs::path dump(dumpdir);
            if (dump.extension() == ".dmp") {
                std::string done = dump.string() + ".json";
                if (!fs::exists(done))
                    dumps.push_back(dump.string());
            }
        }
        // Parse minidump and post
        for (auto& dump : dumps) {
            Json::Value stacktraceJS;
            bool ret = CrashReporter::ProcessMinidump(stacktraceJS, dump);
            if (ret) {
                std::uint32_t stamp = stacktraceJS["crash_info"]["time"].asUInt();
                std::string time = Utility::timeDataStampToString(stamp);
                std::string filename = fs::path(dump).filename().string();
                manager.crashDataProceed(time, stacktraceJS.toStyledString(), filename);

                // Upload dump file to server
                std::string uploadUrl = Constants::kBaseUrl + Constants::kUpload;
                if (Uploader::upload(uploadUrl, dump)) {
                    std::string done = dump + ".json";
                    std::ofstream ofs(done);
                    ofs << stacktraceJS.toStyledString();
                    ofs.close();
                    LOG(INFO) << "write stacktrace to file:" << done;
                } else {
                    LOG(WARNING) << "upload dump file failed";
                }

            } else {
                LOG(WARNING) << "process minidump failed";
            }
        }
    });
    loop->AddRunner(clouser);
}

void UMSApi::updateOnlineConfig()
{
   task::Clouser clouser([] {
        manager.onlineConfigProceed();
    });
   loop->AddRunner(clouser);
}

void UMSApi::bindApplicationLanguage(const std::string& language)
{
    Utility::setApplicationLanguage(language);
}

void UMSApi::bindBaseUrl(const std::string& url)
{
    Constants::kBaseUrl = url;
    ApplicationSettings settings;
    settings.Set("url", url);
    settings.Save();
    LOG(INFO) << "binded base url:" << url;
}

bool UMSApi::hasBindBaseUrl()
{
    ApplicationSettings settings;
    return settings.Contains("url");
}
