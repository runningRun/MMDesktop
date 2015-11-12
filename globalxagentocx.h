#ifndef GLOBALXAGENTOCX_H
#define GLOBALXAGENTOCX_H
#include <QString>
#include <QList>
#include <QWidget>
#include "globalvar.h"
#include "mmdbasewidget.h"

class QAxWidget;
class GlobalXAgentOCX : public QObject
{
    Q_OBJECT
public:
    GlobalXAgentOCX();
    ~GlobalXAgentOCX();

    void setAgentInfo(QString agentInfo);
    void startUp();
    /*
     * Agent状态管理
     */
    void login();
    void logout();
    void ready(QString info);
    void notReady(QString info);
    /*
     * 会话消息管理
     */
    void chatSend(QString sessionid, QString info);
    void chatAnswer(QString sessionid);
    void chatRelease(QString sessionid);
    void chatHold(QString sessionid);
    void chatRetrieve(QString sessionid);
    /*
     * 语音通话管理
     */
    void answer(QString sessionid);
    void release(QString sessionid);
    void hold(QString sessionid);
    void retrieve(QString sessionid);
    /*
     * 多媒体文件上传、下载
     */
    QString uploadFile(QString info);
    QString downloadFile(QString info);
    void addObserver(MMDBaseWidget* newObserver);

public:
    static GlobalXAgentOCX* instance();
    static QAxWidget *globalXAgentOCX;
private:
    static GlobalXAgentOCX* agentOCX;
    QList<MMDBaseWidget*> m_pListObserverMMDBaseWidget;

private slots:
    void onEventAgentStatusChanged(QString status, QString reason);
    void onEventSessionStatusChanged(QString SessionID, QString CurrStatus, QString UserData);
    void onEventChatMessage(QString SessionID, QString Content);
    void onEventCallStatusChanged(QString CallID, QString Status, QString UserData);

private :
    void notifyObserver(EventID eventid, QString eventInfo);

};

#endif // GLOBALXAGENTOCX_H
