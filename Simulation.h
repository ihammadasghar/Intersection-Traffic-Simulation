#ifndef SIMULATION_H
#define SIMULATION_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>

class Simulation: public QGraphicsView{
public:
    Simulation(QWidget * parent=0);

    QGraphicsScene * scene;

};

#endif // SIMULATION_H
