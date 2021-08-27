#ifndef POST_H
#define POST_H

#include "../MyObject/datatype.h"
#include "Delegate.hpp"
#include "../Common/obj2json.h"
#include "../MyObject/commonret.h"
#include <restclient-cpp/restclient.h>
#include "../Common/ThreadPool.h"
#include "../Common/Delegate.hpp"
#include "../Common/filesave.h"

#include <string>
namespace UMSAgent {

template <class T>
class Post
{
public:
    Post(DataType t, T const& o) : type(t), obj(o) {
        getPostInfo(type, obj);
    }
    void getPostInfo(DataType type, const T& data);
    void sendData(const std::string& url);

    std::string my_ur;
    std::string message;
    T obj;
    std::string ret;
    DataType type;
    ThreadPool pool{5};
//    Delegate::Delegate<void(const DataType&, const std::string&, const std::string&)> stateChanged;
};

template<class T>
void Post<T>::getPostInfo(DataType type, const T& data) {
    message = Obj2Json::obj2jsonstr<T>(data);
}

template<class T>
void Post<T>::sendData(const std::string &url)
{
    RestClient::Response r = RestClient::post(url, "application/x-www-form-urlencoded", message);
    CommonRet errorRet;
    errorRet.flag = std::to_string(r.code);
    errorRet.msg = r.body;
    std::string ret;
    ret = Obj2Json::obj2jsonstr<CommonRet>(errorRet);
//    stateChanged(type, ret, message);
    if (r.code != 200) {
        FileSave::saveFile<T>(type, obj);
    }
}
}

#endif // POST_H
