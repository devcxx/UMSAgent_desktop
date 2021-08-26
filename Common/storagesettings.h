#ifndef STORAGESETTINGS_H
#define STORAGESETTINGS_H

#include "../Common/Singleton.h"
#include "../Common/SimpleIni.h"

namespace UMSAgent {
class LocalStorage : public Singleton<LocalStorage>
{
public:
    LocalStorage();
    bool Contains(const std::string& section, const std::string& key);
    bool Add(const std::string& section, const std::string& key, const std::string& value);
    bool Remove(const std::string& section, const std::string& key);
    bool Set(const std::string& section, const std::string& key, const std::string& value);
    bool Save();
    std::string Get(const std::string& section, const std::string& key);
private:
    std::unique_ptr<CSimpleIniA> ini;
    std::mutex lock;
};

class ApplicationSettings {
public:
    bool Contains(const std::string& key);
    bool Add(const std::string& key, const std::string& value);
    bool Remove(const std::string& key);
    bool Save();
    bool Set(const std::string& key, const std::string& value);
    std::string operator[](const std::string& key) const;

public:
    static std::string SECTION;

};

}

#endif // STORAGESETTINGS_H
