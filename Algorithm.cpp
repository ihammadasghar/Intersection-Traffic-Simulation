#include "Algorithm.h"
#include "Simulation.h"
#include "Vehicle.h"
#include "Constants.h"
#include <QTimer>
#include <cmath>

Algorithm::Algorithm(Simulation* simulation)
    : QObject()
    , simulation(simulation)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Algorithm::checkIntersectingLines);
}

void Algorithm::run()
{
    timer->start(kFrameIntervalMs);
}

void Algorithm::checkIntersectingLines()
{
    if (!simulation->isStarted) {
        timer->stop();
        return;
    }

    const QList<Vehicle*>& cars = simulation->aliveVehicles;

    for (int i = 0; i < cars.length(); i++) {
        Vehicle* car1 = cars[i];
        for (int j = i + 1; j < cars.length(); j++) {
            Vehicle* car2 = cars[j];

            auto getDistance = [](double x0, double y0, double x1, double y1) {
                return sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
            };

            float halfLine = kProjectionLineLength / 2.0f;

            // Starting points of projected lines (opposite direction from heading)
            double car1StartX = halfLine * cos(car1->rotationAngle + M_PI) + car1->posX;
            double car1StartY = halfLine * sin(car1->rotationAngle + M_PI) + car1->posY;
            double car2StartX = halfLine * cos(car2->rotationAngle + M_PI) + car2->posX;
            double car2StartY = halfLine * sin(car2->rotationAngle + M_PI) + car2->posY;

            // Ending points of projected lines (heading direction)
            double car1EndX = halfLine * cos(car1->rotationAngle) + car1->posX;
            double car1EndY = halfLine * sin(car1->rotationAngle) + car1->posY;
            double car2EndX = halfLine * cos(car2->rotationAngle) + car2->posX;
            double car2EndY = halfLine * sin(car2->rotationAngle) + car2->posY;

            // Line 1 (car1) represented as a1*x + b1*y = c1
            double a1 = car1EndY - car1StartY;
            double b1 = car1StartX - car1EndX;
            double c1 = a1 * car1StartX + b1 * car1StartY;

            // Line 2 (car2) represented as a2*x + b2*y = c2
            double a2 = car2EndY - car2StartY;
            double b2 = car2StartX - car2EndX;
            double c2 = a2 * car2StartX + b2 * car2StartY;

            double determinant = a1 * b2 - a2 * b1;
            if (qFuzzyIsNull(determinant))
                continue; // Lines are parallel

            // Potential collision coordinates
            double collisionX = (b2 * c1 - b1 * c2) / determinant;
            double collisionY = (a1 * c2 - a2 * c1) / determinant;

            // Distances to collision point
            float dist1 = getDistance(car1->posX, car1->posY, collisionX, collisionY);
            float dist2 = getDistance(car2->posX, car2->posY, collisionX, collisionY);

            // Adjust speeds: speed up the closer car, slow down the farther one
            if (dist1 <= dist2) {
                float ratio = dist1 / dist2;
                car1->changeSpeed(kAccelerationPerSec * ratio);
                car2->changeSpeed(-kAccelerationPerSec * ratio);
            } else {
                float ratio = dist2 / dist1;
                car2->changeSpeed(kAccelerationPerSec * ratio);
                car1->changeSpeed(-kAccelerationPerSec * ratio);
            }
        }
    }
}
