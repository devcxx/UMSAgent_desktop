#ifndef COMMONRET_H
#define COMMONRET_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct CommonRet {
    std::string flag;
    std::string msg;
};
}
JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::CommonRet, flag, msg)
#endif // COMMONRET_H
