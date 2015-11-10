#include "MyButton.h"
#include <QDebug>
#include <QPainter>
#include <QLinearGradient>
#include <QFontMetrics>

MyButton::MyButton(QWidget *parent)
    : QWidget(parent),
      m_text("")
{
    this->m_enterFlag = false;
    this->m_pressedFlag = false;

    this->m_textX = 0;
    this->m_textY = 0;
    this->m_textH = 0;
    this->m_textW = 0;
    this->m_fontHeight = 0;
    this->m_fontWidth = 0;

}

void MyButton::paintEvent(QPaintEvent *pev)
{
    QPainter painter(this);
    painter.setPen(QColor("green"));

    int x, y;
    QRect rect4Text;
    if (TOP2BOTTOM == m_textDirection)
    {
        x = 2 * this->m_fontHeight;
        y = 2 * this->m_fontWidth;
        rect4Text.setX(this->m_fontHeight / 2);
        rect4Text.setY(this->m_fontWidth / 2);
        rect4Text.setHeight(this->m_fontWidth);
        rect4Text.setWidth(this->m_fontHeight);
    }
    else if (LEFT2RIGHT == m_textDirection)
    {
        x = 2 * this->m_fontWidth;
        y = 2 * this->m_fontHeight;
        rect4Text.setX(this->m_fontWidth / 2);
        rect4Text.setY(this->m_fontHeight / 2);
        rect4Text.setHeight(this->m_fontHeight);
        rect4Text.setWidth(this->m_fontWidth);
    }
    this->setFixedSize(x, y);
    painter.drawText(rect4Text, m_text);

    if (this->m_enterFlag)
    {
        paintWidget(60, &painter);
    }
    else if (this->m_pressedFlag)
    {
        paintWidget(30, &painter);
    }
    else {
        paintWidget(100, &painter);
    }
}

void MyButton::mousePressEvent(QMouseEvent *mev)
{
    this->m_pressedFlag = true;
    emit clicked();
}

void MyButton::enterEvent(QEvent *ev)
{
    if (!m_pressedFlag)
        this->m_enterFlag = true;
    this->update();
}

void MyButton::leaveEvent(QEvent *)
{
    this->m_enterFlag = false;
    this->update();
}

void MyButton::paintWidget(int transparency, QPainter *device)
{
    QPen pen(Qt::NoBrush, 1);
    device->setPen(pen);

    QLinearGradient linearGradient(this->rect().topLeft(), this->rect().bottomRight());
    linearGradient.setColorAt(0, QColor(152, 251, 152, transparency));

    QBrush brush(linearGradient);
    device->setBrush(brush);
    device->drawRoundRect(this->rect(), 2, 2);
}

void MyButton::setText(const QString text, TEXTDIRECTION textDirection)
{

    QFont font = this->font();
    QFontMetrics fontMet(font);

    m_text = "";

    if (TOP2BOTTOM == textDirection)
    {
        int len = text.length();
        int i =0;
        for (i=0; i < len; ++i)
        {
            m_text.append(text[i]);
            m_text.append("\n");
        }
    }
    else
    {
        m_text = text;
    }

    this->m_fontWidth = fontMet.width(m_text);
    this->m_fontHeight = fontMet.height();
    this->m_textDirection = textDirection;




    this->update();
}

const QString MyButton::text()
{
    return m_text;
}


void MyButton::setBtnClicked(bool flag)
{
    this->m_pressedFlag = flag;
    this->update();
}

MyButton::~MyButton()
{

}

