#include "StatisticsPanel.h"
#include "Constants.h"
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QFont>

StatisticsPanel::StatisticsPanel(int screenWidth, int screenHeight, int btnPadding,
                                 QGraphicsItem* parent)
    : QObject()
    , QGraphicsRectItem(parent)
{
    collisions = 0;
    collisionsAvoided = 0;
    carsOnScreen = 0;
    totalCarsSpawned = 0;
    stopSignStops = 0;
    yieldEvents = 0;

    int panelX = 190;
    int panelY = screenHeight - (screenHeight / 4) + 8;
    int panelW = screenWidth - 200;
    int panelH = screenHeight / 4 - 16;
    setRect(panelX, panelY, panelW, panelH);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Colors::panelMedium());
    setBrush(brush);
    QPen pen(QColor(255, 255, 255, 20));
    pen.setWidth(1);
    setPen(pen);
    setOpacity(0.95);
    setZValue(2);

    // Header
    QFont headerFont("Segoe UI", 12);
    headerFont.setWeight(QFont::Bold);
    QGraphicsTextItem* header = new QGraphicsTextItem("Live Statistics", this);
    header->setPos(panelX + btnPadding, panelY + 2);
    header->setFont(headerFont);
    header->setDefaultTextColor(Colors::accentTeal());

    QFont f("Segoe UI", 11);
    int startY = panelY + 24;
    int lineH = 22;
    int leftX = panelX + btnPadding;
    int rightX = panelX + panelW / 2 + btnPadding;

    // Left column
    collisionPercentageDisplay = new QGraphicsTextItem("Collision %: 0%", this);
    collisionPercentageDisplay->setPos(leftX, startY);
    collisionPercentageDisplay->setFont(f);
    collisionPercentageDisplay->setDefaultTextColor(Colors::accentRed());

    collisionsDisplay = new QGraphicsTextItem("Collisions: 0", this);
    collisionsDisplay->setPos(leftX, startY + lineH);
    collisionsDisplay->setFont(f);
    collisionsDisplay->setDefaultTextColor(Colors::textPrimary());

    collisionsAvoidedDisplay = new QGraphicsTextItem("Avoided: 0", this);
    collisionsAvoidedDisplay->setPos(leftX, startY + lineH * 2);
    collisionsAvoidedDisplay->setFont(f);
    collisionsAvoidedDisplay->setDefaultTextColor(Colors::accentGreen());

    totalCarsSpawnedDisplay = new QGraphicsTextItem("Spawned: 0", this);
    totalCarsSpawnedDisplay->setPos(leftX, startY + lineH * 3);
    totalCarsSpawnedDisplay->setFont(f);
    totalCarsSpawnedDisplay->setDefaultTextColor(Colors::textPrimary());

    // Right column
    carsOnScreenDisplay = new QGraphicsTextItem("On Screen: 0", this);
    carsOnScreenDisplay->setPos(rightX, startY);
    carsOnScreenDisplay->setFont(f);
    carsOnScreenDisplay->setDefaultTextColor(Colors::textPrimary());

    stopSignDisplay = new QGraphicsTextItem("Stop Sign Stops: 0", this);
    stopSignDisplay->setPos(rightX, startY + lineH);
    stopSignDisplay->setFont(f);
    stopSignDisplay->setDefaultTextColor(Colors::accentYellow());

    yieldDisplay = new QGraphicsTextItem("Yield Events: 0", this);
    yieldDisplay->setPos(rightX, startY + lineH * 2);
    yieldDisplay->setFont(f);
    yieldDisplay->setDefaultTextColor(Colors::accentOrange());
}

void StatisticsPanel::reset()
{
    collisions = 0;
    collisionsAvoided = 0;
    carsOnScreen = 0;
    totalCarsSpawned = 0;
    stopSignStops = 0;
    yieldEvents = 0;

    collisionPercentageDisplay->setPlainText("Collision %: 0%");
    collisionsDisplay->setPlainText("Collisions: 0");
    collisionsAvoidedDisplay->setPlainText("Avoided: 0");
    totalCarsSpawnedDisplay->setPlainText("Spawned: 0");
    carsOnScreenDisplay->setPlainText("On Screen: 0");
    stopSignDisplay->setPlainText("Stop Sign Stops: 0");
    yieldDisplay->setPlainText("Yield Events: 0");
}

void StatisticsPanel::updateCollisionPercentage()
{
    if (totalCarsSpawned == 0) return;
    int percentage = static_cast<int>((static_cast<float>(collisions) / totalCarsSpawned) * 100);
    collisionPercentageDisplay->setPlainText(
        "Collision %: " + QString::number(percentage) + "%");
}

void StatisticsPanel::incrementCollisionsAvoided()
{
    collisionsAvoided++;
    collisionsAvoidedDisplay->setPlainText("Avoided: " + QString::number(collisionsAvoided));
}

void StatisticsPanel::incrementCollisions()
{
    collisions++;
    collisionsDisplay->setPlainText("Collisions: " + QString::number(collisions));
    updateCollisionPercentage();
}

void StatisticsPanel::incrementTotalCarsSpawned()
{
    totalCarsSpawned++;
    totalCarsSpawnedDisplay->setPlainText("Spawned: " + QString::number(totalCarsSpawned));
    updateCollisionPercentage();
}

void StatisticsPanel::incrementCarsOnScreen()
{
    carsOnScreen++;
    carsOnScreenDisplay->setPlainText("On Screen: " + QString::number(carsOnScreen));
}

void StatisticsPanel::decrementCarsOnScreen()
{
    if (carsOnScreen == 0) return;
    carsOnScreen--;
    carsOnScreenDisplay->setPlainText("On Screen: " + QString::number(carsOnScreen));
}

void StatisticsPanel::incrementStopSignStops()
{
    stopSignStops++;
    stopSignDisplay->setPlainText("Stop Sign Stops: " + QString::number(stopSignStops));
}

void StatisticsPanel::incrementYieldEvents()
{
    yieldEvents++;
    yieldDisplay->setPlainText("Yield Events: " + QString::number(yieldEvents));
}
