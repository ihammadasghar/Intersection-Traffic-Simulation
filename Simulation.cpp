#include <QtMath>
#include <QtWidgets>
#include <QTimer>
#include "Simulation.h"
#include "Vehicle.h"

static constexpr int ScreenWidth = 680;
static constexpr int ScreenHeight = 540;
static constexpr int VehicleCount = 7;

Simulation::Simulation(QWidget *parent){
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,ScreenWidth,ScreenHeight);
    setBackgroundBrush(QBrush(QImage(":static/images/backgroundNoLights.png")));

    // Visualise
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(ScreenWidth,ScreenHeight);

    for (int i = 0; i < VehicleCount; ++i) {
        Vehicle *vehicle = new Vehicle;
        scene->addItem(vehicle);
    }

    show();
}
