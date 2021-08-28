QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    UmsTest/main.cpp \
    UmsTest/dialog.cpp

HEADERS += \
    UmsTest/dialog.h

FORMS += \
    UmsTest/dialog.ui

DESTDIR = $$PWD/bin
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
    LIBS += -L$$DESTDIR -lUMSAgentd
} else {
    LIBS += -L$$DESTDIR -lUMSAgent
}

mac {
    QMAKE_POST_LINK += $$quote($$[QT_INSTALL_BINS]/macdeployqt \"$${DESTDIR}/$${TARGET}.app\" -libpath=\"$$DESTDIR\"$$escape_expand(\\n\\t))
}

INCLUDEPATH += $$PWD

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
