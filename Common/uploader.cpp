#include "uploader.h"
#include <curl/curl.h>
#include <fstream>

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || (defined(__cplusplus) && __cplusplus >= 201703L)) && defined(__has_include)
#if __has_include(<filesystem>) && (!defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED >= 101500)
#define GHC_USE_STD_FS
#include <filesystem>
namespace fs = std::filesystem;
#endif
#endif
#ifndef GHC_USE_STD_FS
#include <filesystem.hpp>
#include "easylogging++.h"

namespace fs = ghc::filesystem;
#endif

Uploader::Uploader()
{
}

bool Uploader::upload(const std::string& url, const std::string& file)
{
    bool ret = false;
    std::string contents;
    std::ifstream in(file, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
    }
    CURL* curl;
    CURLcode res;
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* lastptr = NULL;
    struct curl_slist* headerlist = NULL;
    static const char buf[] = "Expect:";

    // set up the header
    curl_formadd(&formpost,
        &lastptr,
        CURLFORM_COPYNAME, "cache-control:",
        CURLFORM_COPYCONTENTS, "no-cache",
        CURLFORM_END);

    curl_formadd(&formpost,
        &lastptr,
        CURLFORM_COPYNAME, "content-type:",
        CURLFORM_COPYCONTENTS, "multipart/form-data",
        CURLFORM_END);

    std::string filename = fs::path(file).filename().string();
    curl_formadd(&formpost, &lastptr,
        CURLFORM_COPYNAME, "file", // <--- the (in this case) wanted file-Tag!
        CURLFORM_BUFFER, filename.c_str(),
        CURLFORM_BUFFERPTR, contents.data(),
        CURLFORM_BUFFERLENGTH, contents.size(),
        CURLFORM_END);

    curl = curl_easy_init();

    headerlist = curl_slist_append(headerlist, buf);
    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            LOG(ERROR) << "upload failed:" << curl_easy_strerror(res);
        } else {
            ret = true;
            LOG(INFO) << "upload success:" << filename;
        }

        curl_easy_cleanup(curl);
        curl_formfree(formpost);
        curl_slist_free_all(headerlist);
        return ret;
    }

    return false;
}
