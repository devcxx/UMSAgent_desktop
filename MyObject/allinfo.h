#ifndef ALLINFO_H
#define ALLINFO_H
#include "clientdata.h"
#include "errorinfo.h"
#include "event.h"
#include "pageinfo.h"
#include "tag.h"
#include <jsoncons/json.hpp>
#include <vector>

namespace UMSAgent {
struct AllInfo {
    std::vector<ClientData> clientData;
    std::vector<ErrorInfo> errorInfo;
    std::vector<Event> eventInfo;
    std::vector<PageInfo> activityInfo;
    std::vector<Tag> tagListInfo;
    std::string appkey;
};
}

JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::AllInfo, clientData, errorInfo, eventInfo,
    activityInfo, tagListInfo, appkey)

#endif // ALLINFO_H
