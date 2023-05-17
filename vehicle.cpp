#include "Vehicle.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include "Simulation.h"
#include <cmath>

extern Simulation * simulation;

Vehicle::Vehicle(int speedRangeLowerBound, int speedRangeUpperBound, QGraphicsItem *parent): QObject(), QGraphicsPixmapItem(parent){
    int random_number = 400;

    int pickedCar = (rand() % 5) - 1;
    QString carTypes[5] = {"Red", "Green", "Yellow", "Taxi", "Orange"};
    QString filePath = ":static/images/car" + carTypes[pickedCar] + ".png";

    x = 1;
    y = 200;
    setPos((int)x, (int)y);
    setPixmap(QPixmap(filePath));
    setTransformOriginPoint(0,0);

    pps = ((rand() % (speedRangeUpperBound)) + (speedRangeLowerBound))/30; // Pixels Per Sec

    QTimer * timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));

    // start the timer 33 msec for 30 FPS
    timer->start(33);
}

void Vehicle::move(){
    // move
    float p1 = 400;
    float a = 5000;
    float h = 3.5;
    float slope = (a/pow((x)+(2*h)-p1, 2));
    float angle = atan(slope);
    int rot = angle*180/3.1415;
    setRotation(rot);

    float changeInX = pps*cos(angle);
    float changeInY = pps*sin(angle);
    x += changeInX;
    y += changeInY;
    setPos((int)x, (int)y);

    // destroy vehicle when it hits the bottom border
    if (pos().y() > 500){
        selfDestruct();
    }

    // destroy vehicle when it hits the top border
    else if (pos().y() < -100){
        selfDestruct();
    }

    // destroy vehicle when it hits the left border
    else if (pos().x() < -100){
        selfDestruct();
    }

    // destroy vehicle when it hits the right border
    else if (pos().x() > 750){
        selfDestruct();
    }
}

void Vehicle::selfDestruct(){
    simulation->decrementCarsOnScreen();
    scene()->removeItem(this);
    delete this;
}
