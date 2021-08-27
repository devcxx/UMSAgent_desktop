#include "datamanager.h"
#include "../Common/utility.h"
#include "../Common/post.h"
#include "../MyObject/tag.h"
#include "../MyObject/allinfo.h"
#include "../Common/filesave.h"
#include "../Common/Delegate.hpp"

namespace UMSAgent {
DataManager::DataManager()
{
}

void DataManager::clientDataProceed()
{
    ClientData obj = model.getClientData();
    if (Utility::isNetWorkConnected()) {
        Post<ClientData> post(DataType::CLIENTDATA, obj);
//        std::function<void(DataType type, const std::string&, ClientData const&)> fn = &DataManager::getData<ClientData>;
//        post.stateChanged.Subscribe(&DataManager::getData<ClientData>);
        post.sendData(model.getUrl(DataType::CLIENTDATA));
    } else {
        FileSave::saveFile(DataType::CLIENTDATA, obj);
    }
}

void DataManager::eventDataProceed(const std::string &eventid, const std::string &pagename, const std::string &lable, int acc, double count)
{
    Event obj = model.getEventInfo(eventid, pagename, lable,acc);
    if (settings["repolicy"] == "1" && Utility::isNetWorkConnected()) {
        Post<Event> post(DataType::ERRORDATA, obj);
        post.sendData(model.getUrl(DataType::EVENTDATA));
    } else {
        FileSave::saveFile(DataType::EVENTDATA, obj);
    }
}

void DataManager::allDataProceed()
{
    if (Utility::isNetWorkConnected() && settings["hasDateToSend"] == "1") {

    }
}

void DataManager::crashDataProceed(const std::string& time, const std::string& stacktrace)
{
    ErrorInfo obj = model.getErrorInfo(time, stacktrace);
    if (settings["repolicy"] == "1" && Utility::isNetWorkConnected()) {
        Post<ErrorInfo> post(DataType::ERRORDATA, obj);
        post.sendData(model.getUrl(DataType::ERRORDATA));
    } else {
        FileSave::saveFile(DataType::ERRORDATA, obj);
    }
}

void DataManager::pageInfoDataProceed(const PageInfo &obj)
{
    if (settings["repolicy"] == "1" && Utility::isNetWorkConnected()) {
        Post<PageInfo> post(DataType::PAGEINFODATA, obj);
        post.sendData(model.getUrl(DataType::PAGEINFODATA));
    } else {
        FileSave::saveFile(DataType::PAGEINFODATA, obj);
    }
}

void DataManager::tagDataProceed(const std::string &tags)
{
    Tag obj = model.getTagData(tags);
    if (settings["repolicy"] == "1" && Utility::isNetWorkConnected()) {
        Post<Tag> post(DataType::TAGDATA, obj);
        post.sendData(model.getUrl(DataType::TAGDATA));
    } else {
        FileSave::saveFile(DataType::TAGDATA, obj);
    }
}

void DataManager::useridDataProceed(const std::string &userid)
{
    Userid obj = model.getUseridData(userid);
    Post<Userid> post(DataType::USERID, obj);
    post.sendData(model.getUrl(DataType::USERID));
}

void DataManager::pushidDataProceed(const std::string &pushid)
{
    Pushid obj = model.getPushidData(pushid);
    Post<Pushid> post(DataType::PUSHID, obj);
    post.sendData(model.getUrl(DataType::PUSHID));
}


}
