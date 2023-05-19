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
    float x;
    float y;
    SpawnOption* spawnOption;
public slots:
    void move();
};

#endif // VEHICLE_H
