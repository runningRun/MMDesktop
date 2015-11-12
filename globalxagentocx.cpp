#include "globalxagentocx.h"
#include <ActiveQt/QAxWidget>

GlobalXAgentOCX::GlobalXAgentOCX()
{
    connect(GlobalXAgentOCX::globalXAgentOCX, SIGNAL(EventAgentStatusChanged(QString, QString)), this, SLOT(onEventAgentStatusChanged(QString,QString)));
    connect(GlobalXAgentOCX::globalXAgentOCX, SIGNAL(EventSessionStatusChanged(QString, QString,QString)), this, SLOT(onEventSessionStatusChanged(QString,QString,QString)));
    connect(GlobalXAgentOCX::globalXAgentOCX, SIGNAL(EventChatMessage(QString, QString)), this, SLOT(onEventChatMessage(QString,QString)));
    connect(GlobalXAgentOCX::globalXAgentOCX, SIGNAL(EventCallStatusChanged(QString, QString, QString)), this, SLOT(onEventCallStatusChanged(QString,QString,QString)));
}

QAxWidget* GlobalXAgentOCX::globalXAgentOCX = NULL;
GlobalXAgentOCX* GlobalXAgentOCX::agentOCX = NULL;

void GlobalXAgentOCX::setAgentInfo(QString agentInfo)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("SetAgentInfo(LPCTSTR info)", agentInfo);
}

void GlobalXAgentOCX::startUp()
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("Startup()");
}

void GlobalXAgentOCX::login()
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("Login()");
}

void GlobalXAgentOCX::logout()
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("Logout()");
}

void GlobalXAgentOCX::ready(QString info)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("Ready(LPCTSTR info)", info);
}

void GlobalXAgentOCX::notReady(QString info)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("NotReady(LPCTSTR info)", info);
}

void GlobalXAgentOCX::chatSend(QString sessionid, QString info)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("ChatSend(LPCTSTR SessionID, LPCTSTR info)", sessionid, info);
}

void GlobalXAgentOCX::chatAnswer(QString sessionid)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("ChatAnswer(LPCTSTR SessionID)", sessionid);
}

void GlobalXAgentOCX::chatRelease(QString sessionid)
{
   GlobalXAgentOCX::globalXAgentOCX->dynamicCall("ChatRelease(LPCTSTR SessionID)", sessionid);
}

void GlobalXAgentOCX::chatHold(QString sessionid)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("ChatHold(LPCTSTR SessionID)", sessionid);
}

void GlobalXAgentOCX::chatRetrieve(QString sessionid)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("ChatRetrieve(LPCTSTR SessionID)", sessionid);
}

void GlobalXAgentOCX::answer(QString sessionid)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("Answer(LPCTSTR sessionid)", sessionid);
}
void GlobalXAgentOCX::release(QString sessionid)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("Release(LPCTSTR sessionid)", sessionid);
}
void GlobalXAgentOCX::hold(QString sessionid)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("Hold(LPCTSTR sessionid)", sessionid);
}

void GlobalXAgentOCX::retrieve(QString sessionid)
{
    GlobalXAgentOCX::globalXAgentOCX->dynamicCall("Retrieve(LPCTSTR sessionid)", sessionid);
}

QString GlobalXAgentOCX::uploadFile(QString info)
{
     QString fileName = (GlobalXAgentOCX::globalXAgentOCX->dynamicCall("UploadFile(LPCTSTR info)", info)).toString();
     if (NULL != fileName)
         return fileName;
     else
         return "";
}

QString GlobalXAgentOCX::downloadFile(QString info)
{
    QString fileName = (GlobalXAgentOCX::globalXAgentOCX->dynamicCall("DownloadFile(LPCTSTR info)", info)).toString();
    if (NULL != fileName)
        return fileName;
    else
        return "";
}

void GlobalXAgentOCX::addObserver(MMDBaseWidget *newObserver)
{
    this->m_pListObserverMMDBaseWidget.append(newObserver);
}

GlobalXAgentOCX* GlobalXAgentOCX::instance()
{
     if (NULL == agentOCX)
     {
         agentOCX = new GlobalXAgentOCX();
     }
     return agentOCX;
}

void GlobalXAgentOCX::onEventSessionStatusChanged(QString SessionID, QString CurrStatus, QString UserData)
{
    QString eventInfo = "";
    eventInfo += (SessionID + "&");
    eventInfo += (CurrStatus + "&");
    eventInfo += UserData;
    notifyObserver(EventSessionStatusChanged, eventInfo);
}

void GlobalXAgentOCX::onEventCallStatusChanged(QString CallID, QString Status, QString UserData)
{
    QString eventInfo = "";
    eventInfo += (CallID + "&");
    eventInfo += (Status + "&");
    eventInfo += (UserData);
    notifyObserver(EventCallStatusChanged, eventInfo);
}

void GlobalXAgentOCX::onEventChatMessage(QString SessionID, QString Content)
{
    QString eventInfo = "";
    eventInfo += (SessionID + "&");
    eventInfo += Content;
    notifyObserver(EventChatMessage, eventInfo);
}

void GlobalXAgentOCX::onEventAgentStatusChanged(QString status, QString reason)
{
    QString eventInfo = "";
    eventInfo += (status + "&");
    eventInfo += reason;
    notifyObserver(EventAgentStattusChanged, eventInfo);
}

void GlobalXAgentOCX::notifyObserver(EventID eventid, QString eventInfo)
{
    MMDBaseWidget* item;
    foreach (item, m_pListObserverMMDBaseWidget) {
        item->receiveSubjectNotify(eventid, eventInfo);
    }
}

GlobalXAgentOCX::~GlobalXAgentOCX()
{

}

