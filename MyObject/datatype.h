#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>
#include <jsoncons/json.hpp>

namespace UMSAgent {
enum class DataType {
    CLIENTDATA = 0,
    CONFIGDATA,
    UPDATEDATA,
    EVENTDATA,
    AllDATA,
    ERRORDATA,
    PAGEINFODATA,
    TAGDATA,
    USERID,
    PUSHID
};

inline std::string GetTypeString(DataType type) {
    switch (type) {
    case DataType::CLIENTDATA:
        return "clientdata";
    case DataType::CONFIGDATA:
        return "configdata";
    case DataType::UPDATEDATA:
        return "updatedate";
    case DataType::EVENTDATA:
        return "eventdata";
    case DataType::TAGDATA:
        return "tagdata";
    case DataType::ERRORDATA:
        return "errordata";
    case DataType::PAGEINFODATA:
        return "pageinfo";
    case DataType::AllDATA:
        return "alldata";
    case DataType::USERID:
        return "userid";
    case DataType::PUSHID:
        return "pushid";
    }
}
}

JSONCONS_ENUM_TRAITS(UMSAgent::DataType, CLIENTDATA, CONFIGDATA,
    UPDATEDATA, EVENTDATA, AllDATA, ERRORDATA, PAGEINFODATA, TAGDATA, USERID, PUSHID)

#endif // DATATYPE_H
