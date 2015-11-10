#ifndef CRMWIDGET_H
#define CRMWIDGET_H

#include <QWidget>

class QLabel;
class QVBoxLayout;

class CRMWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CRMWidget(QWidget *parent = 0);
    ~CRMWidget();

protected:
    void initComponent();

signals:

public slots:

private:
    QLabel* m_pLabel;
    QVBoxLayout* m_pMainVLayout;

};

#endif // CRMWIDGET_H
