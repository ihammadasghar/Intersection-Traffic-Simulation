#ifndef VEHICLE_H
#define VEHICLE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include "SpawnOption.h"

class Vehicle: public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Vehicle(int speedRangeLowerBound, int speedRangeUpperBound, SpawnOption* spawnOption, QGraphicsItem * parent=0);
    void selfDestruct();
    float pps;
    float fx;
    float fy;
    int x;
    int y;
    float rotationAngle;
    int speed;
    int stoppedForCalls;
    SpawnOption* spawnOption;
    QGraphicsTextItem* detailsText;
    QTimer* movementTimer;
    void updateDetails();
    void changeSpeed(double acceleration);

public slots:
    void move();
};

#endif // VEHICLE_H
