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
#include "Results.h"
#include "vehicle.h"
#include "Algorithm.h"

class Simulation: public QGraphicsView{
    Q_OBJECT
public:
    Simulation(QWidget * parent=0);

    QGraphicsScene * scene;

    // states
    QList<Vehicle*> aliveVehicles;
    bool isStarted;

    // Panels
    QGraphicsRectItem* bottomPanel;
    SettingsPanel* settingsPanel;
    StatisticsPanel* statisticsPanel;
    Results* results;

    // Timer
    QTimer* timer;
    QTimer* vehicleSpawnTimer;
    int seconds;
    QGraphicsTextItem* displayTimer;
    int greenLightDirection:

    // Main buttons
    Button* playButton;
    Button* resultsButton;
    Button* endSimButton;

    Algorithm* algorithm;

    //functions
    void decrementCarsOnScreen();
    QGraphicsRectItem* drawPanel(int x, int y, int width, int height, QColor color, double opacity);
    void addSpawnOptions();
    bool destroyCollidingVehicles(Vehicle* car);
    void destroyAllVehicles();
    void changeGreenLight();




public slots:
    void startToggle();
    void addVehicle();
    void drawTimer();
    void incrementTimer();
    void resetTimer();
    void finishSimulation();
  
private:
    void drawGUI();

};

#endif // SIMULATION_H
