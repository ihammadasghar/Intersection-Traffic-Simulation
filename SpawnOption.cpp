#include "SpawnOption.h"

SpawnOption::SpawnOption(int initialX, int initialY, int initialRotation, QString initialDirection, QString turnDirection){
    this->initialX = initialX;
    this->initialY = initialY;
    this->initialRotation = initialRotation;
    this->initialDirection = initialDirection;
    this->turnDirection = turnDirection;
}
