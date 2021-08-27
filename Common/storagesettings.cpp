#include "storagesettings.h"

namespace UMSAgent {
LocalStorage::LocalStorage()
{
    ini = std::make_unique<CSimpleIniA>();
    ini->LoadFile("UMSAgent.ini");
}

bool LocalStorage::Contains(const std::string& section, const std::string& key)
{
    const char* value = ini->GetValue(section.c_str(), key.c_str());
    return (value == NULL) ? false : true;
}

bool LocalStorage::Add(const std::string& section, const std::string& key, const std::string& value)
{
    std::lock_guard<std::mutex> _l(lock);
    SI_Error rc = ini->SetValue(section.c_str(), key.c_str(), value.c_str());
    return (rc == SI_OK) ? true : false;
}

bool LocalStorage::Remove(const std::string& section, const std::string& key)
{
    std::lock_guard<std::mutex> _l(lock);
    bool ret = ini->Delete(section.c_str(), key.c_str());
    return ret;
}

bool LocalStorage::Set(const std::string &section, const std::string &key, const std::string &value)
{
    std::lock_guard<std::mutex> _l(lock);
    SI_Error rc = ini->SetValue(section.c_str(), key.c_str(), value.c_str());
    return (rc == SI_OK) ? true : false;
}

bool LocalStorage::Save()
{
    std::lock_guard<std::mutex> _l(lock);
    SI_Error rc = ini->SaveFile("UMSAgent.ini");
    return (rc == SI_OK) ? true : false;
}

std::string LocalStorage::Get(const std::string& section, const std::string& key)
{
    const char* value = ini->GetValue(section.c_str(), key.c_str(), "");
    return std::string(value);
}

std::string ApplicationSettings::SECTION = "Application";
bool ApplicationSettings::Contains(const std::string& key)
{
    return LocalStorage::getInstance().Contains(SECTION, key);
}

bool ApplicationSettings::Add(const std::string& key, const std::string& value)
{
    return LocalStorage::getInstance().Add(SECTION, key, value);
}

bool ApplicationSettings::Remove(const std::string& key)
{
    return LocalStorage::getInstance().Remove(SECTION, key);
}

bool ApplicationSettings::Save()
{
    return LocalStorage::getInstance().Save();
}

bool ApplicationSettings::Set(const std::string &key, const std::string &value)
{
    return LocalStorage::getInstance().Set(SECTION, key, value);
}

std::string ApplicationSettings::operator[](const std::string& key) const
{
    std::string value = LocalStorage::getInstance().Get(SECTION, key);
    return value;
}
}
