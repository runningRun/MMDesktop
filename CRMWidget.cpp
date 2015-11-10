#include "CRMWidget.h"
#include <QLabel>
#include <QVBoxLayout>

CRMWidget::CRMWidget(QWidget *parent) : QWidget(parent)
{
    m_pLabel = new QLabel("<h1>此处应该显示CRM相关信息</h1>");
    m_pMainVLayout = new QVBoxLayout;

   initComponent();
}

void CRMWidget::initComponent()
{
    m_pMainVLayout->addWidget(m_pLabel);

    setLayout(m_pMainVLayout);
}

CRMWidget::~CRMWidget()
{

}

