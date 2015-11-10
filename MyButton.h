#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include "common.h"

class QPaintEvent;
class QPainter;

class MyButton : public QWidget
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = 0);
    ~MyButton();

    void setText(const QString text, TEXTDIRECTION textDirection);
    const QString text();
    void setBtnClicked(bool flag);

signals:
    void clicked();

public slots:

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);

protected:
    void paintWidget(int transparency, QPainter* device);

private:
    bool m_enterFlag;
    bool m_pressedFlag;
    TEXTDIRECTION m_textDirection;
    QString m_text;
    int m_fontHeight;
    int m_fontWidth;
    int m_textX;    //记录text的位置
    int m_textY;
    int m_textW;    //记录text宽度
    int m_textH;
};

#endif // MYBUTTON_H
