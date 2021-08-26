#include "umsapi.h"
#include "Common/constants.h"
#include "Common/datamanager.h"
#include "Common/utility.h"
#include "UMS/umsmanager.h"
#include "Common/easylogging++.h"

INITIALIZE_EASYLOGGINGPP
using namespace el;
using namespace UMSAgent;

static DataManager& manager = DataManager::getInstance();
static bool isNewSession;
static std::string device_resolution;
static bool isValidKey = false;

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
    if (settings.Contains("UserIdentifier"))
        settings.Remove("UserIdentifier");
    settings.Add("UserIdentifier", userid);
    settings.Save();
    //post userid
    postUserid(userid);
}

void UMSApi::postClientdata()
{
    if (isValidKey) {
    }
    else {
        LOG(ERROR) << "not valid appkey!";
    }
}

void UMSApi::onEvent(const std::string& event_id, const std::string& pagename)
{
    manager.eventDataProceed(event_id, pagename);
}

void UMSApi::onEvent(const std::string& event_id, const std::string& pagename, int acc)
{
    manager.eventDataProceed(event_id, pagename, "", acc);
}

void UMSApi::onEvent(const std::string event_id, const std::string& pagename, const std::string& label)
{
    manager.eventDataProceed(event_id, pagename, label);
}

void UMSApi::onEvent(const std::string& event_id, const std::string& pagename, const std::string& label, int acc)
{
    manager.eventDataProceed(event_id, pagename, label, acc);
}

void UMSApi::onPageBegin(const std::string &name)
{
    if (!name.empty()) {
        UmsManager::getInstance().addPageStart(name);
    }
}

void UMSApi::onPageEnd(const std::string &name)
{
    if (!name.empty()) {
        UmsManager::getInstance().addPageEnd(name);
    }
}

void UMSApi::postTag(const std::string &tag)
{
    manager.tagDataProceed(tag);
}

void UMSApi::postUserid(const std::string &userid)
{
    manager.useridDataProceed(userid);
}

void UMSApi::postPushid(const std::string &pushid)
{
    manager.pushidDataProceed(pushid);
}
