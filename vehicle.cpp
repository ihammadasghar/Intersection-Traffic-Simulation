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

    //setPos(x()+5,y());
    //setPos(x()-5,y());
    setPos(x(),y()+5);
    //setPos(x(),y()-5);

    // destroy vehicle when it hits the bottom border
    if (pos().y() > 435){
        scene()->removeItem(this);
        delete this;
    }

    // destroy vehicle when it hits the top border
    else if (pos().y() < -100){
        scene()->removeItem(this);
        delete this;
    }

    // destroy vehicle when it hits the left border
    else if (pos().x() < -100){
        scene()->removeItem(this);
        delete this;
    }

    // destroy vehicle when it hits the right border
    else if (pos().x() > 750){
        scene()->removeItem(this);
        delete this;
    }
}

