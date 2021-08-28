#include "utility.h"
#include "machineid.h"

#include <cstdio>
#include <ctime>
#include <infoware/system.hpp>
#include <sstream>
#include <chrono>
#include <iomanip>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__)
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "Wininet.lib")
#elif defined(__APPLE__)
#include <SystemConfiguration/SystemConfiguration.h>
#endif

static bool IsNetConnected()
{
    bool returnValue = false;
#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__)
    DWORD dwConnectionStatus;
    returnValue = ::InternetGetConnectedState(&dwConnectionStatus, NULL) == TRUE ? true : false;
#elif defined(__APPLE__)
    // https://stackoverflow.com/questions/7627058/how-to-determine-internet-connection-in-cocoa
    struct sockaddr zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sa_len = sizeof(zeroAddress);
    zeroAddress.sa_family = AF_INET;

    SCNetworkReachabilityRef reachabilityRef = SCNetworkReachabilityCreateWithAddress(NULL, (const struct sockaddr*)&zeroAddress);

    if (reachabilityRef != NULL) {
        SCNetworkReachabilityFlags flags = 0;

        if (SCNetworkReachabilityGetFlags(reachabilityRef, &flags)) {
            BOOL isReachable = ((flags & kSCNetworkFlagsReachable) != 0);
            BOOL connectionRequired = ((flags & kSCNetworkFlagsConnectionRequired) != 0);
            returnValue = (isReachable && !connectionRequired) ? true : false;
        }

        CFRelease(reachabilityRef);
    }
#endif
    return returnValue;
}

static const char* kernelVariantName(iware::system::kernel_t variant) noexcept
{
    switch (variant) {
    case iware::system::kernel_t::windows_nt:
        return "Windows NT";
    case iware::system::kernel_t::linux:
        return "Linux";
    case iware::system::kernel_t::darwin:
        return "Darwin";
    default:
        return "Unknown";
    }
}

namespace UMSAgent {

std::string Utility::appVersion;

Utility::Utility()
{
}

bool Utility::isNetWorkConnected()
{
    return IsNetConnected();
}

std::string Utility::getDeviceId()
{
    std::uint64_t machHash = std::hash<std::string>{}(machineid::machineHash());
    return std::to_string(machHash);
}

std::string Utility::getTime()
{
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer[80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
    return std::string(buffer);
}

std::string Utility::getOsVersion()
{
    const iware::system::OS_info_t OS_info = iware::system::OS_info();
    return OS_info.full_name;
}

std::string Utility::getKernelVariant()
{
    const iware::system::kernel_info_t kernel_info = iware::system::kernel_info();
    const char* variant = kernelVariantName(kernel_info.variant);
    return std::string(variant);
}

std::string Utility::getKernelVersion()
{
    const iware::system::kernel_info_t kernel_info = iware::system::kernel_info();
    std::ostringstream oss;
    oss << kernel_info.major << '.' << kernel_info.minor << '.' << kernel_info.patch;
    return oss.str();
}

std::string Utility::getResolution()
{
    std::string resolution;
    const std::vector<iware::system::display_t> displays = iware::system::displays();
    if (displays.size()) {
        const auto& display = displays[0];
        std::ostringstream oss;
        oss << display.width << 'x' << display.height;
        resolution = oss.str();
    }
    return resolution;
}

std::string Utility::getDeviceName()
{
    std::string result;
#if defined(_WIN32)
    CHAR computerName[_MAX_PATH] = { 0 };
    DWORD arrayLength = _MAX_PATH*sizeof(CHAR);
    GetComputerNameExA(ComputerNameNetBIOS, computerName, &arrayLength);
    return computerName;
#elif defined(__APPLE__)

#endif
    return result;
}

std::string Utility::GetNetStates()
{
    return "Ethernet";
}

std::string Utility::getApplicationVersion()
{
    return appVersion;
}

int64_t Utility::getNowTicks()
{
    std::chrono::time_point<std::chrono::system_clock> now =
    std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
}

std::string Utility::timeDataStampToString(uint32_t timestamp)
{
    std::time_t tmp = timestamp;
    std::tm* t = std::gmtime(&tmp);
    std::stringstream ss;
    ss << std::put_time(t, "%Y-%m-%d-%H-%M-%S");
    std::string output = ss.str();
    return output;
}

void Utility::setApplicationVersion(const std::string& version)
{
    appVersion = version;
}
}
