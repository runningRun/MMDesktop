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
    cstInfoLayout->addWidget(this->m_pInfoLabel = new QLabel("此处应显示客户信息"));
    this->m_pCstInfoGroupBox->setLayout(cstInfoLayout);

    QVBoxLayout* historyLayout = new QVBoxLayout;
    historyLayout->addWidget(this->m_pHistoryPugLabel = new QLabel("此处应显示客户历史足迹"));
    this->m_pHistoryPug->setLayout(historyLayout);


    mainVLayout->addWidget(this->m_pCstInfoGroupBox);
    mainVLayout->addWidget(this->m_pHistoryPug);
    setLayout(mainVLayout);
}

void CstInfoWidget::loadUserInfo(UserID userid)
{
    this->m_pHistoryPugLabel->setText("此处应显示客户"+ userid + "的历史足迹");
    this->m_pInfoLabel->setText("此处应显示客户" + userid + "的信息");
    this->m_pHistoryPugLabel->adjustSize();
    this->m_pInfoLabel->adjustSize();
}

CstInfoWidget::~CstInfoWidget()
{

}

