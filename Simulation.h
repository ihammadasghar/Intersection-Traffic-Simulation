#ifndef SIMULATION_H
#define SIMULATION_H
#include "Button.h"
#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "SettingsPanel.h"
#include "StatisticsPanel.h"
#include "SpawnOption.h"

class Simulation: public QGraphicsView{
    Q_OBJECT
public:
    Simulation(QWidget * parent=0);
    QGraphicsScene * scene;
    QGraphicsRectItem* bottomPanel;
    QTimer* timer;
    SettingsPanel* settingsPanel;
    StatisticsPanel* statisticsPanel;
    bool simulationStarted;
    Button* playButton;
    void decrementCarsOnScreen();
    QGraphicsRectItem* drawPanel(int x, int y, int width, int height, QColor color, double opacity);
    void addSpawnOptions();

public slots:
    void start();
    void addVehicle();
    void drawTrafficLights();
  
private:
    void drawGUI();

};

#endif // SIMULATION_H
