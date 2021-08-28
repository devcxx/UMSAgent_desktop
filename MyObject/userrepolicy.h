#ifndef USERREPOLICY_H
#define USERREPOLICY_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct UserRepolicy {
    std::string autoLocation;
    std::string updateOnlyWifi;
    std::string sessionTime;
    std::string repolicy;
};
}

JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::UserRepolicy, autoLocation, updateOnlyWifi, sessionTime, repolicy)

#endif // USERREPOLICY_H
