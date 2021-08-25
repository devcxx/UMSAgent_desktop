#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace UMSAgent {
namespace Constants {
    const std::string kBaseUrl = "";
    const std::string kPostClientDataUrl = "?/ums/postClientData";
    const std::string kCheckNewVersionUrl = "?/ums/getApplicationUpdate";
    const std::string kGetOnlineConfigUrl = "?/ums/getOnlineConfiguration";
    const std::string kPostEventUrl = "?/ums/postEvent";
    const std::string kAllDataUrl = "?/ums/uploadLog";
    const std::string kErrorDataUrl = "?/ums/postErrorLog";
    const std::string kPostActivityLog = "?/ums/postActivityLog";
    const std::string kPostTag = "?/ums/postTag";
    const std::string kPostUserid = "?/ums/postUserid";
    const std::string kPostPushid = "?/ums/postPushid";
}
}

#endif // CONSTANTS_H
