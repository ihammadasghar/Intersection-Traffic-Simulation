#ifndef SIMULATION_H
#define SIMULATION_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

class Simulation: public QGraphicsView{
    Q_OBJECT
public:
    Simulation(QWidget * parent=0);

    QGraphicsScene * scene;
    QGraphicsRectItem* settingsPanel;
    bool trafficLightsEnabled;
    bool soundEffectsEnabled;
    int unitsOfTime;
    int speedRangeLowerBound;
    int speedRangeUpperBound;
    QGraphicsTextItem* unitsOfTimeSetting;
    QGraphicsTextItem* soundEffectsSetting;
    QGraphicsTextItem* trafficLightSetting;
    QGraphicsTextItem* speedRangeSetting;

public slots:
    void start();
    void addVehicle();
    void toggleSettingsPanel();
    void toggleTrafficLights();
    void toggleSoundEffects();
    void incrementUnitsOfTime();
    void decrementUnitsOfTime();
    void incrementSpeedRangeLowerBound();
    void decrementSpeedRangeLowerBound();
    void incrementSpeedRangeUpperBound();
    void decrementSpeedRangeUpperBound();

private:
    QGraphicsRectItem* drawPanel(int x, int y, int width, int height, QColor color, double opacity);
    void drawGUI();

};

#endif // SIMULATION_H
