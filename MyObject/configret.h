#ifndef CONFIGRET_H
#define CONFIGRET_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct ConfigRet {
    std::string reportpolicy;
    std::string autolocation;
    std::string updateonlywifi;
    std::string sessionmillis;
};
}
JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::ConfigRet, reportpolicy,
    autolocation, updateonlywifi, sessionmillis)

#endif // CONFIGRET_H
