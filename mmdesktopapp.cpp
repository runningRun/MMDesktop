#include "mmdesktopapp.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QDateTime>
#include "globalxagentocx.h"
#include "logindialog.h"
#include "mainwidget.h"
#include "agentinfo.h"

MMDesktopApp::MMDesktopApp()
{
    this->m_pLoginDlg = new Dialog();
    this->m_pSysTrayIcon = new QSystemTrayIcon(this);

    createSysTrayMenu();
    initSysTrayIcon();
    connect(this->m_pLoginDlg, SIGNAL(btnLoginClicked()),
            this, SLOT(loginDlgLogged()));
    connect(this->m_pSysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(sysTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(this->m_pSysTrayIcon, SIGNAL(messageClicked()),
            this, SLOT(messageClicked()));
    connect(this->m_pActionQuit, SIGNAL(triggered()),
            this, SLOT(actionQuitClicked()));

    this->m_pLoginDlg->show();
    this->m_pSysTrayIcon->show();
}

void MMDesktopApp::initSysTrayIcon()
{
    this->m_pSysTrayIcon->setIcon(QIcon("res/mmdesktop.png"));
    this->m_pSysTrayIcon->setContextMenu(m_pSysTrayMenu);
    this->m_pSysTrayIcon->setToolTip("多媒体工作台正在运行···");
}

void MMDesktopApp::showMessage(QString sessionid)
{
    this->m_sessionid = sessionid;
    this->m_pSysTrayIcon->showMessage("您有新的服务请求", "sessionid: "+sessionid+"请求您为其服务",
                                      QSystemTrayIcon::Information, 30000);
}

void MMDesktopApp::createSysTrayMenu()
{
    m_pActionQuit = new QAction(tr("退出"), this);
    m_pActionQuit->setIcon(QIcon("res/exit.png"));
    m_pSysTrayMenu = new QMenu();
    m_pSysTrayMenu->addSeparator();
    m_pSysTrayMenu->addAction(m_pActionQuit);
}

void MMDesktopApp::loginDlgLogged()
{
    this->m_pMainWidget = new MainWidget();
    this->m_pMainWidget->setWindowIcon(QIcon("res/mmdesktop.png"));
    connect(this->m_pMainWidget, SIGNAL(sessionStatusRinging(QString,QString)),
            this, SLOT(sessionStatusRingingHandle(QString,QString)));
    this->m_pLoginDlg->close();
    this->m_pMainWidget->show();
}

void MMDesktopApp::actionQuitClicked()
{
    exit(0);
}

void MMDesktopApp::messageClicked()
{
    GlobalXAgentOCX::instance()->chatAnswer(this->m_sessionid);
    QString strDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString Content = "";
    Content += "<xml>";
    Content += "<from>" + AgentInfo::agentThisDN + "</from>";
    Content += "<to>" + m_from + "</to>";
    Content += "<via>" + m_via + "</via>";
    Content += "<time>" + strDate + "</time>";
    Content += "<msgtype>01</msgtype>";
    Content += "<isAgent>true<isAgent>";
    Content += "<title></title>";
    Content += "<content>您好，2005号坐席为您服务，请问您有什么需要帮助的吗？</content>";
    Content += "<url></url>";
    Content += "</xml>";
    GlobalXAgentOCX::instance()->chatSend(this->m_sessionid, Content);
}

void MMDesktopApp::sysTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        this->m_pMainWidget->showNormal();
        break;
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    case QSystemTrayIcon::Context:
        break;
    default:
        break;
    }
}

void MMDesktopApp::sessionStatusRingingHandle(QString sessionid, QString userData)
{
    QString startTag = "<from>";
    QString endTag = "</from>";
    int startIndex = userData.indexOf(startTag) + startTag.length();
    int endIndex = userData.indexOf(endTag);
    m_from = userData.mid(startIndex, endIndex - startIndex);

    startTag = "<to>";
    endTag = "</to>";
    startIndex = userData.indexOf(startTag) + startTag.length();
    endIndex = userData.indexOf(endTag);
    m_to = userData.mid(startIndex, endIndex - startIndex);

    startTag = "<via>";
    endTag = "</via>";
    startIndex = userData.indexOf(startTag) + startTag.length();
    endIndex = userData.indexOf(endTag);
    m_via = userData.mid(startIndex, endIndex - startIndex);
    m_sessionid = sessionid;
    showMessage(sessionid);
    qDebug("m_from:%s\n, m_to:%s\n, m_via:%s\n", m_from.toStdString().c_str(), m_to.toStdString().c_str(), m_via.toStdString().c_str());
}

MMDesktopApp::~MMDesktopApp()
{
    delete m_pLoginDlg;
    delete m_pMainWidget;
    delete m_pSysTrayIcon;
}

