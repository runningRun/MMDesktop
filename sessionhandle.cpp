#include "sessionhandle.h"
#include "agentinfo.h"
#include <QDateTime>
#include <QThread>

SessionHandle::SessionHandle(UserInfo userInfo, QObject *parent) :
    QObject(parent),
    m_userInfo(userInfo)
{
    this->m_pXAgentOCX = GlobalXAgentOCX::instance();

    connect(&m_uploadFileThread, SIGNAL(uploadFinished(QString,int,QString)),
            this, SLOT(uploadFileFinished(QString,int,QString)));
    connect(&m_downloadFileThread, SIGNAL(downloadFileFinished(int,QString)),
            this, SLOT(downloadFileFinished(int,QString)));
}

void SessionHandle::sendInfo(QString sessionid, int msgtype, QString content)
{
    MSGTYPE filetype;

    switch (msgtype) {
    case 0:                 //文本消息
        sendText(sessionid, content);
        break;
    case 1:
        filetype = PICTURE;
        break;
    case 2:
        filetype = VOICE;
        break;
    case 3:
        filetype = VIDEO;
        break;
        /*
    case 1:                 //图片
        sendPicture(sessionid, content);
        break;
    case 2:                 //声音
        sendVoice(sessionid, content);
        break;
    case 3:                 //视频
        sendVideo(sessionid, content);
        break;
        */
    default:
        break;
    }

    if (0 != msgtype)
    {
        while (m_uploadFileThread.isRunning())
            ;
        m_uploadFileThread.setParameter(sessionid, filetype, content);
        m_uploadFileThread.start();
    }

}

void SessionHandle::sendText(QString sessionid, QString text)
{
    this->m_pXAgentOCX->chatSend(sessionid, encapsulate2XML(TEXT, text));
}

void SessionHandle::sendPicture(QString sessionid, QString picturePath)
{
    QString serverFileName = this->m_pXAgentOCX->uploadFile(picturePath);
    this->m_pXAgentOCX->chatSend(sessionid, encapsulate2XML(PICTURE, serverFileName));
}

void SessionHandle::sendVoice(QString sessionid, QString voicePath)
{
    QString serverFileName = this->m_pXAgentOCX->uploadFile(voicePath);
    this->m_pXAgentOCX->chatSend(sessionid, encapsulate2XML(VOICE, serverFileName));
}

void SessionHandle::sendVideo(QString sessionid, QString videoPath)
{
    QString serverFileName = this->m_pXAgentOCX->uploadFile(videoPath);
    this->m_pXAgentOCX->chatSend(sessionid, encapsulate2XML(VIDEO, serverFileName));
}

void SessionHandle::sendFile(QString sessionid, int fileType, QString serverFileName)
{
    this->m_pXAgentOCX->chatSend(sessionid, encapsulate2XML(fileType, serverFileName));
}

QString SessionHandle::encapsulate2XML(int msgtype, QString content)
{
    QString strMsgtype;
    QString text = "";
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString title = "";
    QString url = "";
    switch (msgtype)
    {
    case TEXT:
        title = "文本";
        url = "";
        strMsgtype = "01";
        break;
    case PICTURE:
        title = "图片";
        url = content;
        strMsgtype = "04";
        break;
    case VOICE:
        title = "语音";
        url = content;
        strMsgtype = "02";
        break;
    case VIDEO:
        title = "视频";
        url = content;
        strMsgtype = "03";
        break;
    default:
        break;
    }
    text += "<xml>";
    text += "<isAgent>true</isAgent>";
    text += "<group>1</group>";
    text += "<from>" + AgentInfo::agentThisDN + "</from>";
    text += "<to>" + this->m_userInfo.thisDN + "</to>";
    text += "<via>" + this->m_userInfo.via + "</via>";
    text += "<time>" + time + "</time>";
    text += "<msgtype>" + strMsgtype + "</msgtype>";
    text += "<title>" + title + "</title>";
    text += "<content>" + content + "</content>";
    text += "<url>" + url + "</url>";
    text += "</xml>";

    return text;
}

void SessionHandle::uploadFileFinished(QString sessionid, int fileType, QString result)
{
    sendFile(sessionid, fileType, result);
}

void SessionHandle::downloadFileFinished(int fileType, QString filePath)
{
    QString tmpFilePath = filePath.replace('\\', "/");
    tmpFilePath = "file:///" + tmpFilePath;
    emit addNewItem(this->m_userInfo.thisDN, fileType, 1, tmpFilePath);
}

void SessionHandle::addNewItemFunc(QString newItemContent)
{
    qDebug("SessionHandle::addNewItemFunc--newItemContent:%s", newItemContent.toStdString().c_str());
    /*
     * mesgtype
     * 01 -- text
     * 02 -- voice
     * 03 -- video
     * 04 -- picture
     */
    QString startTag = "<msgtype>";
    QString endTag = "</msgtype>";
    QString msgtype = "";
    QString msg = "";
    QString fileName = "";
    int imsgtype = -1;
    int startIndex = newItemContent.indexOf(startTag) + startTag.length();
    int endIndex = newItemContent.indexOf(endTag);
    msgtype = newItemContent.mid(startIndex, endIndex - startIndex);

    if (0 == msgtype.compare("01"))
    {
        imsgtype = 0;
    }
    else if (0 == msgtype.compare("02"))
    {
        imsgtype = 2;
    }
    else if (0 == msgtype.compare("03"))
    {
        imsgtype = 3;
    }
    else if (0 == msgtype.compare("04"))
    {
        imsgtype = 1;
    }

    switch (imsgtype) {
    case 0:
        startTag = "<content>";
        endTag = "</content>";
        startIndex = newItemContent.indexOf(startTag) + startTag.length();
        endIndex = newItemContent.indexOf(endTag);
        msg = newItemContent.mid(startIndex, endIndex - startIndex);

        emit addNewItem(this->m_userInfo.thisDN, imsgtype, 1, msg);

        break;
    case 1:
    case 2:
    case 3:						
    	/*
		 * zwl
		 *2015.11.16
		 *完善加载来自客户的图片、语音、视频功能
    	 */
        startTag = "<url>";
        endTag = "</url>";
        startIndex = newItemContent.indexOf(startTag) + startTag.length();
        endIndex = newItemContent.indexOf(endTag);
        msg = newItemContent.mid(startIndex, endIndex - startIndex);
        startIndex = msg.indexOf("/", -1);
        fileName = msg.mid(startIndex, -1);

        m_downloadFileThread.setParameter(imsgtype, fileName);
        m_downloadFileThread.start();
//        msg = GlobalXAgentOCX::instance()->downloadFile(fileName);
//        msg.replace('\\', "/");
//        msg = "file:///" + msg;
        break;
    default:
        break;
    }

    qDebug("SessionHandle::addNewItemHandle--msg:%s", msg.toStdString().c_str());
}

void SessionHandle::finishSession(QString sessionid)
{
    GlobalXAgentOCX::instance()->chatRelease(sessionid);
}

SessionHandle::~SessionHandle()
{

}

