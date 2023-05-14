#ifndef VEHICLE_H
#define VEHICLE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

class Vehicle: public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Vehicle(QGraphicsItem * parent=0);
public slots:
    void move();
};

#endif // VEHICLE_H
