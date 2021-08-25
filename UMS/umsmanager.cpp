#include "umsmanager.h"

namespace UMSAgent {

UmsManager::UmsManager()
{
}

void UmsManager::init()
{
    model = std::make_unique<AllModel>(appkey);
    setting = std::make_unique<CSimpleIniA>();
    setting->LoadFile("UMSAgent.ini");
    app_session = std::make_unique<Session>();
    app_session->initNewSession();
    session_id = app_session->UMS_SESSION_ID;
    initUserRepolicy();
}

void UmsManager::initUserRepolicy()
{
    if (userRepolicy == nullptr)
        userRepolicy = std::make_unique<UserRepolicy>();
    userRepolicy->autoLocation = "0";
    userRepolicy->repolicy = "0";
    userRepolicy->sessionTime = "30";
    userRepolicy->updateOnlyWifi = "1";
}

void UmsManager::initUserSetting()
{
    setting->SetValue("General", "UserIdentifier", "");
    setting->SetValue("General", "hasDateToSend", "0");
}

}
