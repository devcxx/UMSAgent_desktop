#ifndef CLIENTDATA_H
#define CLIENTDATA_H
#include <string>

#include <jsoncons/json.hpp>

namespace UMSAgent {
struct ClientData {
    std::string platform;
    std::string os_version;
    std::string language;
    // private string appkey;
    std::string resolution;
    std::string deviceid;
    bool isMobileDevice;
    std::string devicename;
    std::string defaultbrowser;
    std::string network;
    std::string version;
    std::string time;
    std::string appkey;
    std::string latitude;
    std::string longitude;
    std::string useridentifier;
    std::string modulename;
    std::string sessionId;
};

}
JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::ClientData, platform, os_version, language,
    resolution, deviceid, isMobileDevice, devicename,
    defaultbrowser, network, version, time,
    appkey, latitude, longitude, useridentifier, modulename, sessionId)

#endif // CLIENTDATA_H
