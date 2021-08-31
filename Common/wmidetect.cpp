#include "wmidetect.h"
#include <WbemIdl.h>
#include <Windows.h>
#include <atlbase.h>
#include <atlstr.h>
#include <comdef.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <tchar.h>
#pragma comment(lib, "wbemuuid.lib")
using namespace std;
namespace UMSAgent {
bool WMIDetect::Query(const std::string table, std::wstring wcol, std::string& result)
{
    HRESULT hres; //函数返回值
    char bord[1024];
    //初始化COM
    hres = CoInitialize(0);
    //获得WMI连接COM接口
    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&pLoc);
    if (FAILED(hres)) {
        cout << "Failed to create IWbemLocator object."
             << "Err code = 0x"
             << hex << hres << endl;
        CoUninitialize();
        return false;
    }
    //通过连接接口连接WMI的内核对象名ROOT//CIMV2
    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), //Object path of WMI namespace
        NULL, //User name. NULL = current user
        NULL, //User password. NULL = current
        0, //Locale. NULL indicates current
        NULL, //Security flags.
        0, //Authority(e.r. Kerberos)
        0, //Context object
        &pSvc //pointer to IWbemServices proxy
    );
    if (FAILED(hres)) {
        cout << "Could not connect.Error code = 0x"
             << hex << hres << endl;
        pLoc->Release();
        CoUninitialize();
        return false;
    }
    //设置请求代理的安全级别
    hres = CoSetProxyBlanket(
        pSvc, // Indicates the proxy to set
        RPC_C_AUTHN_WINNT, //RPC_C_AUTHN_xxx
        RPC_C_AUTHZ_NONE, //RPC_C_AUTHZ_xxx
        NULL, // Server principal name
        RPC_C_AUTHN_LEVEL_CALL, // RPC_C_AUTHN_LEVEL_xxx
        RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
        NULL, // client identity
        EOAC_NONE //proxy capabilities
    );
    if (FAILED(hres)) {
        cout << "Could noe set proxy blanket. Error code = 0x"
             << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }
    //通过请求代理来向WMI发送请求
    IEnumWbemClassObject* pEnumerator = NULL;
    string select = "SELECT * FROM " + table;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t(select.c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);
    if (FAILED(hres)) {
        cout << "Query for Network Adapter Configuration failed."
             << "Error code = 0x"
             << hex << hres << endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }
    //循环枚举所有的结果对象
    ULONG uReturn = 0;
    IWbemClassObject* pclsObj = NULL;
    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) {
            break;
        }
        VARIANT vtProp;
        VariantInit(&vtProp);
        hr = pclsObj->Get(wcol.c_str(), 0, &vtProp, 0, 0);
        if (!FAILED(hr)) {
            CW2A tmpstr1(vtProp.bstrVal);
            strcpy_s(bord, 200, tmpstr1);
            result = bord;
        }
        VariantClear(&vtProp);
    }
    //释放资源
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    pclsObj->Release();
    CoUninitialize();
    return true;
}

}
