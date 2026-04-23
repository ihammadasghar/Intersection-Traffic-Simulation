#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QObject>

class QTimer;
class Simulation;

class Algorithm : public QObject {
    Q_OBJECT
public:
    Algorithm(Simulation* simulation);
    void run();

private slots:
    void checkIntersectingLines();

private:
    Simulation* simulation;
    QTimer* timer;
};

#endif // ALGORITHM_H
