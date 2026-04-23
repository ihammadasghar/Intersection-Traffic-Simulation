#ifndef STATISTICSPANEL_H
#define STATISTICSPANEL_H

#include <QObject>
#include <QGraphicsRectItem>

class QGraphicsTextItem;

class StatisticsPanel : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    StatisticsPanel(int screenWidth, int screenHeight, int btnPadding,
                    QGraphicsItem* parent = nullptr);

    int collisions;
    int collisionsAvoided;
    int totalCarsSpawned;
    int carsOnScreen;
    int stopSignStops;
    int yieldEvents;

    void reset();
    void incrementCollisions();
    void incrementCollisionsAvoided();
    void incrementTotalCarsSpawned();
    void incrementCarsOnScreen();
    void decrementCarsOnScreen();
    void incrementStopSignStops();
    void incrementYieldEvents();

private:
    void updateCollisionPercentage();

    QGraphicsTextItem* collisionPercentageDisplay;
    QGraphicsTextItem* collisionsDisplay;
    QGraphicsTextItem* collisionsAvoidedDisplay;
    QGraphicsTextItem* totalCarsSpawnedDisplay;
    QGraphicsTextItem* carsOnScreenDisplay;
    QGraphicsTextItem* stopSignDisplay;
    QGraphicsTextItem* yieldDisplay;
};

#endif // STATISTICSPANEL_H
