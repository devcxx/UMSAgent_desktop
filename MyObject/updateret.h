#ifndef UPDATERET_H
#define UPDATERET_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct UpdateRet {
    std::string time;
    std::string version;
    std::string description;
    std::string fileurl;
};
}
JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::UpdateRet, time, version, description, fileurl)

#endif // UPDATERET_H
