#include "umsapi.h"
#include "Common/constants.h"
#include "Common/datamanager.h"
#include "Common/easylogging++.h"
#include "Common/utility.h"
#include "CrashReporter/minidump-analyzer.h"
#include "UMS/umsmanager.h"
#include "Common/ThreadPool.h"

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

static ThreadPool pool(std::thread::hardware_concurrency());

static void initLogger()
{
    Configurations c;
    c.setToDefault();
    c.setGlobally(el::ConfigurationType::MaxLogFileSize, "2097152");
    c.setGlobally(ConfigurationType::Format, "%datetime %level: %msg");
    c.setGlobally(ConfigurationType::ToFile, "false");
    c.setGlobally(ConfigurationType::ToStandardOutput, "false");
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
        LOG(ERROR) << "appkey is invalid!";
        return false;
    }
    return true;
}

void UMSApi::onAppStart(const std::string& appKey, const std::string& url)
{
    if (isAppkeyValid(appKey)) {
        initLogger();
        Constants::kBaseUrl = url;
        UmsManager::getInstance().appkey = appKey;
        UmsManager::getInstance().init();
        registerEvent();
        manager.appkey = appKey;
        device_resolution = Utility::getResolution();
        isValidKey = true;
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
        pool.enqueue([]{
            manager.clientDataProceed();
            manager.allDataProceed();
        });
    } else {
        LOG(ERROR) << "not valid appkey!";
    }
}

void UMSApi::onEvent(const std::string& event_id, const std::string& pagename)
{
    pool.enqueue([](const std::string& event_id, const std::string& pagename){
        manager.eventDataProceed(event_id, pagename);
    }, event_id, pagename);
}

void UMSApi::onEvent(const std::string& event_id, const std::string& pagename, int acc)
{
    pool.enqueue([](const std::string& event_id, const std::string& pagename, int acc) {
        manager.eventDataProceed(event_id, pagename, "", acc);
    }, event_id, pagename, acc);

}

void UMSApi::onEvent(const std::string event_id, const std::string& pagename, const std::string& label)
{
    pool.enqueue([](const std::string event_id, const std::string& pagename, const std::string& label) {
        manager.eventDataProceed(event_id, pagename, label);
    }, event_id, pagename, label);
}

void UMSApi::onEvent(const std::string& event_id, const std::string& pagename, const std::string& label, int acc)
{
    pool.enqueue([](const std::string& event_id, const std::string& pagename, const std::string& label, int acc) {
        manager.eventDataProceed(event_id, pagename, label, acc);
    }, event_id, pagename, label, acc);
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
        pool.enqueue([](const std::string& name) {
            UmsManager::getInstance().addPageEnd(name);
        }, name);
    }
}

void UMSApi::postTag(const std::string& tag)
{
    pool.enqueue([](const std::string& tag) {
        manager.tagDataProceed(tag);

    }, tag);
}

void UMSApi::postUserid(const std::string& userid)
{
    pool.enqueue([](const std::string& userid) {
        manager.useridDataProceed(userid);
    }, userid);
}

void UMSApi::postPushid(const std::string& pushid)
{
    pool.enqueue([](const std::string& pushid) {
        manager.pushidDataProceed(pushid);
    }, pushid);
}

void UMSApi::postCrashData(const std::string& dumpdir)
{
    pool.enqueue([](const std::string& dumpdir) {
        std::vector<std::string> dumps;
        if (fs::is_directory(dumpdir)) {
            for (auto& p : fs::recursive_directory_iterator(dumpdir)) {
                if (p.path().extension() == ".dmp") {
                    dumps.push_back(p.path().string());
                }
            }
        } else {
            dumps.push_back(dumpdir);
        }
        // Parse minidump and post
        for (auto& dump : dumps) {
            Json::Value stacktraceJS;
            bool ret = CrashReporter::ProcessMinidump(stacktraceJS, dump);
            if (ret) {
                std::uint32_t stamp = stacktraceJS["crash_info"]["time"].asUInt();
                std::string time = Utility::timeDataStampToString(stamp);
                manager.crashDataProceed(time, stacktraceJS.toStyledString());
                std::string dump_bak = dump + ".bak";
                fs::rename(dump, dump_bak);
            }
        }

    }, dumpdir);
}

void UMSApi::updateOnlineConfig()
{
    pool.enqueue([]{
        manager.onlineConfigProceed();
    });
}
