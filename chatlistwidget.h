#ifndef CHATLIST_H
#define CHATLIST_H

#include <QObject>

class QTreeWidget;
class QTreeWidgetItem;
class QQuickWidget;
class UserInfo;
class QMultiMap<QTreeWidgetItem*, QQuickWidget*>;

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
private slots:
    void treeWidgetItemClicked(QTreeWidgetItem *item, int column);

private:
    QTreeWidget *m_pChatListTreeWidget;
    QTreeWidgetItem *m_ptwiVoice;
    QTreeWidgetItem *m_ptwiWeChat;
    QTreeWidgetItem *m_ptwiApp;
    QQuickWidget* m_pCurrentShowQuickWidget;
    QMap<QTreeWidgetItem*, QQuickWidget*> *m_pMapTWI_QuickWidget;
};

#endif // CHATLIST_H
