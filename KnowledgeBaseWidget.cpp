#include "KnowledgeBaseWidget.h"
#include <QLabel>
#include <QVBoxLayout>

KnowledgeBaseWidget::KnowledgeBaseWidget(QWidget *parent) : QWidget(parent)
{
    this->m_pLabel = new QLabel("<h1>此处应该显示知识库信息</h1>");
    this->m_pMainVLayout = new QVBoxLayout;

    initComponent();
}

void KnowledgeBaseWidget::initComponent()
{
    this->m_pMainVLayout->addWidget(this->m_pLabel);

    setLayout(this->m_pMainVLayout);
}

KnowledgeBaseWidget::~KnowledgeBaseWidget()
{

}

