#ifndef SESSIONHANDLE_H
#define SESSIONHANDLE_H

#include <QObject>
#include "globalvar.h"
#include "globalxagentocx.h"
#include "UploadFileThread.h"
#include "DownloadFileThread.h"

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
private slots:
    void uploadFileFinished(QString sessionid, int fileType, QString result);
    void downloadFileFinished(int fileType, QString filePath);
private:        //method
    void sendText(QString sessionid, QString text);
    void sendPicture(QString sessionid, QString picturePath);
    void sendVoice(QString sessionid, QString voicePath);
    void sendVideo(QString sessionid, QString videoPath);
    void sendFile(QString sessionid, int fileType, QString serverFileName);
    QString encapsulate2XML(int msgtype, QString content);
private:
    UserInfo m_userInfo;
    UploadFileThread m_uploadFileThread;
    DownloadFileThread m_downloadFileThread;
    GlobalXAgentOCX* m_pXAgentOCX;
};

#endif // SESSIONHANDLE_H
