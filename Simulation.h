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

class Simulation: public QGraphicsView{
    Q_OBJECT
public:
    Simulation(QWidget * parent=0);
    QGraphicsScene * scene;
    QGraphicsRectItem* bottomPanel;
    QTimer* timer;
    SettingsPanel* settingsPanel;
    StatisticsPanel* statisticsPanel;
    Results* results;

    bool isStarted;
    Button* playButton;
    Button* resultsButton;
    Button* endSimButton;
    void decrementCarsOnScreen();
    QGraphicsRectItem* drawPanel(int x, int y, int width, int height, QColor color, double opacity);
    void addSpawnOptions();
    void destroyCollidingVehicles(QList<QGraphicsItem *> list);
    void destroyAllVehicles();
    int mm,ss;
    QGraphicsTextItem* displayTimer;
    QList<Vehicle*> aliveVehicles;

public slots:
    void startToggle();
    void addVehicle();
    void drawTimer();
    void incrementTimer();
    void resetTimer();
  
private:
    void drawGUI();

};

#endif // SIMULATION_H
