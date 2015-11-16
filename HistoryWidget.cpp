#include "HistoryWidget.h"
#include <QPainter>
#include <QLabel>

HistoryWidget::HistoryWidget(QWidget *parent) : QWidget(parent)
{
    this->m_pHistoryLabel = new QLabel("暂无历史信息", this);
}

void HistoryWidget::loadUserHistory(UserID userid)
{
    this->m_pHistoryLabel->setText("暂无用户" + userid + "的历史信息");
}

void HistoryWidget::paintEvent(QPaintEvent *event)
{
}

HistoryWidget::~HistoryWidget()
{

}

