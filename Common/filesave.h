#ifndef FILESAVE_H
#define FILESAVE_H

#include "../MyObject/datatype.h"
#include "../Common/storagesettings.h"
#include "../Common/obj2json.h"

namespace UMSAgent {
class FileSave
{
public:
    template <typename T>
    static void saveFile(DataType type, T const& obj);
};

template <typename T>
void FileSave::saveFile(DataType type, T const& obj) {
    std::string jsonstr = Obj2Json::obj2jsonstr<T>(obj);
    ApplicationSettings settings;
    std::string key = GetTypeString(type);
    if (settings.Contains(key)) {
        std::string json = settings[key];
        std::vector<T> list = jsoncons::decode_json<std::vector<T>>(json);
        list.push_back(obj);
    } else {
        std::vector<T> list { obj };
        std::string json;
        jsoncons::encode_json<std::vector<T>>(list, json);
        settings.Add(key, json);
    }
    settings.Save();
}
}

#endif // FILESAVE_H
