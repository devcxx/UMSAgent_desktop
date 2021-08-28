#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "Common/Singleton.h"
#include <string>

namespace UMSAgent {
class DeviceInfo : public Singleton<DeviceInfo> {
protected:
    DeviceInfo();
    void initDeviceInfo();
    std::string getDeviceID();

public:
    std::string platform;
    std::string appkey;
    std::string resolution;
    std::string device_id;
    std::string os;
};
}

#endif // DEVICEINFO_H
