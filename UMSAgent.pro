QT -= gui

QT += core

TEMPLATE = lib
DEFINES += UMSAGENT_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Common/constants.cpp \
    Common/urlencode.cc \
    Common/utility.cpp \
    Common/machineid.cpp \
    Common/ConvertUTF.c \
    Common/guid.cpp \
    Model/allmodel.cpp \
    Model/deviceinfo.cpp \
    Model/session.cpp \
    MyObject/allinfo.cpp \
    MyObject/clientdata.cpp \
    MyObject/commonret.cpp \
    MyObject/configret.cpp \
    MyObject/errorinfo.cpp \
    MyObject/event.cpp \
    MyObject/exceptioncontainer.cpp \
    MyObject/onlineconfig.cpp \
    MyObject/pageinfo.cpp \
    MyObject/pushid.cpp \
    MyObject/tag.cpp \
    MyObject/updatepreference.cpp \
    MyObject/updateret.cpp \
    MyObject/userid.cpp \
    MyObject/userrepolicy.cpp \
    UMS/umsmanager.cpp \
    umsagent.cpp

HEADERS += \
    Common/Singleton.h \
    Common/utility.h \
    Common/machineid.h \
    Common/ConvertUTF.h \
    Common/SimpleIni.h \
    Common/guid.hpp \
    Model/allmodel.h \
    Model/deviceinfo.h \
    Model/session.h \
    MyObject/datatype.h \
    MyObject/configret.h \
    MyObject/errorinfo.h \
    MyObject/event.h \
    MyObject/exceptioncontainer.h \
    MyObject/onlineconfig.h \
    MyObject/pushid.h \
    MyObject/tag.h \
    MyObject/updatepreference.h \
    MyObject/updateret.h \
    MyObject/userid.h \
    MyObject/userrepolicy.h \
    UMS/umsmanager.h \
    UMSAgent_global.h \
    Common/constants.h \
    Common/urlencode.h \
    MyObject/allinfo.h \
    MyObject/clientdata.h \
    MyObject/commonret.h \
    MyObject/pageinfo.h \
    umsagent.h


win32 {
    INCLUDEPATH += $$PWD/ThirdParty/win32/infoware/include
    INCLUDEPATH += $$PWD/ThirdParty/win32/jsoncons/include
    INCLUDEPATH += $$PWD/ThirdParty/win32/curl/include
    INCLUDEPATH += $$PWD/ThirdParty/win32/restclient-cpp/include
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/ThirdParty/win32/infoware/lib infowared.lib
        LIBS += -L$$PWD/ThirdParty/win32/jsoncpp/lib jsoncpp.lib
        LIBS += -L$$PWD/ThirdParty/win32/curl/lib libcurl.lib
        LIBS += -L$$PWD/ThirdParty/win32/restclient-cpp/lib restclient-cpp.lib
    } else {
        LIBS += -L$$PWD/ThirdParty/win32/infoware/lib infoware.lib
        LIBS += -L$$PWD/ThirdParty/win32/jsoncpp/lib jsoncpp.lib
        LIBS += -L$$PWD/ThirdParty/win32/curl/lib libcurl.lib
        LIBS += -L$$PWD/ThirdParty/win32/restclient-cpp/lib restclient-cpp.lib
    }

    LIBS += -lgdi32 -lversion -lOle32 -lOleAut32 -lwbemuuid -lkernel32 -luser32 -lwinspool -lshell32 -lole32 -luuid -lcomdlg32 -ladvapi32
}
