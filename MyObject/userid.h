#ifndef USERID_H
#define USERID_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct Userid {
    std::string appkey;
    std::string userid;
    std::string deviceid;
};
}
JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::Userid, appkey, userid, deviceid)

#endif // USERID_H
