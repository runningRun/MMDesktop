#include "chatlistwidget.h"
#include "globalvar.h"
#include <QQuickWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QQmlContext>

ChatListWidget::ChatListWidget(QWidget *parent)
{
    this->m_pChatListTreeWidget = new QTreeWidget(parent);
    this->m_pChatListTreeWidget->setColumnCount(1);
    this->m_pChatListTreeWidget->setHeaderLabel("会话列表");
    this->m_pChatListTreeWidget->setGeometry(QRect(0, 0, 160, 560));
    this->m_pMapTWI_QuickWidget = new QMap<QTreeWidgetItem*, QQuickWidget*>();
    this->m_pCurrentShowQuickWidget = NULL;

    this->m_ptwiVoice = createTreeWidgetItem(this->m_pChatListTreeWidget,QStringList (QString("语音通话")));
    this->m_ptwiVoice->setIcon(0, QIcon("res/telephone.png"));

    this->m_ptwiWeChat = createTreeWidgetItem(this->m_pChatListTreeWidget, QStringList(QString("微信")));
    this->m_ptwiWeChat->setIcon(0, QIcon("res/wechat.png"));

    this->m_ptwiApp = createTreeWidgetItem(this->m_pChatListTreeWidget, QStringList(QString("手机APP")));
    this->m_ptwiApp->setIcon(0, QIcon("res/app.png"));

    connect(this->m_pChatListTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(treeWidgetItemClicked(QTreeWidgetItem*,int)));

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

    if (NULL != this->m_pCurrentShowQuickWidget)
    {
        this->m_pCurrentShowQuickWidget->hide();
    }
    this->m_pCurrentShowQuickWidget = this->m_pMapTWI_QuickWidget->value(newChildItem);
    this->m_pCurrentShowQuickWidget->show();
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
        return ;
    }
    else
    {
        this->m_pCurrentShowQuickWidget->hide();
        this->m_pCurrentShowQuickWidget = this->m_pMapTWI_QuickWidget->value(item);
        this->m_pCurrentShowQuickWidget->show();
    }

    item->setBackgroundColor(0, QColor("transparent"));
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
        this->m_pCurrentShowQuickWidget = NULL;
}

void ChatListWidget::notifyHavingNewMessage(QTreeWidgetItem *item)
{
    if (this->m_pCurrentShowQuickWidget != this->m_pMapTWI_QuickWidget->value(item))
        item->setBackgroundColor(0, QColor("#FFA07A"));
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

