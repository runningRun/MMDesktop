#ifndef KNOWLEDGEBASEWIDGET_H
#define KNOWLEDGEBASEWIDGET_H

#include <QWidget>

class QLabel;
class QVBoxLayout;
class KnowledgeBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KnowledgeBaseWidget(QWidget *parent = 0);
    ~KnowledgeBaseWidget();

protected:
    void initComponent();

signals:

public slots:

private:
    QLabel* m_pLabel;
    QVBoxLayout* m_pMainVLayout;
};

#endif // KNOWLEDGEBASEWIDGET_H
