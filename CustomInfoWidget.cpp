#include "CustomInfoWidget.h"
#include "MyButton.h"
#include "common.h"

#include "HistoryWidget.h"
#include "CstInfoWidget.h"

CustomInfoWidget::CustomInfoWidget(QWidget *parent) : QWidget(parent)
{
    this->m_pHistoryWidget = new HistoryWidget();
    this->m_pCstInfoWidget = new CstInfoWidget();
    initComponet();
}

void CustomInfoWidget::initComponet()
{
    //创建按钮
    MyButton* myButton;
    QStringList stringList;
    QHBoxLayout *pHLayoutBtn = new QHBoxLayout;
    pHLayoutBtn->setSpacing(0);

    stringList << "客户资料" << "历史记录";
    for (int i=0; i<stringList.size(); ++i)
    {
        myButton = new MyButton();
        myButton->setText(stringList.at(i), LEFT2RIGHT);
        connect(myButton, SIGNAL(clicked()), &m_signalMapper, SLOT(map()));

        this->m_listMyButton.append(myButton);
        this->m_signalMapper.setMapping(myButton, i);
        pHLayoutBtn->addWidget(myButton);
    }
    connect(&this->m_signalMapper, SIGNAL(mapped(int)), this, SLOT(switchPage(int)));



    m_pMainVLayout = new QVBoxLayout;

    m_pMainVLayout->addLayout(pHLayoutBtn);
    m_pMainVLayout->addWidget(this->m_pCstInfoWidget);
    m_pMainVLayout->addWidget(this->m_pHistoryWidget);
    this->m_pCurrentShowWidget = this->m_pCstInfoWidget;
    this->m_listMyButton.at(0)->setBtnClicked(true);        //界面对应的按钮设置为点击状态
    this->m_pHistoryWidget->hide();
    setLayout(m_pMainVLayout);

}

void CustomInfoWidget::switchPage(int index)
{
    this->m_pCurrentShowWidget->hide();
    switch (index) {
    case 0:
        this->m_pCurrentShowWidget = this->m_pCstInfoWidget;
        break;
    case 1:
        this->m_pCurrentShowWidget = this->m_pHistoryWidget;
    default:
        break;
    }
    this->m_pCurrentShowWidget->show();

    int i = this->m_listMyButton.count();
    for (; i>0; --i)
    {
        this->m_listMyButton.at(i-1)->setBtnClicked(false);
    }
    this->m_listMyButton.at(index)->setBtnClicked(true);
}

CustomInfoWidget::~CustomInfoWidget()
{
}

