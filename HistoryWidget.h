#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include "common.h"

class QLabel;

class HistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryWidget(QWidget *parent = 0);
    ~HistoryWidget();
    void loadUserHistory(UserID userid);

protected:
    void paintEvent(QPaintEvent* event);

signals:

public slots:

private:
    QLabel* m_pHistoryLabel;

};

#endif // HISTORYWIDGET_H
