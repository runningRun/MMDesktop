#include "CstInfoWidget.h"
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

CstInfoWidget::CstInfoWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* mainVLayout = new QVBoxLayout;
    this->m_pCstInfoGroupBox = new QGroupBox("客户信息");
    this->m_pHistoryPug = new QGroupBox("历史足迹");


    QVBoxLayout* cstInfoLayout = new QVBoxLayout;
    cstInfoLayout->addWidget(new QLabel("此处应显示客户信息"));
    this->m_pCstInfoGroupBox->setLayout(cstInfoLayout);

    QVBoxLayout* historyLayout = new QVBoxLayout;
    historyLayout->addWidget(new QLabel("此处应显示客户历史足迹"));
    this->m_pHistoryPug->setLayout(historyLayout);


    mainVLayout->addWidget(this->m_pCstInfoGroupBox);
    mainVLayout->addWidget(this->m_pHistoryPug);
    setLayout(mainVLayout);
}

CstInfoWidget::~CstInfoWidget()
{

}

