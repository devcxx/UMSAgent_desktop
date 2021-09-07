#ifndef UPLOADER_H
#define UPLOADER_H
#include <string>

class Uploader
{
public:
    Uploader();
    static bool upload(const std::string& url, const std::string& file);
};

#endif // UPLOADER_H
