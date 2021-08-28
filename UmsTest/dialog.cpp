#include "dialog.h"
#include "ui_dialog.h"
#include <umsapi.h>

const static std::string kAppKey = "ec9af1ac7f762fd36fb6dd03a297bd24";
const static std::string kUmsURL = "http://192.168.3.92:8986/api/a";
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    UMSApi::onAppStart(kAppKey, kUmsURL);
    UMSApi::bindUserIdentifier("DESKTOP-CJE7V2F");
//    UMSApi::postPushid("Desktop test push");
    UMSApi::postClientdata();
//    UMSApi::updateOnlineConfig();
    UMSApi::onEvent("start", "Dialog");
    QString dumpDir = QApplication::applicationDirPath() + "/minidump";
    UMSApi::postCrashData(dumpDir.toStdString());

}

Dialog::~Dialog()
{
    delete ui;
}

