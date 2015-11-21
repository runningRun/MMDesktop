#ifndef DOWNLOADFILETHREAD_H
#define DOWNLOADFILETHREAD_H

#include <QThread>

class DownloadFileThread : public QThread
{
    Q_OBJECT
public:
    DownloadFileThread();
    void setParameter(int fileType, QString serverFileName);

protected:
    void run();

signals:
    void downloadFileFinished(int fileType, QString result);
private:
    int m_fileType;
    QString m_serverFileName;
    QString m_result;
};

#endif // DOWNLOADFILETHREAD_H
