#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "../Common/Singleton.h"
#include "../Model/allmodel.h"
#include "../MyObject/pageinfo.h"
#include "../Common/storagesettings.h"

namespace UMSAgent {
class DataManager : public Singleton<DataManager> {
public:
    DataManager();
    //client data proceed
    void clientDataProceed();
    //event data proceed
    void eventDataProceed(const std::string& eventid, const std::string& pagename,
        const std::string& lable = "", int acc = 1, double count = 0.0);
    //get online config preference
    void onlineConfigProceed();
    //check new version
    void checkNewVersionProceed(const std::string& version);
    //all data proceed
    void allDataProceed();
    //save crash info when app crash
    void crashDataProceed(const std::string& time, const std::string& stacktrace);
    //page visit data proceed
    void pageInfoDataProceed(const PageInfo& obj);
    // tag data proceed
    void tagDataProceed(const std::string& tags);
    //userid data proceed
    void useridDataProceed(const std::string& userid);
    // pushid data proceed
    void pushidDataProceed(const std::string& pushid);

    std::string appkey;
    AllModel model;
    ApplicationSettings settings;
};
}

#endif // DATAMANAGER_H
