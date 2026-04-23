#include <QApplication>
#include "Simulation.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    Simulation simulation;
    simulation.show();

    return app.exec();
}
