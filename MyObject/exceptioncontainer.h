#ifndef EXCEPTIONCONTAINER_H
#define EXCEPTIONCONTAINER_H

#include <jsoncons/json.hpp>
#include <string>

namespace UMSAgent {
struct ExceptionContainer {
    std::string Message;
    std::string StackTrace;
};
}

JSONCONS_ALL_MEMBER_TRAITS(UMSAgent::ExceptionContainer, Message, StackTrace)

#endif // EXCEPTIONCONTAINER_H
