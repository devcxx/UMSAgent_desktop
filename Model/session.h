#ifndef SESSION_H
#define SESSION_H

#include <map>
#include <string>
#include "../Common/variant.hpp"

namespace UMSAgent {
class Session {
public:
    Session();
    void initNewSession();
    static Session initSessionWithOldData();
    void generateSessionID();
    void onPageStart(const std::string& pagename);
    void onPageEnd(const std::string& pagename);

public:
    static std::string UMS_SESSION_ID;
    typedef nonstd::variant<int, std::int64_t, std::string> Variant;
    std::map<std::string, Variant> pageDictionary;
    std::string endtime;
};
}

#endif // SESSION_H
