#ifndef CUSTOMINFOWIDGET_H
#define CUSTOMINFOWIDGET_H

#include <QWidget>
#include <QList>
#include <QWidgetList>
#include <QStackedWidget>
#include <QSignalMapper>
#include <QBoxLayout>

class MyButton;
class HistoryWidget;
class CstInfoWidget;

class CustomInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomInfoWidget(QWidget *parent = 0);
    ~CustomInfoWidget();

private:
    void initComponet();


signals:

public slots:

private slots:
    void switchPage(int index);         //在客户资料选项卡和历史记录选项卡间切换

private:
    QList<MyButton* > m_listMyButton;
    QSignalMapper m_signalMapper;

    HistoryWidget* m_pHistoryWidget;
    CstInfoWidget* m_pCstInfoWidget;
    QWidget* m_pCurrentShowWidget;

    QVBoxLayout* m_pMainVLayout;
};

#endif // CUSTOMINFOWIDGET_H
