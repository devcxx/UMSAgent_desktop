#ifndef UMSAPI_H
#define UMSAPI_H

#include <string>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define UMSAGENT_EXPORT __declspec(dllexport)
#define UMSAGENT_IMPORT __declspec(dllimport)
#else
#define UMSAGENT_EXPORT __attribute__((visibility("default")))
#define UMSAGENT_IMPORT __attribute__((visibility("default")))
#endif

namespace UMSApi {
enum class DataType {
    CLIENTDATA = 0, //0
    CONFIGDATA, //1
    UPDATEDATA, //2
    EVENTDATA, //3
    AllDATA, //4
    ERRORDATA, //5
    PAGEINFODATA, //6
    TAGDATA, //7
    USERID, //8
    PUSHID //9
};

/*run this function when app start
 *appkey:the only id of your app
 * url:your web service base url
 * */
UMSAGENT_EXPORT void onAppStart(const std::string& appKey, const std::string& url);
// bind user id
UMSAGENT_EXPORT void bindUserIdentifier(const std::string& userid);
UMSAGENT_EXPORT void bindApplicationVersion(const std::string& version);
UMSAGENT_EXPORT void bindApplicationLanguage(const std::string& language);

//post clientdata and all data
UMSAGENT_EXPORT void postClientdata();

UMSAGENT_EXPORT void postCrashData(const std::string& dumpdir);

//upload event
UMSAGENT_EXPORT void onEvent(const std::string& event_id, const std::string& pagename);
//upload event with lable
UMSAGENT_EXPORT void onEvent(const std::string event_id,
    const std::string& pagename, const std::string& label);
//upload event with excuted times
UMSAGENT_EXPORT void onEvent(const std::string& event_id,
    const std::string& pagename, int acc);
//upload event with lable and  excuted times
UMSAGENT_EXPORT void onEvent(const std::string& event_id, const std::string& pagename,
    const std::string& label, int acc);
//get online config preference
//call_back_process_configdata :this function will be excuted when getting data from server
UMSAGENT_EXPORT void updateOnlineConfig();
// page visit when open current page
UMSAGENT_EXPORT void onPageBegin(const std::string& name);
//page visit when leave current page
UMSAGENT_EXPORT void onPageEnd(const std::string& name);
//post tag
UMSAGENT_EXPORT void postTag(const std::string& tag);
//post userid
UMSAGENT_EXPORT void postUserid(const std::string& userid);
//post pushid
UMSAGENT_EXPORT void postPushid(const std::string& pushid);

};

#endif // UMSAPI_H
