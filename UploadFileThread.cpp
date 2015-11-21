#include "UploadFileThread.h"
#include "globalxagentocx.h"

UploadFileThread::UploadFileThread()
{

}

void UploadFileThread::setParameter(QString sessionid, int fileType, QString filePath)
{
    this->m_sessionid = sessionid;
    this->m_fileType = fileType;
    this->m_filePath = filePath;
}

QString UploadFileThread::result()
{
    return this->m_result;
}

void UploadFileThread::run()
{
    GlobalXAgentOCX *agentOCX = GlobalXAgentOCX::instance();
    this->m_result = agentOCX->uploadFile(this->m_filePath);
    emit uploadFinished(this->m_sessionid, this->m_fileType,this->m_result);
}
