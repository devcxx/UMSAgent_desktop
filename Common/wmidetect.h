#ifndef WMIDETECT_H
#define WMIDETECT_H
#include <string>

namespace UMSAgent {
namespace WMIDetect
{
    bool Query(const std::string table, std::wstring wcol, std::string& result);
};
}

#endif // WMIDETECT_H
