#ifndef ONLINECONFIG_H
#define ONLINECONFIG_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct OnLineConfig {
    std::string appkey;
};
}
JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::OnLineConfig, appkey)

#endif // ONLINECONFIG_H
