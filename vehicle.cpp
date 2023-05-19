#include "Vehicle.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include "Simulation.h"
#include <cmath>

extern Simulation * simulation;

Vehicle::Vehicle(int speedRangeLowerBound, int speedRangeUpperBound, SpawnOption* spawnOption, QGraphicsItem *parent): QObject(), QGraphicsPixmapItem(parent){
    int pickedCar = (rand() % 4);
    QString carTypes[5] = {"Red", "Green", "Taxi", "Orange"};
    QString filePath = ":static/images/car" + carTypes[pickedCar] + ".png";

    x = 0;
    y = 0;
    this->spawnOption = spawnOption;
    setRotation((spawnOption->initialRotation) * 180/3.1415);
    setPos(spawnOption->initialX, spawnOption->initialY);
    setPixmap(QPixmap(filePath));
    setTransformOriginPoint(0,0);

    pps = ((rand() % (speedRangeUpperBound)) + (speedRangeLowerBound))/30; // Pixels Per Sec

    QTimer * timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));

    // start the timer 33 msec for 30 FPS
    timer->start(33);
}

void Vehicle::move(){
    auto straight = [](float a, float p1, float h, int x) {return 0;};
    auto leftTurn = [](float a, float p1, float h, int x) {return -a/pow(x-p1, 2);};
    auto rightTurn = [](float a, float p1, float h, int x) {return a/pow(x+(2*h)-p1, 2);};
    // move
    float p1 = 320;
    float a = 1700;
    float h = 30;
    float slope = (spawnOption->turnDirection == "right" ? rightTurn(a, p1, h, x) : 0);
    if(spawnOption->turnDirection == "left") slope = leftTurn(a, p1, h, x);

    float angle = atan(slope);

    float changeInX = pps*cos(angle);
    float changeInY = pps*sin(angle);
    x += changeInX;
    y += changeInY;

    int finalX, finalY;
    float rotation = (float)(spawnOption->initialRotation) * 180/3.1415;
    rotation = rotation + angle;
    setRotation(rotation*180/3.1415);

    if(spawnOption->initialDirection == "right"){
        finalX = spawnOption->initialX + (int)x;
        finalY = spawnOption->initialY + (int)y;
    }
    else if(spawnOption->initialDirection == "left"){
        finalX = spawnOption->initialX - (int)x;
        finalY = spawnOption->initialY - (int)y;
    }
    else if(spawnOption->initialDirection == "up"){
        finalY = spawnOption->initialY - (int)x;
        finalX = spawnOption->initialX + (int)y;
    }
    else if(spawnOption->initialDirection == "down"){
        finalY = spawnOption->initialY + (int)x;
        finalX = spawnOption->initialX - (int)y;
    }
    setPos(finalX, finalY);

    // destroy vehicle when it hits the bottom border
    if (pos().y() > 600){
        selfDestruct();
    }

    // destroy vehicle when it hits the top border
    else if (pos().y() < 0){
        selfDestruct();
    }

    // destroy vehicle when it hits the left border
    else if (pos().x() < 0){
        selfDestruct();
    }

    // destroy vehicle when it hits the right border
    else if (pos().x() > 600){
        selfDestruct();
    }
}

void Vehicle::selfDestruct(){
    simulation->decrementCarsOnScreen();
    scene()->removeItem(this);
    delete this;
}
