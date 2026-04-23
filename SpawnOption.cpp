#include "SpawnOption.h"

SpawnOption::SpawnOption(int initialX, int initialY, int initialRotation,
                         Direction initialDirection, TurnDirection turnDirection)
    : initialX(initialX)
    , initialY(initialY)
    , initialRotation(initialRotation)
    , initialDirection(initialDirection)
    , turnDirection(turnDirection)
{
}
