#include <QApplication>
#include "Simulation.h"

Simulation * simulation;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    simulation = new Simulation();
    simulation->show();

    return app.exec();
}
