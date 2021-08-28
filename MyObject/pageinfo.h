#ifndef PAGEINFO_H
#define PAGEINFO_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct PageInfo {
    std::string start_millis;
    std::string end_millis;
    std::string activities;
    std::string version;
    std::string appkey;
    std::string session_id;
    std::string duration;
};
}
JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::PageInfo, start_millis, end_millis,
    activities, version, appkey, session_id, duration)

#endif // PAGEINFO_H
