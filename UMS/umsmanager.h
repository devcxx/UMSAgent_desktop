#ifndef UMSMANAGER_H
#define UMSMANAGER_H

#include <memory>

#include "../Common/SimpleIni.h"
#include "../Common/Singleton.h"
#include "../Model/allmodel.h"
#include "../Model/session.h"
#include "../MyObject/userrepolicy.h"
#include "../Common/storagesettings.h"

namespace UMSAgent {
class UmsManager : public Singleton<UmsManager> {
public:
    void init();
    void initUserRepolicy();
    void initUserSetting();

    void addPageStart(const std::string& pagename);
    void addPageEnd(const std::string& pagename);

    //protected:
    UmsManager();

public:
    std::string appkey;
    Session app_session;
    std::string session_id;
    bool readOnlineConfig = false;
    UserRepolicy userRepolicy;
    AllModel model;
    ApplicationSettings setting;
};
}

#endif // UMSMANAGER_H
