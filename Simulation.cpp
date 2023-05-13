#include <QtMath>
#include <QtWidgets>
#include <QTimer>
#include "Simulation.h"
#include "Vehicle.h"
#include "Button.h"

static constexpr int ScreenWidth = 1024;
static constexpr int ScreenHeight = 768;
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

    QGraphicsRectItem* panel = drawPanel(0, ScreenHeight - (ScreenHeight/4), ScreenWidth, ScreenHeight/4, Qt::green, 0.7);
    scene->addItem(panel);

    Button* playButton = new Button(QString("Play"), Qt::blue, 200, 100, 0, 0, panel);
    int bxPos = this->width()/2 - playButton->boundingRect().width()/2;
    int byPos = 275;
    playButton->setPos(bxPos,byPos);
    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(playButton);

    show();
}

void Simulation::start(){
    for (int i = 0; i < VehicleCount; ++i) {
        Vehicle *vehicle = new Vehicle;
        scene->addItem(vehicle);
    }
}

QGraphicsRectItem* Simulation::drawPanel(int x, int y, int width, int height, QColor color, double opacity){
    QGraphicsRectItem* panel = new QGraphicsRectItem(x,y,width,height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    return panel;
}
