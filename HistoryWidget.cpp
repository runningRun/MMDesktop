#include "HistoryWidget.h"
#include <QPainter>
#include <QLabel>

HistoryWidget::HistoryWidget(QWidget *parent) : QWidget(parent)
{
    QLabel *label = new QLabel("暂无历史信息", this);
}

void HistoryWidget::paintEvent(QPaintEvent *event)
{
}

HistoryWidget::~HistoryWidget()
{

}

