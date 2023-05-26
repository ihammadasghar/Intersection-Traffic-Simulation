#include "Button.h"
#include <QGraphicsTextItem>
#include <QBrush>
#include <QFont>

Button::Button(QString name, QColor bgColor, int size, qreal width, qreal height, qreal x, qreal y, QGraphicsItem *parent): QGraphicsRectItem(parent){
    color = bgColor;
    color.setAlphaF(0.7);

    // draw the rect
    setRect(x,y,width,height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    setBrush(brush);

    // draw the text
    QFont f;
    f.setPointSize(size);

    text = new QGraphicsTextItem(name,this);
    //int xPos = ((width - text->boundingRect().width())/2)-(22/2);
    //int yPos = ((height - text->boundingRect().height())/2)-(22/2);

    int xPos = ((width - text->boundingRect().width())/2)-(size/2);
    int yPos = ((height - text->boundingRect().height())/2)-(size/2);
    text->setPos(xPos,yPos);
    text->setFont(f);
    text->setDefaultTextColor(Qt::white);

    // allow responding to hover events
    setAcceptHoverEvents(true);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event){
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    // change color to cyan
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    color.setAlphaF(1);
    brush.setColor(color);
    setBrush(brush);
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    // change color to dark cyan
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    color.setAlphaF(0.7);
    brush.setColor(color);
    setBrush(brush);
}

void Button::setText(QString txt){
    text->setPlainText(txt);
}

void Button::setColor(QColor bgColor){
    color = bgColor;
    color.setAlphaF(0.7);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    setBrush(brush);
    brush.setColor(color);
}
