#include "deviceinfo.h"
#include "../Common/utility.h"

namespace UMSAgent {

DeviceInfo::DeviceInfo()
{
    initDeviceInfo();
}

void DeviceInfo::initDeviceInfo()
{
    device_id = Utility::getDeviceId();
    platform = Utility::getKernelVariant();
    os = Utility::getOsVersion();
    resolution = Utility::getResolution();
    // !!!TODO: appkey
}

}
