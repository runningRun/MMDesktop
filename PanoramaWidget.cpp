#include "PanoramaWidget.h"
#include "MyButton.h"
#include <QDebug>
#include <QPainter>
#include <QBoxLayout>
#include <QSignalMapper>
#include <CustomInfoWidget.h>
#include <CRMWidget.h>
#include <KnowledgeBaseWidget.h>

PanoramaWidget::PanoramaWidget(QWidget *parent) : QWidget(parent)
{

    this->m_pBtnVLayout = new QVBoxLayout;
    this->m_pMainHLayout = new QHBoxLayout;

    this->m_pCstInfoWidget = new CustomInfoWidget;
    this->m_pCRMWidget = new CRMWidget;
    this->m_pKnowledgeWidget = new KnowledgeBaseWidget;

    this->m_pCstInfoWidget->hide();
    this->m_pCRMWidget->hide();
    this->m_pKnowledgeWidget->hide();

    initComponet();
}

void PanoramaWidget::initComponet()
{
    QStringList stringList;
    stringList << "客户资料" << "CRM  " << "知识库";

    MyButton * myButton;

    //设置按钮
    for (int i=0; i<stringList.size(); ++i)
    {
        myButton = new MyButton();
        myButton->setText(stringList.at(i), TOP2BOTTOM);
        this->m_listMyButton.append(myButton);
        connect(myButton, SIGNAL(clicked()), &m_SigalMapper, SLOT(map()));
        this->m_SigalMapper.setMapping(myButton, i);
        this->m_pBtnVLayout->addWidget(myButton);
    }
    connect(&m_SigalMapper, SIGNAL(mapped(int)), this, SLOT(switchWidget(int)));

    //添加与按钮对应的显示信息界面

    this->m_pMainHLayout->addLayout(this->m_pBtnVLayout);
    this->m_pMainHLayout->addWidget(this->m_pCstInfoWidget);
    this->m_pMainHLayout->addWidget(this->m_pCRMWidget);
    this->m_pMainHLayout->addWidget(this->m_pKnowledgeWidget);

    this->m_pMainHLayout->setSpacing(1);

    setLayout(this->m_pMainHLayout);
    this->m_pCurrentWidget = this->m_pCstInfoWidget;
    this->m_pCurrentWidget->show();
    this->m_listMyButton.at(0)->setBtnClicked(true);

}


void PanoramaWidget::paintEvent(QPaintEvent * pev)
{

}


void PanoramaWidget::switchWidget(int index)
{
    this->m_pCurrentWidget->hide();
    switch (index) {
    case 0:
        this->m_pCurrentWidget = this->m_pCstInfoWidget;
        break;
    case 1:
        this->m_pCurrentWidget = this->m_pCRMWidget;
        break;
    case 2:
        this->m_pCurrentWidget = this->m_pKnowledgeWidget;
        break;
    default:
        break;
    }
    this->m_pCurrentWidget->show();

    int i = this->m_listMyButton.count();
    for (; i>0; --i)
    {
        this->m_listMyButton.at(i-1)->setBtnClicked(false);
    }
    this->m_listMyButton.at(index)->setBtnClicked(true);

}


PanoramaWidget::~PanoramaWidget()
{

}

