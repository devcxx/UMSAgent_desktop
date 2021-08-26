#ifndef OBJ2JSON_H
#define OBJ2JSON_H

#include "../MyObject/datatype.h"

namespace UMSAgent {
class Obj2Json
{
public:
    template <typename T>
    static std::string obj2jsonstr(const T& data);
    static std::string allData2jsonstr();
};

template <typename T>
std::string Obj2Json::obj2jsonstr(const T& data)
{
    std::string ret;
    jsoncons::encode_json<T>(data, ret);
    return ret;
}


}

#endif // OBJ2JSON_H
