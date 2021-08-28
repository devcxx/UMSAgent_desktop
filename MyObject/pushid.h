#ifndef PUSHID_H
#define PUSHID_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct Pushid {
    std::string appkey;
    std::string clientid;
    std::string deviceid;
};
}
JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::Pushid, appkey, clientid, deviceid)

#endif // PUSHID_H
