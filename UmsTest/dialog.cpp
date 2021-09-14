#include "dialog.h"
#include "ui_dialog.h"
#include <umsapi.h>

const static std::string kAppKey = "48d81c9fc76f11b11e15e4e70f60e8b5";
const static std::string kUmsURL = "https://japm.sd7dd.com/a";
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    if (!UMSApi::hasBindBaseUrl())
        UMSApi::bindBaseUrl(kUmsURL);
    UMSApi::onAppStart(kAppKey);
    UMSApi::bindUserIdentifier("kid");
    UMSApi::bindApplicationLanguage("CN");
    UMSApi::bindApplicationVersion("1.0.0");
//    UMSApi::postPushid("Desktop test push");
    UMSApi::postClientdata();
//    UMSApi::updateOnlineConfig();
    UMSApi::onEvent("start", "Dialog");
    UMSApi::onPageBegin("pageTest1");
    QString dumpDir = QApplication::applicationDirPath() + "/minidump";
    UMSApi::postCrashData(dumpDir.toStdString());
    UMSApi::onEvent("Dialog", "", "show");
    UMSApi::onPageEnd("pageTest1");

}

Dialog::~Dialog()
{
    delete ui;
}

