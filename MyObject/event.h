#ifndef EVENT_H
#define EVENT_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct Event {
    std::string event_identifier;
    std::string time;
    std::string activity;
    std::string label;
    std::string appkey;
    std::string version;
    int acc;
};
}

JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::Event, event_identifier,
    time, activity, label, appkey, version, acc)

#endif // EVENT_H
