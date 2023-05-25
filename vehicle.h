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
    int rotationAngle;
    int speed;
    SpawnOption* spawnOption;
    QGraphicsTextItem* detailsText;
    void updateDetails();
    void changeSpeedOverInterval(double acceleration,int interval);

public slots:
    void move();
};

#endif // VEHICLE_H
