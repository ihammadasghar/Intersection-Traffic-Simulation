#ifndef SPAWNOPTION_H
#define SPAWNOPTION_H

#include "Direction.h"

class SpawnOption {
public:
    SpawnOption(int initialX, int initialY, int initialRotation,
                Direction initialDirection, TurnDirection turnDirection);

    int initialX;
    int initialY;
    int initialRotation;
    Direction initialDirection;
    TurnDirection turnDirection;
};

#endif // SPAWNOPTION_H
