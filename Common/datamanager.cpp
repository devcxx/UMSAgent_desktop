#include "datamanager.h"
#include "../Common/Delegate.hpp"
#include "../Common/filesave.h"
#include "../Common/post.h"
#include "../Common/utility.h"
#include "../MyObject/allinfo.h"
#include "../MyObject/tag.h"
#include "../UMS/umsmanager.h"

namespace UMSAgent {
DataManager::DataManager()
{
    appkey = UmsManager::getInstance().appkey;
    model.key = appkey;
}

void DataManager::clientDataProceed()
{
    ClientData obj = model.getClientData();
    if (Utility::isNetWorkConnected()) {
        Post<ClientData> post(DataType::CLIENTDATA, obj);
        post.sendData(model.getUrl(DataType::CLIENTDATA));
    } else {
        FileSave::saveFile(DataType::CLIENTDATA, obj);
    }
}

void DataManager::eventDataProceed(const std::string& eventid, const std::string& pagename, const std::string& lable, int acc, double count)
{
    Event obj = model.getEventInfo(eventid, pagename, lable, acc);
    if (Utility::isNetWorkConnected()) {
        Post<Event> post(DataType::ERRORDATA, obj);
        post.sendData(model.getUrl(DataType::EVENTDATA));
    } else {
        FileSave::saveFile(DataType::EVENTDATA, obj);
    }
}

void DataManager::onlineConfigProceed()
{
    OnLineConfig obj = model.getOnlineConfig();
    if (Utility::isNetWorkConnected()) {
        Post<OnLineConfig> post(DataType::CONFIGDATA, obj);
        post.sendData(model.getUrl(DataType::CONFIGDATA));
    }
}

void DataManager::allDataProceed()
{
    if (Utility::isNetWorkConnected() && settings["hasDateToSend"] == "1") {
        std::string allData = Obj2Json::allData2jsonstr();
        RestClient::Response res = RestClient::post(model.getUrl(DataType::AllDATA), "application/x-www-form-urlencoded", allData);
        if (res.code == 200) {
            ApplicationSettings settings;
            if (settings.Contains("clientdata")) {
                settings.Remove("clientdata");
            }
            if (settings.Contains("eventdata")) {
                settings.Remove("eventdata");
            }
            if (settings.Contains("errordata")) {
                settings.Remove("errordata");
            }
            if (settings.Contains("pageinfo")) {
                settings.Remove("pageinfo");
            }
            if (settings.Contains("hasDateToSend")) {
                settings.Remove("hasDateToSend");
            }
            settings.Save();
        }
    }
}

void DataManager::crashDataProceed(const std::string& time, const std::string& stacktrace)
{
    ErrorInfo obj = model.getErrorInfo(time, stacktrace);
    if (Utility::isNetWorkConnected()) {
        Post<ErrorInfo> post(DataType::ERRORDATA, obj);
        post.sendData(model.getUrl(DataType::ERRORDATA));
    } else {
        FileSave::saveFile(DataType::ERRORDATA, obj);
    }
}

void DataManager::pageInfoDataProceed(const PageInfo& obj)
{
    if (Utility::isNetWorkConnected()) {
        Post<PageInfo> post(DataType::PAGEINFODATA, obj);
        post.sendData(model.getUrl(DataType::PAGEINFODATA));
    } else {
        FileSave::saveFile(DataType::PAGEINFODATA, obj);
    }
}

void DataManager::tagDataProceed(const std::string& tags)
{
    Tag obj = model.getTagData(tags);
    if (Utility::isNetWorkConnected()) {
        Post<Tag> post(DataType::TAGDATA, obj);
        post.sendData(model.getUrl(DataType::TAGDATA));
    } else {
        FileSave::saveFile(DataType::TAGDATA, obj);
    }
}

void DataManager::useridDataProceed(const std::string& userid)
{
    Userid obj = model.getUseridData(userid);
    Post<Userid> post(DataType::USERID, obj);
    post.sendData(model.getUrl(DataType::USERID));
}

void DataManager::pushidDataProceed(const std::string& pushid)
{
    Pushid obj = model.getPushidData(pushid);
    Post<Pushid> post(DataType::PUSHID, obj);
    post.sendData(model.getUrl(DataType::PUSHID));
}

}
