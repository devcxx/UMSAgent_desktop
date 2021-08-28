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
#include <objc/objc.h>
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
    std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

#ifdef _WIN32
static bool sysGetVersionExWByRef(OSVERSIONINFOEXW& osVerInfo)
{
  ZeroMemory(&osVerInfo, sizeof(osVerInfo));
  osVerInfo.dwOSVersionInfoSize = sizeof(osVerInfo);

  typedef NTSTATUS(__stdcall *RtlGetVersionPtr)(RTL_OSVERSIONINFOEXW* pOsInfo);
  static HMODULE hNtDll = GetModuleHandleW(L"ntdll.dll");
  if (hNtDll != NULL)
  {
    static RtlGetVersionPtr RtlGetVer = (RtlGetVersionPtr) GetProcAddress(hNtDll, "RtlGetVersion");
    if (RtlGetVer && RtlGetVer(&osVerInfo) == 0)
      return true;
  }
  // failed to get OS information directly from ntdll.dll
  // use GetVersionExW() as fallback
  // note: starting from Windows 8.1 GetVersionExW() may return unfaithful information
  if (GetVersionExW((OSVERSIONINFOW*) &osVerInfo) != 0)
      return true;

  ZeroMemory(&osVerInfo, sizeof(osVerInfo));
  return false;
}
#endif

#if defined (__APPLE__)
#include "DarwinUtils.h"
extern "C" const char *GetOSXVersionString(void);
extern "C" const char *GetSystemMachineName(void);
#endif

std::string Utility::getOsVersion()
{
#if defined (_WIN32)
    std::ostringstream oss;
    OSVERSIONINFOEXW osvi;
    sysGetVersionExWByRef(osvi);
    oss << osvi.dwMajorVersion << osvi.dwMinorVersion;
    return oss.str();
#elif defined (__APPLE__)
    const char* osVersion = GetOSXVersionString();
    return osVersion;
#endif
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

std::string Utility::getPlatform()
{
#if defined(_WIN32)
    return "Windows";
#elif defined (__APPLE__)
    return "MacOS";
#endif
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
#if defined(_WIN32)
    CHAR computerName[_MAX_PATH] = { 0 };
    DWORD arrayLength = _MAX_PATH*sizeof(CHAR);
    GetComputerNameExA(ComputerNameNetBIOS, computerName, &arrayLength);
    return computerName;
#elif defined(__APPLE__)
    return GetSystemMachineName();
#endif
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
    ss << std::put_time(t, "%Y-%m-%d %H:%M:%S");
    std::string output = ss.str();
    return output;
}

void Utility::setApplicationVersion(const std::string& version)
{
    appVersion = version;
}
}
