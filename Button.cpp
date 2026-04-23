#include "Button.h"
#include "Constants.h"
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QFont>

Button::Button(QString name, QColor bgColor, int fontSize, qreal width, qreal height,
               qreal x, qreal y, QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{
    baseColor = bgColor;
    currentColor = bgColor;
    currentColor.setAlphaF(0.85);

    // Draw rounded rect
    setRect(x, y, width, height);
    QPen pen(QColor(255, 255, 255, 25));
    pen.setWidth(1);
    setPen(pen);
    applyStyle();

    // Draw the text
    QFont f("Segoe UI", fontSize);
    f.setWeight(QFont::Medium);

    text = new QGraphicsTextItem(name, this);
    int xPos = static_cast<int>((width - text->boundingRect().width()) / 2 - fontSize / 2);
    int yPos = static_cast<int>((height - text->boundingRect().height()) / 2 - fontSize / 2);
    text->setPos(xPos, yPos);
    text->setFont(f);
    text->setDefaultTextColor(Colors::textPrimary());

    setAcceptHoverEvents(true);
}

void Button::applyStyle()
{
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(currentColor);
    setBrush(brush);
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (toggleMode) {
        toggled = !toggled;
        if (toggled) {
            currentColor = Colors::accentBlue();
            currentColor.setAlphaF(0.9);
        } else {
            currentColor = baseColor;
            currentColor.setAlphaF(0.85);
        }
        applyStyle();
    }
    emit clicked();
}

void Button::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    hovered = true;
    if (toggleMode && toggled) {
        currentColor = Colors::accentBlue().lighter(120);
    } else {
        currentColor = baseColor.lighter(130);
    }
    currentColor.setAlphaF(1.0);
    applyStyle();
}

void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    hovered = false;
    if (toggleMode && toggled) {
        currentColor = Colors::accentBlue();
        currentColor.setAlphaF(0.9);
    } else {
        currentColor = baseColor;
        currentColor.setAlphaF(0.85);
    }
    applyStyle();
}

void Button::setText(QString txt)
{
    text->setPlainText(txt);
}

void Button::setColor(QColor bgColor)
{
    baseColor = bgColor;
    currentColor = bgColor;
    currentColor.setAlphaF(0.85);
    applyStyle();
}

void Button::setToggleMode(bool enabled)
{
    toggleMode = enabled;
}

void Button::setToggled(bool on)
{
    toggled = on;
    if (toggled) {
        currentColor = Colors::accentBlue();
        currentColor.setAlphaF(0.9);
    } else {
        currentColor = baseColor;
        currentColor.setAlphaF(0.85);
    }
    applyStyle();
}
