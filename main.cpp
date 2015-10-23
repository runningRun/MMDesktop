//#include "logindialog.h"
//#include "mainwidget.h"
#include <QApplication>
#include "mmdesktopapp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Dialog loginDialog;
//    loginDialog.show();
//    MainWidget mainwidget;
//    mainwidget.show();
    MMDesktopApp mmDesktopApp;


    QApplication::setQuitOnLastWindowClosed(false);
    return a.exec();
}
