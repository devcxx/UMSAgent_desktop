#ifndef POST_H
#define POST_H

#include "../MyObject/datatype.h"
#include "Delegate.hpp"
#include "../Common/obj2json.h"
#include "../MyObject/commonret.h"
#include <restclient-cpp/restclient.h>
#include <restclient-cpp/connection.h>
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
//    Delegate::Delegate<void(const DataType&, const std::string&, const std::string&)> stateChanged;
};

template<class T>
void Post<T>::getPostInfo(DataType type, const T& data) {
    message = Obj2Json::obj2jsonstr<T>(data);
}

template<class T>
void Post<T>::sendData(const std::string &url)
{
    std::string data = "content={\"data\":[" + message+ "]}";
    std::string ctype = "application/x-www-form-urlencoded";
    RestClient::Response r;
    RestClient::Connection *conn = new RestClient::Connection("");
    // set connection timeout to 5s
    conn->SetTimeout(5);
    conn->AppendHeader("Content-Type", ctype);
    r = conn->post(url, data);
    delete conn;

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
