#ifndef TAG_H
#define TAG_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct Tag {
    std::string appkey;
    std::string tags;
    std::string deviceid;
};
}
JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::Tag, appkey, tags, deviceid)

#endif // TAG_H
