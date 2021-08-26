#include "session.h"
#include "../Common/guid.hpp"
#include "../Common/storagesettings.h"
#include "../Common/utility.h"
#include "../Model/allmodel.h"
#include "../MyObject/pageinfo.h"
#include "../UMS/umsmanager.h"
#include "../Common/datamanager.h"

namespace UMSAgent {

std::string Session::UMS_SESSION_ID = "session_id";
Session::Session()
{
}

void Session::initNewSession()
{
}

Session Session::initSessionWithOldData()
{
    ApplicationSettings settings;
    if (settings.Contains("cobub_session_id") && settings.Contains("closeTime")) {
        Session session;
        session.UMS_SESSION_ID = settings["cobub_session_id"];
        session.endtime = settings["closeTime"];
        return session;
    }
    return Session();
}

void Session::generateSessionID()
{
    std::string sessionid = xg::newGuid().str();
    UMS_SESSION_ID = sessionid;
    ApplicationSettings settings;
    if (settings.Contains("cobub_session_id"))
    {
        settings.Set("cobub_session_id", sessionid);
    }
    else
    {
        settings.Add("cobub_session_id", sessionid);
    }
    if (settings.Contains("pages"))
    {
        settings.Remove("pages");
    }
    if (settings.Contains("current_pages"))
    {
        settings.Remove("current_pages");
    }
    if (settings.Contains("duration"))
    {
        settings.Remove("duration");
    }
    settings.Save();
}

void Session::onPageStart(const std::string& pagename)
{
    if (pageDictionary.find(pagename) != pageDictionary.end()) {
        pageDictionary.erase(pagename);
    }
    if (pageDictionary.find(pagename+"starttime") != pageDictionary.end()) {
        pageDictionary.erase(pagename+"starttime");
    }
    pageDictionary.insert(std::pair<std::string, Variant>(pagename, Utility::getNowTicks()));
    pageDictionary.insert(std::pair<std::string, Variant>(pagename+"starttime", Utility::getTime()));

}

void Session::onPageEnd(const std::string& pagename)
{
    if (pageDictionary.find(pagename) != pageDictionary.end()) {
        std::int64_t t1 = nonstd::get<std::int64_t>(pageDictionary.at(pagename));
        std::int64_t duration = (Utility::getNowTicks() - t1) / 1000;
        PageInfo pageInfo;
        pageInfo.appkey = UmsManager::getInstance().appkey;
        pageInfo.duration = std::to_string(duration);
        pageInfo.end_millis = Utility::getTime();
        std::int64_t startMillis = nonstd::get<std::int64_t>(pageDictionary[pagename+"starttime"]);
        pageInfo.start_millis = std::to_string(startMillis);
        pageInfo.version = Utility::getApplicationVersion();
        pageInfo.activities = pagename;
        pageInfo.session_id = UMS_SESSION_ID;
        DataManager::getInstance().appkey = UmsManager::getInstance().appkey;
        DataManager::getInstance().pageInfoDataProceed(pageInfo);
    }
}
}
