#ifndef STATISTICSPANEL_H
#define STATISTICSPANEL_H

#include <QObject>
#include <QGraphicsItem>

class StatisticsPanel: public QObject,public QGraphicsRectItem{
    Q_OBJECT
public:
    StatisticsPanel(int screenWidth, int screenHeight, int btnPadding, QGraphicsItem * parent=NULL);
    int collisons;
    int collisonsAvoided;
    int totalCarsSpawned;
    int carsOnScreen;
    QGraphicsTextItem* collisonsDisplay;
    QGraphicsTextItem* collisonsAvoidedDisplay;
    QGraphicsTextItem* totalCarsSpawnedDisplay;
    QGraphicsTextItem* carsOnScreenDisplay;
    void reset();
    void incrementTotalCarsSpawned();
    void incrementCarsOnScreen();
    void decrementCarsOnScreen();

};

#endif // STATISTICSPANEL_H
