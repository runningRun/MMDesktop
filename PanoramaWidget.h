#ifndef PANORAMAWIDGET_H
#define PANORAMAWIDGET_H

#include <QWidget>
#include <QList>
#include <QSignalMapper>
#include "common.h"



class MyButton;
class QVBoxLayout;
class QHBoxLayout;
class CustomInfoWidget;
class CRMWidget;
class KnowledgeBaseWidget;

const int BUTTONWIDTH = 30;
const int BUTTONHEIGHT = 100;

class PanoramaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PanoramaWidget(QWidget *parent = 0);
    ~PanoramaWidget();

    void showUserInfo(UserID userid);

protected:
    void initComponet();
    void paintEvent(QPaintEvent*);

signals:

public slots:

private slots:
    void switchWidget(int index);               //切换显示窗口
    void switchUserInfo(UserID userid);         //切换显示的用户信息

private:
    QList<MyButton* > m_listMyButton;           //存放功能列表
    QSignalMapper m_SigalMapper;                //信号参数

    QVBoxLayout* m_pBtnVLayout;                 //功能按钮布局
    QHBoxLayout* m_pMainHLayout;                //控制总体布局
    QWidget* m_pCurrentWidget;                  //指向当前显示的窗口

    CustomInfoWidget* m_pCstInfoWidget;
    CRMWidget* m_pCRMWidget;
    KnowledgeBaseWidget* m_pKnowledgeWidget;

};

#endif // PANORAMAWIDGET_H
