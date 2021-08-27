#include "obj2json.h"
#include "../Common/storagesettings.h"
#include "../MyObject/allinfo.h"
#include "../MyObject/clientdata.h"
#include "../MyObject/errorinfo.h"
#include "../MyObject/event.h"
#include "../MyObject/onlineconfig.h"
#include "../MyObject/pageinfo.h"
#include "../MyObject/pushid.h"
#include "../MyObject/tag.h"
#include "../MyObject/updatepreference.h"
#include "../MyObject/userid.h"
#include "../UMS/umsmanager.h"

#include <jsoncons/json.hpp>
namespace UMSAgent {
using namespace jsoncons;
std::string Obj2Json::allData2jsonstr()
{
    AllInfo allinfo;
    allinfo.appkey = UmsManager::getInstance().appkey;
    ApplicationSettings settings;
    if (settings.Contains("clientdata")) {
        std::vector<ClientData> list_client_data = decode_json<std::vector<ClientData>>(settings["clientdata"]);
        allinfo.clientData = list_client_data;
    }

    if (settings.Contains("tagdata")) {
        std::vector<Tag> list_tag_data = decode_json<std::vector<Tag>>(settings["tagdata"]);
        allinfo.tagListInfo = list_tag_data;
    }

    if (settings.Contains("eventdata")) {
        std::vector<Event> list_event_data = decode_json<std::vector<Event>>(settings["eventdata"]);
        allinfo.eventInfo = list_event_data;
    }

    if (settings.Contains("errordata")) {
        std::vector<ErrorInfo> err_list = decode_json<std::vector<ErrorInfo>>(settings["errordata"]);
        allinfo.errorInfo = err_list;
    }

    if (settings.Contains("pageinfo")) {
        std::vector<PageInfo> list_pageinfo_data = decode_json<std::vector<PageInfo>>(settings["pageinfo"]);
        allinfo.activityInfo = list_pageinfo_data;

    }

    std::string ret;
    jsoncons::encode_json<AllInfo>(allinfo, ret);
    return ret;
}

}
