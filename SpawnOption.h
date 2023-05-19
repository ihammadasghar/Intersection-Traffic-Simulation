#ifndef SPAWNOPTION_H
#define SPAWNOPTION_H

#include <QString>

class SpawnOption{
public:
    SpawnOption(int initialX, int initialY, int initialRotation, QString initialDirection, QString turnDirection);
    int initialX;
    int initialY;
    int initialRotation;
    QString initialDirection;
    QString turnDirection;
};

#endif // SPAWNOPTION_H
