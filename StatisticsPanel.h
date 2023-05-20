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
    void incrementCollisions();
    void incrementTotalCarsSpawned();
    void incrementCarsOnScreen();
    void decrementCarsOnScreen();

};

#endif // STATISTICSPANEL_H
