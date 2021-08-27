#include "umsmanager.h"

namespace UMSAgent {

UmsManager::UmsManager()
{
}

void UmsManager::init()
{
    model.key = appkey;
    app_session.initNewSession();
    session_id = app_session.UMS_SESSION_ID;
    initUserRepolicy();
}

void UmsManager::initUserRepolicy()
{
    userRepolicy.autoLocation = "0";
    userRepolicy.repolicy = "0";
    userRepolicy.sessionTime = "30";
    userRepolicy.updateOnlyWifi = "1";
}

void UmsManager::initUserSetting()
{
    if (!setting.Contains("UserIdentifier")) {
        setting.Add("UserIdentifier", "");
    }
    if (!setting.Contains("hasDateToSend")) {
        setting.Add("hasDateToSend", "0");
    }

    if (!setting.Contains("repolicy")) {
        setting.Add("repolicy", userRepolicy.repolicy);
    } else {
        userRepolicy.repolicy = setting["repolicy"];
    }
    if (!setting.Contains("autolocation")) {
        setting.Add("autolocation", userRepolicy.autoLocation);
    } else {
        userRepolicy.autoLocation = setting["autolocation"];
    }

    if (!setting.Contains("sessiontime")) {
        setting.Add("sessiontime", userRepolicy.sessionTime);
    } else {
        userRepolicy.sessionTime = setting["sessiontime"];
    }

    if (!setting.Contains("updateonlywifi")) {
        setting.Add("updateonlywifi", userRepolicy.updateOnlyWifi);
    } else {
        userRepolicy.updateOnlyWifi = setting["updateonlywifi"];
    }
    setting.Save();
}

void UmsManager::addPageStart(const std::string& pagename)
{
    app_session.onPageStart(pagename);
}

void UmsManager::addPageEnd(const std::string& pagename)
{
    app_session.onPageEnd(pagename);
}

}
