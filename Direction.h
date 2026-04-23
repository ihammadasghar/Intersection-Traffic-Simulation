#ifndef DIRECTION_H
#define DIRECTION_H

#include <QString>

enum class Direction { Right, Down, Left, Up };
enum class TurnDirection { Left, Right, Straight };

inline QString directionToString(Direction d)
{
    switch (d) {
    case Direction::Right: return "Right";
    case Direction::Down:  return "Down";
    case Direction::Left:  return "Left";
    case Direction::Up:    return "Up";
    }
    return "";
}

#endif // DIRECTION_H
