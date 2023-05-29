#include "Vehicle.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include "Simulation.h"
#include <cmath>
#include <QThread>

extern Simulation * simulation;

Vehicle::Vehicle(int speedRangeLowerBound, int speedRangeUpperBound, SpawnOption* spawnOption, QGraphicsItem *parent): QObject(), QGraphicsPixmapItem(parent){
    int pickedCar = (rand() % 4);
    QString carTypes[5] = {"Red", "Green", "Taxi", "Orange"};
    QString filePath = ":static/images/car" + carTypes[pickedCar] + ".png";

    detailsText = NULL;
    fx = 0;
    fy = 0;
    x = spawnOption->initialX;
    y = spawnOption->initialY;
    this->spawnOption = spawnOption;
    rotationAngle = (spawnOption->initialRotation)*3.1415/180;
    setRotation((spawnOption->initialRotation));
    setPos(x,y);
    setPixmap(QPixmap(filePath));
    setTransformOriginPoint(0,0);

    speed = (rand() % (speedRangeUpperBound - speedRangeLowerBound)) + speedRangeLowerBound;
    pps = speed/30; // Pixels Per Sec

    movementTimer = new QTimer(this);
    connect(movementTimer,SIGNAL(timeout()),this,SLOT(move()));

    // start the timer 33 msec for 30 FPS
    movementTimer->start(33);
}

void Vehicle::updateDetails(){
    if(!(simulation->settingsPanel->vehicleDetailsEnabled)) {
        if(detailsText){
            detailsText->setVisible(false);
            detailsText = NULL;
        }
        return;
    }
    if(detailsText==NULL){
        QFont f;
        f.setPointSize(10);
        detailsText = new QGraphicsTextItem("X:" + QString::number(x) + ", Y:" + QString::number(y) + "\nSpeed: " + QString::number(speed) + " KMPH", this);
        detailsText->setPos(0, 20);
        detailsText->setFont(f);
        detailsText->setDefaultTextColor(Qt::yellow);
    }
    detailsText->setPlainText("X:" + QString::number(x) + ", Y:" + QString::number(y) + "\nSpeed: " + QString::number(speed) + " KMPH");
}

void Vehicle::move(){
    // check if simulation is paused
    if(!(simulation->isStarted)){
        movementTimer->stop();
        return;
    }

    // detect collisions
    if(simulation->destroyCollidingVehicles(this)) {
        selfDestruct();
        return;
    }

    auto leftTurn = [](float a, float p1, float h, int fx) {return -a/pow(fx-p1, 2);};
    auto rightTurn = [](float a, float p1, float h, int fx) {return a/pow(fx+(2*h)-p1, 2);};
    // move
    float p1 = 320;
    float a = 1700;
    float h = 30;
    float slope = (spawnOption->turnDirection == "right" ? rightTurn(a, p1, h, fx) : 0);
    if(spawnOption->turnDirection == "left") slope = leftTurn(a, p1, h, fx);

    float angle = atan(slope);

    float changeInX = pps*cos(angle);
    float changeInY = pps*sin(angle);
    fx += changeInX;
    fy += changeInY;

    rotationAngle = ((float)(spawnOption->initialRotation)*3.1415/180) + angle;
    setRotation(rotationAngle*180/3.1415);

    if(spawnOption->initialDirection == "right"){
        x = spawnOption->initialX + (int)fx;
        y = spawnOption->initialY + (int)fy;
    }
    else if(spawnOption->initialDirection == "left"){
        x = spawnOption->initialX - (int)fx;
        y = spawnOption->initialY - (int)fy;
    }
    else if(spawnOption->initialDirection == "up"){
        y = spawnOption->initialY - (int)fx;
        x = spawnOption->initialX + (int)fy;
    }
    else if(spawnOption->initialDirection == "down"){
        y = spawnOption->initialY + (int)fx;
        x = spawnOption->initialX - (int)fy;
    }
    setPos(x, y);

    updateDetails();

    // destroy vehicle when it hits the bottom border
    if (y > 550){
        selfDestruct();
        simulation->statisticsPanel->incrementCollisionsAvoided();
    }

    // destroy vehicle when it hits the top border
    else if (y < 0){
        selfDestruct();
        simulation->statisticsPanel->incrementCollisionsAvoided();
    }

    // destroy vehicle when it hits the left border
    else if (x < 0){
        selfDestruct();
        simulation->statisticsPanel->incrementCollisionsAvoided();
    }

    // destroy vehicle when it hits the right border
    else if (x > 600){
        selfDestruct();
        simulation->statisticsPanel->incrementCollisionsAvoided();
    }

}

void Vehicle::changeSpeed(double acceleration){
    int newSpeed = speed + acceleration;
    if(newSpeed < 80){
        speed = 80;
    }else if(newSpeed > 130) {
        speed = 130;
    }else{
        speed = newSpeed;
    }
    pps = speed/30;

}

void Vehicle::selfDestruct(){
    simulation->statisticsPanel->decrementCarsOnScreen();
    simulation->aliveVehicles.removeOne(this);
    scene()->removeItem(this);
    delete this;
}
