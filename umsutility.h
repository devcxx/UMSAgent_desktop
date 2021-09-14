#ifndef UMSUTILITY_H
#define UMSUTILITY_H

#include <string>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define UMSAGENT_EXPORT __declspec(dllexport)
#define UMSAGENT_IMPORT __declspec(dllimport)
#else
#define UMSAGENT_EXPORT __attribute__((visibility("default")))
#define UMSAGENT_IMPORT __attribute__((visibility("default")))
#endif

namespace UMSUtility {
//check network is connected
UMSAGENT_EXPORT bool isNetWorkConnected();
//get device id
UMSAGENT_EXPORT std::string getDeviceId();
// get OS version (eg. 10.0)
UMSAGENT_EXPORT std::string getOsVersion();
// get system platform (windows/mac)
UMSAGENT_EXPORT std::string getPlatform();
// get screen resolution (eg. 1920x1080)
UMSAGENT_EXPORT std::string getResolution();
// get device name (eg. 123’s Mac mini)
UMSAGENT_EXPORT std::string getDeviceName();
// get default browser name (eg. Chrome/Firefox)
UMSAGENT_EXPORT std::string getDefaultBrowser();
// get system model (eg. Macmini8,1)
UMSAGENT_EXPORT std::string getComputerModel();
}

#endif // UMSUTILITY_H
