#ifndef UPDATEPREFERENCE_H
#define UPDATEPREFERENCE_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct UpdatePreference {
    std::string appkey;
    std::string version_code;
};
}

JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::UpdatePreference, appkey, version_code)

#endif // UPDATEPREFERENCE_H
