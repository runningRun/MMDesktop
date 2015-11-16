#ifndef CHATLIST_H
#define CHATLIST_H

#include <QObject>

class QTreeWidget;
class QTreeWidgetItem;
class QQuickWidget;
class UserInfo;
class QTimer;
class QMultiMap<QTreeWidgetItem*, QQuickWidget*>;
//class QList<QTreeWidgetItem *>;

class ChatListWidget : public QObject
{
    Q_OBJECT
public:
    ChatListWidget(QWidget *parent);
    void Load();
    ~ChatListWidget();

    void createNewTreeWidgetItem(UserInfo *userInfo);
    void addNewQuickWidget(QTreeWidgetItem* item, QQuickWidget* quickWidget);
    void deleteQuickWidget(QTreeWidgetItem* item);
    void notifyHavingNewMessage(QTreeWidgetItem* item);
    QQuickWidget* currentShowQuickWidget();

private:
    QQuickWidget* createQuickWidget(QWidget* parent);
    QTreeWidgetItem* createTreeWidgetItem(QTreeWidget* parent, QStringList stringList);
    QTreeWidgetItem* createTreeWidgetItem(QTreeWidgetItem* item, QStringList stringList, QString sessionid="");

signals:
    void createNewTreeWidgetItem(QTreeWidgetItem* item, QString sessionid);
    void signalTreeWidgetItemClicked(QTreeWidgetItem* item, int column);
    void signalCurrentShowQuickWidgetChanged(QQuickWidget* previous, QQuickWidget* current);
private slots:
    void treeWidgetItemClicked(QTreeWidgetItem *item, int column);
    void currentTreeWidgetItemChanged(QTreeWidgetItem* currentItem, QTreeWidgetItem* previousItem);
    void treeWidgetItemBgColorChange();

private:
    QTreeWidget *m_pChatListTreeWidget;
    QTreeWidgetItem *m_ptwiVoice;
    QTreeWidgetItem *m_ptwiWeChat;
    QTreeWidgetItem *m_ptwiApp;
    QQuickWidget* m_pCurrentShowQuickWidget;
    QTimer* m_pTimerChangeItemBgColor;
    QMap<QTreeWidgetItem*, QQuickWidget*> *m_pMapTWI_QuickWidget;
    QList<QTreeWidgetItem*>* m_pListHavingNewMsgTwi;
};

#endif // CHATLIST_H
