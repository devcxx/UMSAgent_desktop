#ifndef ERRORINFO_H
#define ERRORINFO_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct ErrorInfo {
    std::string os_version;
    std::string stacktrace;
    std::string time;
    std::string version;
    std::string deviceid;
    std::string activity;
    std::string appkey;
    std::string devicename;
    std::string useridentifier;
};
}
JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::ErrorInfo, os_version,
    stacktrace, time, version, deviceid, activity, appkey, devicename, useridentifier)

#endif // ERRORINFO_H
