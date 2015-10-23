#ifndef SESSIONHANDLE_H
#define SESSIONHANDLE_H

#include <QObject>
#include "globalvar.h"
#include "globalxagentocx.h"

class SessionHandle : public QObject
{
    Q_OBJECT

public:
    SessionHandle(UserInfo userInfo, QObject* parent = 0);
    void addNewItemFunc(QString newItemContent);
    ~SessionHandle();

    Q_INVOKABLE void sendInfo(QString sessionid, int msgtype, QString content);
    Q_INVOKABLE void finishSession(QString sessionid);
signals:
    void addNewItem(QString senderName, int newItemMsgtype, int newItemDirection, QString newItemContent);
private:        //method
    void sendText(QString sessionid, QString text);
    void sendPicture(QString sessionid, QString picturePath);
    void sendVoice(QString sessionid, QString voicePath);
    void sendVideo(QString sessionid, QString videoPath);
    QString encapsulate2XML(int msgtype, QString content);
private:
    UserInfo m_userInfo;
    GlobalXAgentOCX* m_pXAgentOCX;
};

#endif // SESSIONHANDLE_H
