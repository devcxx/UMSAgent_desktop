#ifndef UMSMANAGER_H
#define UMSMANAGER_H

#include <memory>

#include "../Common/SimpleIni.h"
#include "../Common/Singleton.h"
#include "../Model/allmodel.h"
#include "../Model/session.h"
#include "../MyObject/userrepolicy.h"

namespace UMSAgent {
class UmsManager : public Singleton<UmsManager> {
public:
    void init();
    void initUserRepolicy();
    void initUserSetting();

    //protected:
    UmsManager();

public:
    std::string appkey;
    std::unique_ptr<Session> app_session;
    std::string session_id;
    bool readOnlineConfig = false;
    std::unique_ptr<UserRepolicy> userRepolicy;
    std::unique_ptr<AllModel> model;
    std::unique_ptr<CSimpleIniA> setting;
};
}

#endif // UMSMANAGER_H
