#ifndef CSTINFOWIDGET_H
#define CSTINFOWIDGET_H

#include <QWidget>
#include "common.h"
class QLabel;
class QGroupBox;

class CstInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CstInfoWidget(QWidget *parent = 0);
    ~CstInfoWidget();

    void loadUserInfo(UserID userid);

signals:

public slots:

private:
    QGroupBox* m_pCstInfoGroupBox;  //客户信息
    QGroupBox* m_pHistoryPug;       //历史足迹

    QLabel* m_pInfoLabel;
    QLabel* m_pHistoryPugLabel;

};

#endif // CSTINFOWIDGET_H
