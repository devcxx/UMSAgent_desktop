#ifndef DATATYPE_H
#define DATATYPE_H

#include <jsoncons/json.hpp>

namespace UMSAgent {
enum class DataType {
    ClientData = 0,
    OnlineConfig,
    NewVersion,
    Event,
    All,
    Error,
    Activity,
    Tag,
    Userid,
    Pushid
};
}

JSONCONS_ENUM_TRAITS(UMSAgent::DataType, ClientData, OnlineConfig,
    NewVersion, Event, All, Error, Activity, Tag, Userid, Pushid)

#endif // DATATYPE_H
