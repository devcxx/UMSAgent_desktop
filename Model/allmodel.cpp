#include "allmodel.h"

#include "../Common/urlencode.h"
#include "../Common/utility.h"
#include "../Common/constants.h"
#include "../UMS/umsmanager.h"
#include "../Common/storagesettings.h"

namespace UMSAgent {
AllModel::AllModel()
{
}

Event AllModel::getEventInfo(const std::string& eventid, const std::string& pagename, const std::string& label, int acc)
{
    Event e;
    e.event_identifier = eventid;
    e.activity = UrlEncodeString(pagename);
    e.time = Utility::getTime();
    e.appkey = UmsManager::getInstance().appkey;
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
    clientdata.platform = Utility::getPlatform();
    clientdata.os_version = Utility::getOsVersion();
    // clientdata.language = ?
    clientdata.resolution = Utility::getResolution();
    clientdata.deviceid = Utility::getDeviceId();
    clientdata.devicename = Utility::getDeviceName();
    clientdata.version = Utility::getApplicationVersion();
    clientdata.appkey = UmsManager::getInstance().appkey;
    clientdata.time = Utility::getTime();
    ApplicationSettings settings;
    clientdata.useridentifier = settings["UserIdentifier"];
    clientdata.sessionId = settings["cobub_session_id"];
    clientdata.language = Utility::getApplicationLanguage();
    // clientdata.userid
    clientdata.isMobileDevice = false;
    clientdata.network = Utility::GetNetStates();
    clientdata.modulename = Utility::GetComputerModel();
    clientdata.defaultbrowser = "";
    return clientdata;
}

ErrorInfo AllModel::getErrorInfo(const std::string& time, const std::string& stacktrace)
{
    ErrorInfo error;
    error.appkey = UmsManager::getInstance().appkey;
    error.stacktrace = stacktrace;
    error.time = time;
    error.version = Utility::getApplicationVersion();
    error.deviceid = Utility::getDeviceId();
    error.os_version = Utility::getPlatform();
    error.devicename = Utility::getDeviceName();
    ApplicationSettings settings;
    error.useridentifier = settings["UserIdentifier"];
    return error;
}

std::string AllModel::getUrl(DataType type)
{
    std::string url;
    switch (type) {
    case DataType::CLIENTDATA: //get client data
        url = Constants::kBaseUrl + Constants::kPostClientDataUrl;
        break;
    case DataType::CONFIGDATA: //get online config data
        url = Constants::kBaseUrl + Constants::kGetOnlineConfigUrl;
        break;
    case DataType::UPDATEDATA: //check new version data
        url = Constants::kBaseUrl + Constants::kCheckNewVersionUrl;
        break;
    case DataType::EVENTDATA: //post user event data
        url = Constants::kBaseUrl + Constants::kPostEventUrl;
        break;
    case DataType::AllDATA: // all data
        url = Constants::kBaseUrl + Constants::kAllDataUrl;
        break;
    case DataType::ERRORDATA: // error data
        url = Constants::kBaseUrl + Constants::kErrorDataUrl;
        break;
    case DataType::PAGEINFODATA: // page info data
        url = Constants::kBaseUrl + Constants::kPostActivityLog;
        break;
    case DataType::TAGDATA: //tag data
        url = Constants::kBaseUrl + Constants::kPostTag;
        break;
    case DataType::USERID: //post userid
        url = Constants::kBaseUrl + Constants::kPostPushid;
        break;
    case DataType::PUSHID: //post pushid
        url = Constants::kBaseUrl + Constants::kPostPushid;
        break;
    default:
        break;
    }
    return url;
}

}
