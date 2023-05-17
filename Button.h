#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

class Button:public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    // constructors
    Button(QString name, QColor bgColor, qreal width, qreal height, qreal x, qreal y, QGraphicsItem* parent=NULL);

    // public methods (events)
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void setText(QString txt);
    void setColor(QColor bgColor);

signals:
    void clicked();
private:
    QGraphicsTextItem* text;
    QColor color;
};

#endif // BUTTON_H
