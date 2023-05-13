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

    drawGUI();

    show();
}

void Simulation::start(){
    for (int i = 0; i < VehicleCount; ++i) {
        Vehicle *vehicle = new Vehicle;
        scene->addItem(vehicle);
    }
}

void Simulation::toggleSettingsPanel(){
    settingsPanel->setVisible(!settingsPanel->isVisible());
}

void Simulation::drawGUI(){
    // Settings panel toggle button
    Button* settingsButton = new Button(QString("Settings"), Qt::yellow, 50, 50, 0, 0);
    settingsButton->setPos(ScreenWidth - (ScreenWidth/10), (ScreenHeight/10) - 50);
    connect(settingsButton,SIGNAL(clicked()),this,SLOT(toggleSettingsPanel()));
    scene->addItem(settingsButton);

    // settingsPanel panel
    settingsPanel = drawPanel(ScreenWidth/10, ScreenHeight/10, ScreenWidth - (ScreenWidth/5), ScreenHeight - (ScreenHeight/5) - (ScreenHeight/4), Qt::green, 0.7);
    settingsPanel->setVisible(false);
    scene->addItem(settingsPanel);

    // Bottom panel
    QGraphicsRectItem* panel = drawPanel(0, ScreenHeight - (ScreenHeight/4), ScreenWidth, ScreenHeight/4, Qt::green, 0.7);

    // Start/Stop button in Bottom Panel
    Button* playButton = new Button(QString("Play"), Qt::blue, 200, 100, 0, 0, panel);
    int bxPos = (ScreenWidth/10);
    int byPos = ScreenHeight - (ScreenHeight/10) - 100;
    playButton->setPos(bxPos,byPos);
    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));

    scene->addItem(panel);
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
