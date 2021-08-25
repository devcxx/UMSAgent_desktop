#ifndef UTILITY_H
#define UTILITY_H

#include <string>

namespace UMSAgent {
class Utility {
public:
    Utility();
    //check network is connected
    static bool isNetWorkConnected();
    //get device id
    static std::string getDeviceId();
    static std::string getTime();
    static std::string getOsVersion();
    static std::string getKernelVariant();
    static std::string getKernelVersion();
    static std::string getResolution();
    static std::string getDeviceName();
    static std::string getApplicationVersion();
    static void setApplicationVersion(const std::string& version);
    static std::string appVersion;
};
}

#endif // UTILITY_H
