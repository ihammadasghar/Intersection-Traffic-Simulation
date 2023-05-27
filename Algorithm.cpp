#include "Algorithm.h"
#include "Simulation.h"
#include <QTimer>

extern Simulation * simulation;

Algorithm::Algorithm(QString name): QObject(){
    this->name = name;
}

void Algorithm::run(){
    QTimer * timer2 = new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(intersectingLines()));
    timer2->start(33);
}

void Algorithm::intersectingLines(){
    if(!(simulation->isStarted)) return;

    float maxDistance = 800; // max distance between two cars to avoid possible collision
    float lineLength = 600;
    double accelerationPerSec = 10;

    QList<Vehicle*> cars = simulation->aliveVehicles;
    Vehicle* car1;
    Vehicle* car2;


    for(int i = 0; i < cars.length(); i++){
        car1 = cars[i];
        for(int j = i + 1; j < cars.length(); j++){
            car2 = cars[j];
            auto getDistance = [](double x0, double y0, double x1, double y1){ return pow((pow(x1 - x0,2) +pow(y1 - y0, 2)),0.5);};
            float distance = getDistance(car1->x, car1->y, car2->x, car2->y);

            // check distance
            if(distance>maxDistance) continue;

            // starting points of the lines
            double AFIRST = ((lineLength/2) * cos((car1->rotationAngle)+3.1415)) + car1->x;
            double ASECOND = ((lineLength/2) * sin(car1->rotationAngle)+3.1415) + car1->y;
            double CFIRST = ((lineLength/2) * cos(car2->rotationAngle)+3.1415) + car2->x;
            double CSECOND = ((lineLength/2) * sin(car2->rotationAngle)+3.1415) + car2->y;

            // ending points of the lines
            double BFIRST = ((lineLength/2) * cos(car1->rotationAngle)) + car1->x;
            double BSECOND = ((lineLength/2) * sin(car1->rotationAngle)) + car1->y;
            double DFIRST = ((lineLength/2) * cos(car2->rotationAngle)) + car2->x;
            double DSECOND = ((lineLength/2) * sin(car2->rotationAngle)) + car2->y;

            // ending points of the lines
            // Line AB represented as a1x + b1y = c1
            double a1 = BSECOND - ASECOND;
            double b1 = AFIRST - BFIRST;
            double c1 = a1*(AFIRST) + b1*(ASECOND);

            // Line CD represented as a2x + b2y = c2
            double a2 = DSECOND - CSECOND;
            double b2 = CFIRST - DFIRST;
            double c2 = a2*(CFIRST)+ b2*(CSECOND);

            double determinant = a1*b2 - a2*b1;

            // possible collision cordinates
            double x = (b2*c1 - b1*c2)/determinant;
            double y = (a1*c2 - a2*c1)/determinant;

            // distances to collision
            float car1ToCollision = getDistance(car1->x, car1->y, x, y);
            float car2ToCollision = getDistance(car2->x, car2->y, x, y);

            if(car1ToCollision <= car2ToCollision){
                // speed up car 1 and slow down car 2 to avoid collision
                car1->changeSpeed(accelerationPerSec);
                car2->changeSpeed(-accelerationPerSec/2);
            }
            else {
                // speed up car 2 and slow down car 1 to avoid collision
                car2->changeSpeed(accelerationPerSec);
                car1->changeSpeed(-accelerationPerSec/2);
            }


        }
    }
}

