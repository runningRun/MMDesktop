#ifndef MMDESKTOPAPP_H
#define MMDESKTOPAPP_H

#include <QObject>
#include <QSystemTrayIcon>


class Dialog;
class MainWidget;
class QAction;
class QMenu;

class MMDesktopApp : public QObject
{
    Q_OBJECT
public:
    MMDesktopApp();
    ~MMDesktopApp();
private:
    void initSysTrayIcon();
    void showMessage(QString sessionid);
    void createSysTrayMenu();
private slots:
    void loginDlgLogged();      //点击完登录界面的“登录”按钮后，执行的动作
    void actionQuitClicked();
    void sysTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void messageClicked();
    void sessionStatusRingingHandle(QString sessionid, QString userData);

private:
    Dialog *m_pLoginDlg;
    MainWidget *m_pMainWidget;
    QSystemTrayIcon *m_pSysTrayIcon;
    QMenu *m_pSysTrayMenu;
    QAction *m_pActionQuit;
    QString m_sessionid;
    QString m_from;
    QString m_via;
    QString m_to;
};

#endif // MMDESKTOPAPP_H
