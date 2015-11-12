#ifndef CSTINFOWIDGET_H
#define CSTINFOWIDGET_H

#include <QWidget>
class QGroupBox;

class CstInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CstInfoWidget(QWidget *parent = 0);
    ~CstInfoWidget();

signals:

public slots:

private:
    QGroupBox* m_pCstInfoGroupBox;  //客户信息
    QGroupBox* m_pHistoryPug;       //历史足迹

};

#endif // CSTINFOWIDGET_H
