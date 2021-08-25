#include "allmodel.h"

#include "../Common/urlencode.h"
#include "../Common/utility.h"
#include "../UMS/umsmanager.h"
#include <infoware/version.hpp>

namespace UMSAgent {
AllModel::AllModel(const std::string& appkey)
    : key(appkey)
{
}

Event AllModel::getEventInfo(const std::string& eventid, const std::string& pagename, const std::string& label, int acc)
{
    Event e;
    e.event_identifier = eventid;
    e.activity = UrlEncodeString(pagename);
    e.time = Utility::getTime();
    e.appkey = key;
    e.label = UrlEncodeString(label);
    e.version = Utility::getApplicationVersion();
    e.acc = acc;
    return e;
}

OnLineConfig AllModel::getOnlineConfig()
{
    OnLineConfig obj;
    obj.appkey = UmsManager::getInstance().appkey;
    return obj;
}

UpdatePreference AllModel::getUpdatePreference(const std::string& version)
{
    UpdatePreference obj;
    obj.appkey = UmsManager::getInstance().appkey;
    obj.version_code = version;
    return obj;
}

Tag AllModel::getTagData(const std::string& tags)
{
    Tag tag;
    tag.appkey = UmsManager::getInstance().appkey;
    tag.tags = UrlEncodeString(tags);
    tag.deviceid = Utility::getDeviceId();
    return tag;
}

Userid AllModel::getUseridData(const std::string& userid)
{
    Userid userids;
    userids.appkey = UmsManager::getInstance().appkey;
    userids.userid = userid;
    userids.deviceid = Utility::getDeviceId();
    return userids;
}

Pushid AllModel::getPushidData(const std::string& pushid)
{
    Pushid pushids;
    pushids.appkey = UmsManager::getInstance().appkey;
    pushids.clientid = pushid;
    pushids.deviceid = Utility::getDeviceId();
    return pushids;
}

ClientData AllModel::getClientData()
{
    ClientData clientdata;
    clientdata.platform = Utility::getKernelVariant();
    clientdata.os_version = Utility::getOsVersion();
    // clientdata.language = ?
    clientdata.resolution = Utility::getResolution();
    clientdata.deviceid = Utility::getDeviceId();
    clientdata.devicename = Utility::getDeviceName();
    clientdata.version = Utility::getApplicationVersion();
    clientdata.appkey = key;
    clientdata.time = Utility::getTime();
    // clientdata.userid
    clientdata.isMobileDevice = false;
    return clientdata;
}

std::string AllModel::getUrl(DataType type)
{
    std::string url;
    switch (type) {
    case DataType::ClientData:
        break;
    }
    return url;
}

}
