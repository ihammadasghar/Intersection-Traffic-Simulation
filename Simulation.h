#ifndef SIMULATION_H
#define SIMULATION_H

#include <QGraphicsView>
#include <QList>
#include "Direction.h"
#include "Constants.h"

class QGraphicsScene;
class QGraphicsRectItem;
class QGraphicsTextItem;
class QTimer;
class Vehicle;
class Button;
class SettingsPanel;
class StatisticsPanel;
class Results;
class SpawnOption;
class Algorithm;

class Simulation : public QGraphicsView {
    Q_OBJECT
public:
    Simulation(QWidget* parent = nullptr);

    // State
    QList<Vehicle*> aliveVehicles;
    bool isStarted;

    // Panels (public for Vehicle access)
    SettingsPanel* settingsPanel;
    StatisticsPanel* statisticsPanel;

    // Vehicle lifecycle
    void removeVehicle(Vehicle* vehicle);
    void onVehicleExitedRoad(Vehicle* vehicle);
    bool handleCollisions(Vehicle* car);
    void destroyAllVehicles();

public slots:
    void startToggle();
    void addVehicle();
    void incrementTimer();
    void finishSimulation();

private:
    void drawGUI();
    void drawTrafficLights();
    void drawBottomPanel();
    void drawTimerDisplay();
    void drawStatusBar();
    void updateStatusBar();
    void resetTimer();
    void changeGreenLight();
    QString getActiveStrategiesString() const;
    QGraphicsRectItem* createPanel(int x, int y, int width, int height,
                                    QColor color, double opacity);

    QGraphicsScene* scene;
    Algorithm* algorithm;
    Results* results;

    // Traffic lights
    QList<Button*> trafficLightButtons;
    int greenLightIndex;
    static constexpr Direction kDirectionOrder[4] = {
        Direction::Right, Direction::Down, Direction::Left, Direction::Up
    };

    // Spawn
    SpawnOption* spawnOptions[kSpawnOptionCount];
    int previousSpawnOption;

    // Timer
    QTimer* timer;
    QTimer* vehicleSpawnTimer;
    int seconds;
    QGraphicsTextItem* displayTimer;

    // Bottom panel
    QGraphicsRectItem* bottomPanel;
    Button* playButton;
    Button* resultsButton;
    Button* endSimButton;

    // Status bar
    QGraphicsTextItem* statusText;
    QGraphicsRectItem* statusBar;

protected:
    void resizeEvent(QResizeEvent* event) override;
};

#endif // SIMULATION_H
