#ifndef MMDBASEWIDGET_H
#define MMDBASEWIDGET_H

#include <QWidget>
#include "globalvar.h"
class MMDBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MMDBaseWidget(QWidget *parent = 0);
    virtual void receiveSubjectNotify(EventID eventID, QString eventInfo)
    {
        printf("MMDBaseWidget receiveSubjectNotify eventID:%d\t eventInfo:%s\n", eventID, eventInfo.toStdString().c_str());
    }
    ~MMDBaseWidget();

signals:

public slots:
};

#endif // MMDBASEWIDGET_H
