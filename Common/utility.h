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
    static std::string getPlatform();
    static std::string getResolution();
    static std::string getDeviceName();
    static std::string GetNetStates();
    static std::string getApplicationVersion();
    static std::string getApplicationLanguage();
    static std::int64_t getNowTicks();
    static std::string timeDataStampToString(std::uint32_t timestamp);
    static void setApplicationVersion(const std::string& version);
    static void setApplicationLanguage(const std::string& language);
    static std::string GetMachineGuid();
    static std::string GetComputerModel();
    static std::string appVersion;
    static std::string appLanguage;
};
}

#endif // UTILITY_H
