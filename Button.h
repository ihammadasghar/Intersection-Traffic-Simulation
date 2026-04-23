#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QColor>

class QGraphicsTextItem;

class Button : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    Button(QString name, QColor bgColor, int fontSize, qreal width, qreal height,
           qreal x = 0, qreal y = 0, QGraphicsItem* parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void setText(QString txt);
    void setColor(QColor bgColor);

    // Toggle button support
    void setToggleMode(bool enabled);
    bool isToggled() const { return toggled; }
    void setToggled(bool on);

signals:
    void clicked();

private:
    void applyStyle();

    QGraphicsTextItem* text;
    QColor baseColor;
    QColor currentColor;
    bool hovered = false;
    bool toggleMode = false;
    bool toggled = false;
    int borderRadius = 6;
};

#endif // BUTTON_H
