#ifndef VEHICLE_H
#define VEHICLE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

class Vehicle: public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Vehicle(int speedRangeLowerBound, int speedRangeUpperBound, QGraphicsItem * parent=0);
    void selfDestruct();
    float pps;
    float x;
    float y;
    int initialRotation;
    int initialX;
    int initialY;
public slots:
    void move();
};

#endif // VEHICLE_H
