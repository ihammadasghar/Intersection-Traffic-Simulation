#ifndef SIMULATION_H
#define SIMULATION_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>

class Simulation: public QGraphicsView{
    Q_OBJECT
public:
    Simulation(QWidget * parent=0);

    QGraphicsScene * scene;

public slots:
    void start();

private:
    QGraphicsRectItem* drawPanel(int x, int y, int width, int height, QColor color, double opacity);

};

#endif // SIMULATION_H
