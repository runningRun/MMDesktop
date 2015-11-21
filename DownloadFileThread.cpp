#include "DownloadFileThread.h"
#include "globalxagentocx.h"

DownloadFileThread::DownloadFileThread()
{

}

void DownloadFileThread::setParameter(int fileType, QString serverFileName)
{
    this->m_fileType = fileType;
    this->m_serverFileName = serverFileName;
}

void DownloadFileThread::run()
{
    GlobalXAgentOCX *agentOCX = GlobalXAgentOCX::instance();
    this->m_result = agentOCX->downloadFile(m_serverFileName);
    this->msleep(1000);
    emit downloadFileFinished(m_fileType, m_result);
}

