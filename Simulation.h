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
#include "vehicle.h"
#include "Algorithm.h"

class Simulation: public QGraphicsView{
    Q_OBJECT
public:
    Simulation(QWidget * parent=0);
    QGraphicsScene * scene;
    QGraphicsRectItem* bottomPanel;
    QTimer* timer;
    SettingsPanel* settingsPanel;
    StatisticsPanel* statisticsPanel;
    bool isStarted;
    Button* playButton;
    void decrementCarsOnScreen();
    QGraphicsRectItem* drawPanel(int x, int y, int width, int height, QColor color, double opacity);
    void addSpawnOptions();
    bool destroyCollidingVehicles(Vehicle* car);
    void destroyAllVehicles();
    int mm,ss;
    QGraphicsTextItem* displayTimer;
    QList<Vehicle*> aliveVehicles;
    Algorithm* algorithm;

public slots:
    void startToggle();
    void addVehicle();
    void drawTrafficLights();
    void drawTimer();
    void incrementTimer();
    void resetTimer();
  
private:
    void drawGUI();

};

#endif // SIMULATION_H
