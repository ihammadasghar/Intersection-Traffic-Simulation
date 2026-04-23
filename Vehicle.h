#ifndef VEHICLE_H
#define VEHICLE_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "SpawnOption.h"

class QTimer;
class QGraphicsTextItem;
class Simulation;

class Vehicle : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Vehicle(int speedRangeLowerBound, int speedRangeUpperBound,
            SpawnOption* spawnOption, Simulation* simulation,
            QGraphicsItem* parent = nullptr);

    float pixelsPerFrame;
    float projectedX;
    float projectedY;
    int posX;
    int posY;
    float rotationAngle;
    int speed;
    int originalSpeed;
    float distanceCovered;
    SpawnOption* spawnOption;
    QGraphicsTextItem* detailsText;
    QTimer* movementTimer;

    void updateDetails();
    void changeSpeed(double acceleration);

    // Safety behavior state
    bool isStopped() const { return stoppedAtSign; }
    bool isInIntersection() const;
    bool isApproachingIntersection() const;

public slots:
    void move();

private:
    void applyStopSign();
    void applySpeedBump();
    void applyYielding();
    void applySpeedLimit();
    void restoreSpeed();

    Simulation* simulation;

    // Stop sign state
    bool stoppedAtSign = false;
    bool hasStoppedAtSign = false;
    int stopTimer = 0;

    // Speed bump state
    bool inSpeedBumpZone = false;

    // Yield state
    bool isYielding = false;

    // Speed limit
    bool inSpeedLimitZone = false;
};

#endif // VEHICLE_H
