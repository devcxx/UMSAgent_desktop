#include "storagesettings.h"
#include <iostream>
#include <iomanip>
#include <memory>

namespace UMSAgent {

static const char kFile[] = "UMSAgent.ini";
static const long kMaxSize = 1048576; // 1MB

static long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

LocalStorage::LocalStorage()
{
    ini = std::make_unique<CSimpleIniA>();
    long fSize = GetFileSize(kFile);
    if (fSize > kMaxSize)
        std::remove(kFile);
    ini->LoadFile(kFile);
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
    SI_Error rc = ini->SaveFile(kFile);
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

std::string ApplicationSettings::Get(const std::string &key)
{
    std::string value = LocalStorage::getInstance().Get(SECTION, key);
    return value;
}

std::string ApplicationSettings::operator[](const std::string& key) const
{
    std::string value = LocalStorage::getInstance().Get(SECTION, key);
    return value;
}
}
