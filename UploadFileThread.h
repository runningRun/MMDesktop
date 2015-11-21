#ifndef UPLOADFILETHREAD_H
#define UPLOADFILETHREAD_H

#include <QThread>

class UploadFileThread : public QThread
{
    Q_OBJECT
public:
    UploadFileThread();

    void setParameter(QString sessionid, int fileTypem, QString filePath);
    QString result();

protected:
    void run();

signals:
    void uploadFinished(QString sessionid, int fileType, QString result);

private:
    QString m_filePath;
    QString m_sessionid;
    int m_fileType;
    QString m_result;
};

#endif // UPLOADFILETHREAD_H
