#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>

class HistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryWidget(QWidget *parent = 0);
    ~HistoryWidget();

protected:
    void paintEvent(QPaintEvent* event);

signals:

public slots:

};

#endif // HISTORYWIDGET_H
