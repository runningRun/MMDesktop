#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "mmdbasewidget.h"

struct UserInfo;
class QWidget;
class QTreeWidgetItem;
class QQuickWidget;
class ChatListWidget;
class GlobalXAgentOCX;
class SessionHandle;
class QMultiMap<QString, SessionHandle*>;
class QMultiMap<QString, QTreeWidgetItem*>;
class QMultiMap<QTreeWidgetItem*, QQuickWidget*>;
class QMultiMap<QString, UserInfo*>;


namespace Ui {
class MainWidget;
}

class MainWidget : public MMDBaseWidget
{
    Q_OBJECT
public:
    explicit MainWidget(MMDBaseWidget *parent = 0);
    ~MainWidget();
    void receiveSubjectNotify(EventID eventID, QString eventInfo);
protected:
    void closeEvent(QCloseEvent *);
private:
    void enableAgentButtons(AGENT_STATUS agentStatus);
    void sessionStatusChangedHandle(SESSION_STATUS sessionStatus, QString sessionid, QString userData);
    void sessionEnd(QString sessionid);
    void parseUserInfo(QString sessionid, QString userData);
    void chatMessageHandle(QString sessionid, QString userDate);

    void onEventAgentStatusChanged(QString eventInfo);
    void onEventSessionStatusChanged(QString eventInfo);
    void onEventChatMessage(QString eventInfo);
    void onEventCallStatusChanged(QString eventInfo);
signals:
    void sessionStatusRinging(QString sessionid, QString userData);
private slots:
    void ChatListWidgetAddedNewTreeWidgetItem(QTreeWidgetItem* item, QString sessionid);
    void PushButtonLogin_outClicked();
    void PushButtonReay_NOTClicked();
private:
    Ui::MainWidget *ui;
    AGENT_STATUS m_agentState;
    GlobalXAgentOCX *m_pAgentOCX;
    ChatListWidget *m_pChatListWidget;      //会话列表窗口
    QQuickWidget* m_pCurrentShowQuickWidget;
    QMap<QString, SessionHandle*> *m_pMapSessionid_SessionHandle;
    QMap<QString, QTreeWidgetItem*> *m_pMapSessionid_TWI;   //TWI--TreeWidgetItem
    QMap<QTreeWidgetItem*, QQuickWidget*> *m_pMapTWI_QuickWidget;
    QMap<QString, UserInfo*> *m_pMapSession_UserInfo;
};

#endif // MAINWIDGET_H
