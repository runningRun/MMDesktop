#include "chatlistwidget.h"
#include "globalvar.h"
#include <QQuickWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QQmlContext>
#include <QTimer>
#include <QList>

ChatListWidget::ChatListWidget(QWidget *parent)
{
    this->m_pChatListTreeWidget = new QTreeWidget(parent);
    this->m_pMapTWI_QuickWidget = new QMap<QTreeWidgetItem*, QQuickWidget*>();
    this->m_pTimerChangeItemBgColor = new QTimer(this);
    this->m_pListHavingNewMsgTwi = new QList<QTreeWidgetItem*>();
    this->m_pCurrentShowQuickWidget = NULL;
    emit signalCurrentShowQuickWidgetChanged(NULL, NULL);

    //初始化ChatListTreeWidget
    this->m_pChatListTreeWidget->setColumnCount(1);
    this->m_pChatListTreeWidget->setHeaderLabel("会话列表");
    this->m_pChatListTreeWidget->setGeometry(QRect(0, 0, 160, 560));

    this->m_ptwiVoice = createTreeWidgetItem(this->m_pChatListTreeWidget,QStringList (QString("语音通话")));
    this->m_ptwiVoice->setIcon(0, QIcon("res/telephone.png"));
    this->m_ptwiWeChat = createTreeWidgetItem(this->m_pChatListTreeWidget, QStringList(QString("微信")));
    this->m_ptwiWeChat->setIcon(0, QIcon("res/wechat.png"));
    this->m_ptwiApp = createTreeWidgetItem(this->m_pChatListTreeWidget, QStringList(QString("手机APP")));
    this->m_ptwiApp->setIcon(0, QIcon("res/app.png"));
    connect(this->m_pChatListTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(treeWidgetItemClicked(QTreeWidgetItem*,int)));
    connect(this->m_pChatListTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
            this, SLOT(currentTreeWidgetItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    //初始化定时器
    this->m_pTimerChangeItemBgColor->setInterval(300);
    connect(this->m_pTimerChangeItemBgColor, SIGNAL(timeout()), this, SLOT(treeWidgetItemBgColorChange()));
    this->m_pTimerChangeItemBgColor->start(300);

}

void ChatListWidget::Load()
{
    /*
    QTreeWidgetItem *childItem = createTreeWidgetItem(this->m_ptwiVoice, QStringList(QString("session 1")), "1");
    childItem->setIcon(0, QIcon("res/telephone_1.png"));
    this->m_ptwiVoice->addChild(childItem);

    childItem = createTreeWidgetItem(this->m_ptwiWeChat, QStringList(QString("WeChat1")), "2");
    childItem->setIcon(0, QIcon("res/wechat_1.png"));
    this->m_ptwiWeChat->addChild(childItem);
    childItem = createTreeWidgetItem(this->m_ptwiWeChat, QStringList(QString("WeChat2")), "3");
    childItem->setIcon(0, QIcon("res/wechat_2.png"));
    this->m_ptwiWeChat->addChild(childItem);

    childItem = createTreeWidgetItem(this->m_ptwiApp, QStringList(QString("App User 1")), "4");
    childItem->setIcon(0, QIcon("res/app_1.png"));
    this->m_ptwiApp->addChild(childItem);
    childItem = createTreeWidgetItem(this->m_ptwiApp, QStringList(QString("App User 2")), "5");
    childItem->setIcon(0, QIcon("res/app_2.png"));
    this->m_ptwiApp->addChild(childItem);
    */
}

void ChatListWidget::createNewTreeWidgetItem(UserInfo *userInfo)
{
    QTreeWidgetItem* parent, *newChildItem;

    QString via = userInfo->via;
    QString sessionid = userInfo->sessionid;

    if (0 == via.compare("XRobot", Qt::CaseInsensitive))
    {
        parent = this->m_ptwiApp;
    }
    else
    {
        parent = this->m_ptwiWeChat;
    }
    newChildItem = createTreeWidgetItem(parent, QStringList(QString (sessionid)), sessionid);

    if (parent == this->m_ptwiApp)
    {
        newChildItem->setIcon(0,QIcon("res/app_2.png"));
    }
    else if (parent == this->m_ptwiWeChat)
    {
        newChildItem->setIcon(0, QIcon("res/wechat_2.png"));
    }
    parent->addChild(newChildItem);

    this->m_pChatListTreeWidget->expandItem(parent);
    newChildItem->setSelected(true);

    QQuickWidget* previousShowCurrentWidget = this->m_pCurrentShowQuickWidget;
    if (NULL != this->m_pCurrentShowQuickWidget)
    {
        this->m_pCurrentShowQuickWidget->hide();
    }
    this->m_pCurrentShowQuickWidget = this->m_pMapTWI_QuickWidget->value(newChildItem);
    this->m_pCurrentShowQuickWidget->show();
    emit signalCurrentShowQuickWidgetChanged(previousShowCurrentWidget, this->m_pCurrentShowQuickWidget);
}

QTreeWidgetItem* ChatListWidget::createTreeWidgetItem(QTreeWidget *parent, QStringList stringList)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(parent, stringList);
    return item;
}

QTreeWidgetItem* ChatListWidget::createTreeWidgetItem(QTreeWidgetItem *parent, QStringList stringList, QString sessionid)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(parent, stringList);
    emit createNewTreeWidgetItem(item, sessionid);
    return item;
}

void ChatListWidget::addNewQuickWidget(QTreeWidgetItem *item, QQuickWidget *quickWidget)
{
    this->m_pMapTWI_QuickWidget->insert(item, quickWidget);
}

QQuickWidget* ChatListWidget::currentShowQuickWidget()
{
    return this->m_pCurrentShowQuickWidget;
}

void ChatListWidget::treeWidgetItemClicked(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *parent = item->parent();
    QQuickWidget* previousQuickWidget = this->m_pCurrentShowQuickWidget;
    if (NULL == parent)
    {
        if (!item->isExpanded())
            item->setExpanded(true);
        else
            item->setExpanded(false);
        return;
    }
    if ((this->m_pMapTWI_QuickWidget->value(item) == m_pCurrentShowQuickWidget) || (NULL == this->m_pCurrentShowQuickWidget))
    {
        this->m_pCurrentShowQuickWidget = this->m_pMapTWI_QuickWidget->value(item);
        this->m_pCurrentShowQuickWidget->show();
        emit signalTreeWidgetItemClicked(item, column);
    }
    else
    {
        this->m_pCurrentShowQuickWidget->hide();
        this->m_pCurrentShowQuickWidget = this->m_pMapTWI_QuickWidget->value(item);
        this->m_pCurrentShowQuickWidget->show();
    }

    item->setBackgroundColor(0, QColor("transparent"));

    if (this->m_pListHavingNewMsgTwi->contains(item))
    {
        this->m_pListHavingNewMsgTwi->removeOne(item);
    }
    emit signalCurrentShowQuickWidgetChanged(previousQuickWidget, this->m_pCurrentShowQuickWidget);
}

void ChatListWidget::currentTreeWidgetItemChanged(QTreeWidgetItem *currentItem, QTreeWidgetItem *previousItem)
{
    if (NULL != previousItem)
    {
        if (currentItem->parent() != NULL && previousItem->parent() != NULL)
        {
            if (this->m_pListHavingNewMsgTwi->contains(currentItem))
            {
                this->m_pListHavingNewMsgTwi->removeOne(currentItem);
            }
//            this->m_pListHavingNewMsgTwi->append(previousItem);
        }
    }
}

void ChatListWidget::treeWidgetItemBgColorChange()
{
    static int currentColorFlag = 0;
    QTreeWidgetItem* item;
    int size = this->m_pListHavingNewMsgTwi->size();
    int i = 0;
    if (0 == currentColorFlag)
    {
        for (i=0; i<size; ++i)
        {
            this->m_pListHavingNewMsgTwi->at(i)->setBackgroundColor(0, QColor("#FFA07A"));
        }
        currentColorFlag = 1;
    }
    else if (1 == currentColorFlag)
    {
        for (i=0; i<size; ++i)
        {
            this->m_pListHavingNewMsgTwi->at(i)->setBackgroundColor(0, QColor("green"));
        }
        currentColorFlag = 2;
    }
    else if (2 == currentColorFlag)
    {
        for (i=0; i<size; ++i)
        {
            this->m_pListHavingNewMsgTwi->at(i)->setBackgroundColor(0, QColor("transparent"));
        }
        currentColorFlag = 0;
    }
}
QQuickWidget* ChatListWidget::createQuickWidget(QWidget *parent)
{
   QQuickWidget* quickWidget = new QQuickWidget(parent);
   quickWidget->setGeometry(QRect(-1, -1, 480, 560));
   quickWidget->setSource(QUrl::fromLocalFile("D:/Qt/project/Qt Creator Project/MMDesktop/qml/myChatView.qml"));
   quickWidget->rootContext()->setContextProperty("sessionID", "123-456-789");
   quickWidget->hide();
   return quickWidget;
}

void ChatListWidget::deleteQuickWidget(QTreeWidgetItem *item)
{
    QQuickWidget* quickWidget = this->m_pMapTWI_QuickWidget->value(item);
    this->m_pMapTWI_QuickWidget->remove(item);
    if (this->m_pCurrentShowQuickWidget == quickWidget)
    {
        QQuickWidget* previousQuickWidget = this->m_pCurrentShowQuickWidget;
        this->m_pCurrentShowQuickWidget = NULL;
        emit signalCurrentShowQuickWidgetChanged(previousQuickWidget, this->m_pCurrentShowQuickWidget);
    }
}

void ChatListWidget::notifyHavingNewMessage(QTreeWidgetItem *item)
{
    if (this->m_pCurrentShowQuickWidget != this->m_pMapTWI_QuickWidget->value(item))
    {
        item->setBackgroundColor(0, QColor("#FFA07A"));
        this->m_pListHavingNewMsgTwi->append(item);
    }
}

ChatListWidget::~ChatListWidget()
{
    QQuickWidget *deletingQuickWidget;
    QList<QQuickWidget*> quickWidgetList(this->m_pMapTWI_QuickWidget->values());

    while(!quickWidgetList.empty())
    {
        deletingQuickWidget = quickWidgetList.at(0);
        quickWidgetList.removeAt(0);
        delete deletingQuickWidget;
    }

    delete this->m_pMapTWI_QuickWidget;
    delete this->m_ptwiApp;
    delete this->m_ptwiVoice;
    delete this->m_ptwiWeChat;
}

