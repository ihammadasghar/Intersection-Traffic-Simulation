#ifndef STATISTICSPANEL_H
#define STATISTICSPANEL_H

#include <QObject>
#include <QGraphicsItem>

class StatisticsPanel: public QObject,public QGraphicsRectItem{
    Q_OBJECT
public:
    StatisticsPanel(int screenWidth, int screenHeight, int btnPadding, QGraphicsItem * parent=NULL);
    int collisions;
    int collisionsAvoided;
    int totalCarsSpawned;
    int carsOnScreen;
    QGraphicsTextItem* collisionsDisplay;
    QGraphicsTextItem* collisionsAvoidedDisplay;
    QGraphicsTextItem* totalCarsSpawnedDisplay;
    QGraphicsTextItem* carsOnScreenDisplay;
    QGraphicsTextItem* collisionPercentageDisplay;
    void reset();
    void updateCollisionPercentage();
    void incrementCollisions();
    void incrementCollisionsAvoided();
    void incrementTotalCarsSpawned();
    void incrementCarsOnScreen();
    void decrementCarsOnScreen();

};

#endif // STATISTICSPANEL_H
