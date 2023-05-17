#ifndef SIMULATION_H
#define SIMULATION_H
#include "Button.h"
#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "SettingsPanel.h"

class Simulation: public QGraphicsView{
    Q_OBJECT
public:
    Simulation(QWidget * parent=0);

    QGraphicsScene * scene;
    QTimer* timer;
    SettingsPanel* settingsPanel;
    bool simulationStarted;
    int collisons;
    int collisonsAvoided;
    int totalCarsSpawned;
    int carsOnScreen;

    Button* playButton;

    QGraphicsTextItem* collisonsDisplay;
    QGraphicsTextItem* collisonsAvoidedDisplay;
    QGraphicsTextItem* totalCarsSpawnedDisplay;
    QGraphicsTextItem* carsOnScreenDisplay;

    void decrementCarsOnScreen();

public slots:
    void start();
    void addVehicle();
    void drawStatistics();
    QGraphicsRectItem* drawPanel(int x, int y, int width, int height, QColor color, double opacity);

private:
    void drawGUI();

};

#endif // SIMULATION_H
