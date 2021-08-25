#ifndef SESSION_H
#define SESSION_H

#include <map>
#include <string>

namespace UMSAgent {
class Session {
public:
    Session();
    void initNewSession();
    void generateSessionID();
    void onPageStart(const std::string& pagename);
    void onPageEnd(const std::string& pagename);

public:
    static std::string UMS_SESSION_ID;

private:
    std::map<std::string, std::string> m_pageDictionary;
};
}

#endif // SESSION_H
