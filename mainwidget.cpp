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
#include <QTimer>
#include <QDebug>
#include "agentinfo.h"

MainWidget::MainWidget(MMDBaseWidget *parent) :
    MMDBaseWidget(parent),
    ui(new Ui::MainWidget)
{    
    ui->setupUi(this);

    this->ui->lineEditAgentID_2->setText(AgentInfo::agentID);
//    this->ui->lineEditAgentName->setText(AgentInfo::agentName);
//    this->ui->lineEditAgentNameGroupID->setText(AgentInfo::agentGroupID);
//    this->ui->lineEditAgentThisDN->setText(AgentInfo::agentThisDN);

    this->m_pChatListWidget = new ChatListWidget(this->ui->widgetChatList);
    this->m_pMapSessionid_TWI = new QMap<QString, QTreeWidgetItem*>();
    this->m_pMapTWI_QuickWidget = new QMap<QTreeWidgetItem*, QQuickWidget*>();
    this->m_pMapSession_UserInfo = new QMap<QString, UserInfo*>();
    this->m_pMapSessionid_SessionHandle = new QMap<QString, SessionHandle*>();
    this->m_pCurrentShowQuickWidget = NULL;
    this->m_agentState = STATE_AGENT_LOGOUT;
    this->m_callStatus = STATE_CALL_IVR;
    this->m_pTimerHookoffBtnChangeBgClr = new QTimer(this);
    this->m_pTimerHookoffBtnChangeBgClr->setInterval(300);
    initToolBtnIcon();
    enableAgentButtons(this->m_agentState);

    connect(this->m_pChatListWidget, SIGNAL(createNewTreeWidgetItem(QTreeWidgetItem*, QString)),
                     this, SLOT(ChatListWidgetAddedNewTreeWidgetItem(QTreeWidgetItem*, QString)));

    connect(this->m_pChatListWidget, SIGNAL(signalCurrentShowQuickWidgetChanged(QQuickWidget*,QQuickWidget*)),
            this, SLOT(switchCustomerInfo(QQuickWidget*,QQuickWidget*)));

    connect(this->ui->toolButtonLogin_out, SIGNAL(clicked()),
            this, SLOT(PushButtonLogin_outClicked()));
    connect(this->ui->toolButtonReady_NOT, SIGNAL(clicked()),
            this, SLOT(PushButtonReay_NOTClicked()));
    connect(this->m_pTimerHookoffBtnChangeBgClr, SIGNAL(timeout()),
            this, SLOT(timerHookoffBtnChangeBgClrTimeout()));
    connect(this->ui->toolButtonHookOff_release, SIGNAL(clicked()),
            this, SLOT(PushButtonHookoff_releaseClicked()));
    connect(this->ui->toolButtonHold_retrive, SIGNAL(clicked()),
            this, SLOT(PushButtonHold_retiveClicked()));
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

void MainWidget::initToolBtnIcon()
{
    /*
    enum ICONINDEX
    {
        AGENTIN = 0,
        AGENTOUT = 1,
        READY = 2,
        NOTREADY = 3,
        DIAL = 4,
        HOOKOFF = 5,
        HANGUP = 6,
        HOLD = 7,
        RETRIEVE = 8,
        CONSULT = 9,
        MONITOR = 10,
        BREAK = 11,
        INTRUDE = 12
    };
    */
    this->m_pListToolBtnIcon.append(new QIcon(":/res/agentIn.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/agentOut.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/agentReady.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/agentNotready.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/dail.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/hookOff.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/hangup.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/holdCall.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/retrieveCall.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/callHelp.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/callMonitor.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/callChai.png"));
    this->m_pListToolBtnIcon.append(new QIcon(":/res/callCha.png"));

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
void MainWidget::PushButtonHookoff_releaseClicked()
{
    switch (this->m_callStatus) {
    case STATE_CALL_RINGING:
        //摘机应答,停止定时器
        this->m_pTimerHookoffBtnChangeBgClr->stop();
        this->ui->toolButtonHookOff_release->setStyleSheet("");
        this->m_pAgentOCX->answer(this->m_callID);
        break;
    case STATE_CALL_ESTABLISHED:
        //挂机
        this->m_pAgentOCX->release(this->m_callID);
        break;
    default:
        break;
    }
}

void MainWidget::PushButtonHold_retiveClicked()
{
    switch (this->m_callStatus)
    {
    case STATE_CALL_ESTABLISHED:
        //保持
        this->m_pAgentOCX->hold(this->m_callID);
        break;
    case STATE_CALL_HELD:
        this->m_pAgentOCX->retrieve(this->m_callID);
        //取回
    default:
        break;
    }
}
void MainWidget::timerHookoffBtnChangeBgClrTimeout()
{
    static int currentHookoffBtnBgClr = 0;
    static QString backgroundColor[] = {"background-color: green", "background-color: blue", "background-color: yellow","background-color: transparent", };
    switch (currentHookoffBtnBgClr) {
    case 0:
        currentHookoffBtnBgClr = 1;
        break;
    case 1:
        currentHookoffBtnBgClr = 2;
        break;
    case 2:
        currentHookoffBtnBgClr = 3;
        break;
    case 3:
        currentHookoffBtnBgClr = 0;
        break;
    default:
        break;
    }
    this->ui->toolButtonHookOff_release->setStyleSheet(backgroundColor[currentHookoffBtnBgClr]);
}

void MainWidget::switchCustomerInfo(QQuickWidget* prevoiusShowQuickWidget, QQuickWidget* currentShowQuickWidget)
{
    if (NULL == currentShowQuickWidget)
        this->ui->widget->showUserInfo((UserID)"");
    else
    {
        QString sessionid;
        QTreeWidgetItem* item = this->m_pMapTWI_QuickWidget->key(currentShowQuickWidget);
        sessionid = this->m_pMapSessionid_TWI->key(item);
        UserInfo* userInfo = this->m_pMapSession_UserInfo->value(sessionid);
        this->ui->widget->showUserInfo((UserID)userInfo->thisDN);
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

void MainWidget::callStatusChangedHandle(CALL_STATUS callStaus, QString callID, QString userData)
{
    m_callStatus = callStaus;
    m_callID = callID;
    enableCallButtons(callStaus);
    switch (callStaus) {
    case STATE_CALL_RINGING:
        this->m_pTimerHookoffBtnChangeBgClr->start();
        break;
    case STATE_CALL_ESTABLISHED:
        if (this->m_pTimerHookoffBtnChangeBgClr->isActive())
        {
            this->m_pTimerHookoffBtnChangeBgClr->stop();
            this->ui->toolButtonHookOff_release->setStyleSheet("");
        }
        break;
    default:
        break;
    }
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
    //eventInfo := callID&status&userData
    QStringList info = eventInfo.split('&');
    CALL_STATUS callStatus;

    qDebug("CallStatus:%s", info[1].toStdString().c_str());
    if (info.length() >= 2)
    {
        QString strStatus = info[1];
        if (0 == strStatus.compare("STATE_CALL_RINGING", Qt::CaseInsensitive))
        {
            callStatus = STATE_CALL_RINGING;
        }
        else if (0 == strStatus.compare("STATE_CALL_ESTABLISHED", Qt::CaseInsensitive))
        {
            callStatus = STATE_CALL_ESTABLISHED;
        }
        else if (0 == strStatus.compare("STATE_CALL_RELEASED", Qt::CaseInsensitive))
        {
            callStatus = STATE_CALL_RELEASED;
        }
        else if (0 == strStatus.compare("STATE_CALL_HELD", Qt::CaseInsensitive))
        {
            callStatus = STATE_CALL_HELD;
        }
        else if (0 == strStatus.compare("STATE_CALL_DIALING", Qt::CaseInsensitive))
        {
            callStatus = STATE_CALL_DIALING;
        }
        else if (0 == strStatus.compare("STATE_CALL_CONSULT_DIALING", Qt::CaseInsensitive))
        {
            callStatus = STATE_CALL_CONSULT_DIALING;
        }
        else if (0 == strStatus.compare("STATE_CALL_CONSULT", Qt::CaseInsensitive))
        {
            callStatus = STATE_CALL_CONSULT;
        }
        else if (0 == strStatus.compare("STATE_CALL_CONFERENCE", Qt::CaseInsensitive))
        {
            callStatus = STATE_CALL_CONFERENCE;
        }
        else if (0 == strStatus.compare("STATE_CALL_MONITORING", Qt::CaseInsensitive))
        {
            callStatus = STATE_CALL_MONITORING;
        }
        callStatusChangedHandle(callStatus, info[0], info[1]);
    }
}

void MainWidget::enableAgentButtons(AGENT_STATUS agentStatus)
{
    switch (agentStatus)
    {
    case STATE_AGENT_LOGOUT:
        this->ui->toolButtonBreak->setEnabled(false);
        this->ui->toolButtonConsult->setEnabled(false);
        this->ui->toolButtonHold_retrive->setEnabled(false);
        this->ui->toolButtonHookOff_release->setEnabled(false);
        this->ui->toolButtonInsert->setEnabled(false);
        this->ui->toolButtonLogin_out->setEnabled(true);
        this->ui->toolButtonMonitor->setEnabled(false);
        this->ui->toolButtonReady_NOT->setEnabled(false);
        this->ui->toolButtonLogin_out->setText("签 入");
        this->ui->toolButtonLogin_out->setIcon(*(this->m_pListToolBtnIcon.at(AGENTIN)));
        break;
    case STATE_AGENT_NOTREADY_FOR_NEXTCALL:
    case STATE_AGENT_READY:
        this->ui->toolButtonLogin_out->setEnabled(true);
        this->ui->toolButtonReady_NOT->setEnabled(true);
        this->ui->toolButtonConsult->setEnabled(true);
        this->ui->toolButtonLogin_out->setText("签 出");
        this->ui->toolButtonReady_NOT->setText("示 忙");
        this->ui->toolButtonLogin_out->setIcon(*(this->m_pListToolBtnIcon.at(AGENTOUT)));
        this->ui->toolButtonReady_NOT->setIcon(*(this->m_pListToolBtnIcon.at(NOTREADY)));
        break;
    case STATE_AGENT_NOTREADY:
        this->ui->toolButtonLogin_out->setEnabled(true);
        this->ui->toolButtonReady_NOT->setEnabled(true);
        this->ui->toolButtonReady_NOT->setText("示 闲");
        this->ui->toolButtonReady_NOT->setIcon(*(this->m_pListToolBtnIcon.at(READY)));
        break;
    default:
        break;
    }
    this->m_agentState = agentStatus;
}

void MainWidget::enableCallButtons(CALL_STATUS callStatus)
{
    switch (callStatus) {
    case STATE_CALL_RINGING:
        this->ui->toolButtonHookOff_release->setEnabled(true);
        break;
    case STATE_CALL_ESTABLISHED:
        this->ui->toolButtonHookOff_release->setText("挂 机");
        this->ui->toolButtonHold_retrive->setText("保 持");
        this->ui->toolButtonHold_retrive->setEnabled(true);

        this->ui->toolButtonHookOff_release->setIcon(*(this->m_pListToolBtnIcon.at(HANGUP)));
        this->ui->toolButtonHold_retrive->setIcon(*(this->m_pListToolBtnIcon.at(HOLD)));

        break;
    case STATE_CALL_RELEASED:
        this->ui->toolButtonHookOff_release->setText("摘 机");
        this->ui->toolButtonHold_retrive->setText("保 持");
        this->ui->toolButtonHookOff_release->setEnabled(false);
        this->ui->toolButtonHold_retrive->setEnabled(false);

        this->ui->toolButtonHookOff_release->setIcon(*(this->m_pListToolBtnIcon.at(HOOKOFF)));
        this->ui->toolButtonHold_retrive->setIcon(*(this->m_pListToolBtnIcon.at(HOLD)));

        break;
    case STATE_CALL_HELD:
        this->ui->toolButtonHold_retrive->setText("取 回");
        this->ui->toolButtonHold_retrive->setIcon(*(this->m_pListToolBtnIcon.at(RETRIEVE)));
        break;
    default:
        break;
    }
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

    this->ui->widget->showUserInfo("");

    delete item;
}

MainWidget::~MainWidget()
{
    delete this->m_pChatListWidget;
    delete ui;
}
