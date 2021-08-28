#ifndef ALLMODEL_H
#define ALLMODEL_H

#include "../MyObject/clientdata.h"
#include "../MyObject/datatype.h"
#include "../MyObject/event.h"
#include "../MyObject/onlineconfig.h"
#include "../MyObject/pushid.h"
#include "../MyObject/tag.h"
#include "../MyObject/updatepreference.h"
#include "../MyObject/userid.h"
#include "../MyObject/errorinfo.h"

namespace UMSAgent {
class AllModel {
public:
    AllModel();
    //get user event info
    Event getEventInfo(const std::string& eventid, const std::string& pagename, const std::string& label = "", int acc = 1);
    //get online config info
    OnLineConfig getOnlineConfig();
    UpdatePreference getUpdatePreference(const std::string& version);
    //get Tag data
    Tag getTagData(const std::string& tag);
    //get Userid data
    Userid getUseridData(const std::string& userid);
    //get Pushid data
    Pushid getPushidData(const std::string& pushid);
    //get client data
    ClientData getClientData();
    ErrorInfo getErrorInfo(const std::string& time, const std::string& stacktrace);
    std::string getUrl(DataType type);

    std::string key;
    std::string eventid;
};
}

#endif // ALLMODEL_H
