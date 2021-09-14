#include "umsutility.h"
#include "Common/utility.h"

bool UMSUtility::isNetWorkConnected()
{
    return UMSAgent::Utility::isNetWorkConnected();
}

std::string UMSUtility::getDeviceId()
{
    return UMSAgent::Utility::getDeviceId();
}

std::string UMSUtility::getOsVersion()
{
    return UMSAgent::Utility::getOsVersion();
}

std::string UMSUtility::getPlatform()
{
    return UMSAgent::Utility::getPlatform();
}

std::string UMSUtility::getResolution()
{
    return UMSAgent::Utility::getResolution();
}

std::string UMSUtility::getDeviceName()
{
    return UMSAgent::Utility::getDeviceName();
}

std::string UMSUtility::getDefaultBrowser()
{
    return UMSAgent::Utility::getDefaultBrowser();
}

std::string UMSUtility::getComputerModel()
{
    return UMSAgent::Utility::GetComputerModel();
}
