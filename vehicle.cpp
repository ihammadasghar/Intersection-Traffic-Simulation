#include "Vehicle.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <stdlib.h>
#include "Simulation.h"

extern Simulation * simulation;

Vehicle::Vehicle(QGraphicsItem *parent): QObject(), QGraphicsPixmapItem(parent){
    int random_number = rand() % 600;
    setPos(random_number,0);

    setPixmap(QPixmap(":static/images/carRed.jpg"));
    setTransformOriginPoint(50,50);
    setRotation(180);

    QTimer * timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));

    // start the timer
    timer->start(50);
}

void Vehicle::move(){
    // move
    setPos(x(),y()+5);

    // destroy vehicle when it goes out of the screen
    if (pos().y() > 600){
        scene()->removeItem(this);
        delete this;
    }
}
