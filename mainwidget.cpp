#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "globalvar.h"
#include "globalxagentocx.h"
#include <QPalette>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QIcon>
#include <QMessageBox>
#include <QQuickWidget>
#include <QQmlContext>
#include "chatlistwidget.h"
#include "sessionhandle.h"
#include <QQmlEngine>
#include <QQuickItem>
#include <QListView>
#include <QVariantMap>
#include <QAbstractItemModel>
#include "agentinfo.h"

MainWidget::MainWidget(MMDBaseWidget *parent) :
    MMDBaseWidget(parent),
    ui(new Ui::MainWidget)
{    
    ui->setupUi(this);

    this->ui->lineEditAgentID->setText(AgentInfo::agentID);
    this->ui->lineEditAgentName->setText(AgentInfo::agentName);
    this->ui->lineEditAgentNameGroupID->setText(AgentInfo::agentGroupID);
    this->ui->lineEditAgentThisDN->setText(AgentInfo::agentThisDN);

    this->m_pChatListWidget = new ChatListWidget(this->ui->widgetChatList);
    this->m_pMapSessionid_TWI = new QMap<QString, QTreeWidgetItem*>();
    this->m_pMapTWI_QuickWidget = new QMap<QTreeWidgetItem*, QQuickWidget*>();
    this->m_pMapSession_UserInfo = new QMap<QString, UserInfo*>();
    this->m_pMapSessionid_SessionHandle = new QMap<QString, SessionHandle*>();
    this->m_pCurrentShowQuickWidget = NULL;
    this->m_agentState = STATE_AGENT_LOGOUT;
    enableAgentButtons(this->m_agentState);

    connect(this->m_pChatListWidget, SIGNAL(createNewTreeWidgetItem(QTreeWidgetItem*, QString)),
                     this, SLOT(ChatListWidgetAddedNewTreeWidgetItem(QTreeWidgetItem*, QString)));
    connect(this->ui->pushButtonLogin_out, SIGNAL(clicked()),
            this, SLOT(PushButtonLogin_outClicked()));
    connect(this->ui->pushButtonReady_NOT, SIGNAL(clicked()),
            this, SLOT(PushButtonReay_NOTClicked()));
    this->m_pChatListWidget->Load();

    this->m_pAgentOCX = GlobalXAgentOCX::instance();
    this->m_pAgentOCX->addObserver(this);
}


void MainWidget::ChatListWidgetAddedNewTreeWidgetItem(QTreeWidgetItem *item, QString sessionid)
{
    QQuickWidget* quickWidget;
    UserInfo *userInfo = this->m_pMapSession_UserInfo->value(sessionid);
    SessionHandle* sessionHandle = new SessionHandle(*userInfo);
    quickWidget = new QQuickWidget(this->ui->quickWidget);
    quickWidget->setGeometry(QRect(0, 0, 480, 560));
    quickWidget->rootContext()->setContextProperty("sessionID", sessionid);
    quickWidget->rootContext()->setContextProperty("sessionHandle", sessionHandle);
    quickWidget->rootContext()->setContextProperty("agentID", AgentInfo::agentThisDN);
    quickWidget->setSource(QUrl::fromLocalFile("qml/myChatView.qml"));
    quickWidget->hide();
    this->m_pMapSessionid_TWI->insert(sessionid, item);
    this->m_pMapTWI_QuickWidget->insert(item, quickWidget);
    this->m_pChatListWidget->addNewQuickWidget(item, quickWidget);
    this->m_pMapSessionid_SessionHandle->insert(sessionid, sessionHandle);
}

void MainWidget::closeEvent(QCloseEvent *)
{
    hide();
}

void MainWidget::PushButtonLogin_outClicked()
{
    switch (this->m_agentState)
    {
    case STATE_AGENT_LOGOUT:
        this->m_pAgentOCX->login();
        break;
    case STATE_AGENT_READY:
    case STATE_AGENT_NOTREADY:
    case STATE_AGENT_NOTREADY_FOR_NEXTCALL:
        this->m_pAgentOCX->logout();
        break;
    default:
        QMessageBox msgBox;
        msgBox.setText("目前状态不能签出，当前状态为： " + this->m_agentState);
        break;
    }
}

void MainWidget::PushButtonReay_NOTClicked()
{
    switch (this->m_agentState)
    {
    case STATE_AGENT_READY:
        this->m_pAgentOCX->notReady("");
        break;
    case STATE_AGENT_NOTREADY:
        this->m_pAgentOCX->ready("");
    default:
        break;
    }
}

void MainWidget::receiveSubjectNotify(EventID eventID, QString eventInfo)
{
    switch (eventID) {
    case EventAgentStattusChanged:
        onEventAgentStatusChanged(eventInfo);
        break;
    case EventSessionStatusChanged:
        onEventSessionStatusChanged(eventInfo);
        break;
    case EventChatMessage:
        onEventChatMessage(eventInfo);
        break;
    case EventCallStatusChanged:
        onEventCallStatusChanged(eventInfo);
        break;
    default:
        QMessageBox msgBox;
        msgBox.setText("EventID ERROE\n EventInfo: " + eventInfo);
        msgBox.exec();
        break;
    }
}

void MainWidget::parseUserInfo(QString sessionid, QString userData)
{
    UserInfo *userInfo = new UserInfo();
    QString startTag = "<from>";
    QString endTag = "</from>";
    int startIndex = userData.indexOf(startTag) + startTag.length();
    int endIndex = userData.indexOf(endTag);
    userInfo->thisDN = userData.mid(startIndex, endIndex - startIndex);

    startTag = "<via>";
    endTag = "</via>";
    startIndex = userData.indexOf(startTag) + startTag.length();
    endIndex = userData.indexOf(endTag);
    userInfo->via = userData.mid(startIndex, endIndex - startIndex);

    userInfo->sessionid = sessionid;

    this->m_pMapSession_UserInfo->insert(sessionid, userInfo);
}

void MainWidget::chatMessageHandle(QString sessionid, QString userDate)
{
    SessionHandle* sessionHandle = this->m_pMapSessionid_SessionHandle->value(sessionid);
    sessionHandle->addNewItemFunc(userDate);

    this->m_pChatListWidget->notifyHavingNewMessage(this->m_pMapSessionid_TWI->value(sessionid));

    /*
    QTreeWidgetItem *item = this->m_pMapSessionid_TWI->value(sessionid);
    QQuickWidget *quickWidget = this->m_pMapTWI_QuickWidget->value(item);

    QQuickItem *rootItem = quickWidget->rootObject();
    if (NULL != rootItem)
    {
        QVariantMap newElement;
        newElement.insert("md_msgtype", 0);
        newElement.insert("md_direction", 1);
        newElement.insert("md_msgContent", userDate);
        newElement.insert("md_pictureFilePath", "");
        newElement.insert("md_audioFilePath", "");
        newElement.insert("md_videoFilePath", "");

        QList<QObject*> allChildrenItem = rootItem->findChildren<QObject*>();
        QAbstractItemModel *listViewModel = NULL;
        for (int i=0; i < allChildrenItem.size(); ++i)
        {
            qDebug("ObjectName: %s\n", allChildrenItem.at(i)->objectName().toStdString().c_str());
            if (0 == allChildrenItem.at(i)->objectName().compare("historyListViewModel"))
            {
                listViewModel = static_cast<QAbstractItemModel*>(allChildrenItem.at(i));
                break;
            }
        }
        if (NULL != listViewModel)
        {
            QMetaObject::invokeMethod(allChildrenItem.at(0), "append", Q_ARG(QVariant, QVariant::fromValue(newElement)));
        }

        qDebug("rootItem objectName :%s", rootItem->objectName().toStdString().c_str());
        QObject *listViewRect = rootItem->findChild<QObject*>("listViewRect");
        if (NULL != listViewRect)
        {
            qDebug("listViewRect objectName :%s", listViewRect->objectName().toStdString().c_str());
            QListView *listView = listViewRect->findChild<QListView*>("historyListView");
            if (NULL != listView)
            {
                qDebug("listView objectName :%s", listView->objectName().toStdString().c_str());

                QMetaObject::invokeMethod(listView->model(), "append", Q_ARG(QVariant, QVariant::fromValue(newElement)));
                QMetaObject::invokeMethod(listView->model(), "positionViewAtEnd");
            }
        }
    }

//    QQmlContext *ctx = quickWidget->rootContext();
//    if (NULL != ctx)
//    {
//        QVariantMap newElement;
//        newElement.insert("md_msgtype", 0);
//        newElement.insert("md_direction", 1);
//        newElement.insert("md_msgContent", userDate);
//        newElement.insert("md_pictureFilePath", "");
//        newElement.insert("md_audioFilePath", "");
//        newElement.insert("md_videoFilePath", "");
//        ctx->setContextProperty("historyListViewModel", QVariant::fromValue(newElement));
//    }*/
}

void MainWidget::onEventAgentStatusChanged(QString eventInfo)
{
    QStringList info = eventInfo.split("&");
    if (info.length() > 0)
    {
        if (0 == info[0].compare("STATE_AGENT_LOGOUT", Qt::CaseInsensitive))
        {
            enableAgentButtons(STATE_AGENT_LOGOUT);
        }
        else if (0 == info[0].compare("STATE_AGENT_READY", Qt::CaseInsensitive))
        {
            enableAgentButtons(STATE_AGENT_READY);
        }
        else if (0 == info[0].compare("STATE_AGENT_NOTREADY", Qt::CaseInsensitive))
        {
            enableAgentButtons(STATE_AGENT_NOTREADY);
        }
        else
        {
            //非法状态
        }
    }
    else
    {
        return ;
    }
}

void MainWidget::onEventSessionStatusChanged(QString eventInfo)
{
    QStringList info = eventInfo.split("&");
    SESSION_STATUS sessionStatus;

    if (info.length() >=2 )
    {
        if (0 == info[1].compare("STATE_SESSION_RINGING", Qt::CaseInsensitive))
        {
            sessionStatus = STATE_SESSION_RINGING;
        }
        else if(0 == info[1].compare("STATE_SESSION_ESTABLISHED", Qt::CaseInsensitive))
        {
            sessionStatus = STATE_SESSION_ESTABLISHED;
        }
        else if(0 == info[1].compare("STATE_SESSION_RELEASED", Qt::CaseInsensitive))
        {
            sessionStatus = STATE_SESSION_RELEASED;
        }
        else if(0 == info[1].compare("STATE_SESSION_HELD", Qt::CaseInsensitive))
        {
            sessionStatus = STATE_SESSION_HELD;
        }
        sessionStatusChangedHandle(sessionStatus, info[0], info[2]);
    }

}

void MainWidget::onEventChatMessage(QString eventInfo)
{
    QStringList info = eventInfo.split("&");
    if (info.length() >= 2)
    {
        chatMessageHandle(info[0], info[1]);
    }
}

void MainWidget::onEventCallStatusChanged(QString eventInfo)
{

}

void MainWidget::enableAgentButtons(AGENT_STATUS agentStatus)
{
    switch (agentStatus)
    {
    case STATE_AGENT_LOGOUT:
        this->ui->pushButtonBreak->setEnabled(false);
        this->ui->pushButtonConsult->setEnabled(false);
        this->ui->pushButtonHold_retrive->setEnabled(false);
        this->ui->pushButtonHookOff_release->setEnabled(false);
        this->ui->pushButtonInsert->setEnabled(false);
        this->ui->pushButtonLogin_out->setEnabled(true);
        this->ui->pushButtonMonitor->setEnabled(false);
        this->ui->pushButtonReady_NOT->setEnabled(false);
        this->ui->pushButtonLogin_out->setText("签  入");
        break;
    case STATE_AGENT_NOTREADY_FOR_NEXTCALL:
    case STATE_AGENT_READY:
        this->ui->pushButtonLogin_out->setEnabled(true);
        this->ui->pushButtonReady_NOT->setEnabled(true);
        this->ui->pushButtonConsult->setEnabled(true);
        this->ui->pushButtonLogin_out->setText("签  出");
        this->ui->pushButtonReady_NOT->setText("示  忙");
        break;
    case STATE_AGENT_NOTREADY:
        this->ui->pushButtonLogin_out->setEnabled(true);
        this->ui->pushButtonReady_NOT->setEnabled(true);
        this->ui->pushButtonReady_NOT->setText("示  闲");
        break;
    default:
        break;
    }
    this->m_agentState = agentStatus;
}

void MainWidget::sessionStatusChangedHandle(SESSION_STATUS sessionStatus, QString sessionid, QString userData)
{
    switch (sessionStatus) {
    case STATE_SESSION_RINGING:
        //添加振铃处理
        parseUserInfo(sessionid, userData);
        emit sessionStatusRinging(sessionid, userData);
        break;
    case STATE_SESSION_ESTABLISHED:
        //会话建立处理
        this->m_pChatListWidget->createNewTreeWidgetItem(this->m_pMapSession_UserInfo->value(sessionid));
        break;
    case STATE_SESSION_RELEASED:
        sessionEnd(sessionid);
        break;
    default:
        break;
    }
}

void MainWidget::sessionEnd(QString sessionid)
{
    QTreeWidgetItem* item = this->m_pMapSessionid_TWI->value(sessionid);
    QQuickWidget* quickWidget = this->m_pMapTWI_QuickWidget->value(item);
    UserInfo* userInfo = this->m_pMapSession_UserInfo->value(sessionid);
    SessionHandle* sessionHandle = this->m_pMapSessionid_SessionHandle->value(sessionid);

    this->m_pMapSessionid_SessionHandle->remove(sessionid);
    this->m_pMapSessionid_TWI->remove(sessionid);
    this->m_pMapSession_UserInfo->remove(sessionid);
    this->m_pMapTWI_QuickWidget->remove(item);

    if (this->m_pCurrentShowQuickWidget == quickWidget)
        this->m_pCurrentShowQuickWidget = NULL;

    delete quickWidget;
    delete userInfo;
    delete sessionHandle;

    this->m_pChatListWidget->deleteQuickWidget(item);
    delete item;
}

MainWidget::~MainWidget()
{
    delete this->m_pChatListWidget;
    delete ui;
}
