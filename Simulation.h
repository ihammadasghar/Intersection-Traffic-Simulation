#ifndef SIMULATION_H
#define SIMULATION_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>

class Simulation: public QGraphicsView{
    Q_OBJECT
public:
    Simulation(QWidget * parent=0);

    QGraphicsScene * scene;
    QGraphicsRectItem* settingsPanel;
    bool trafficLightsEnabled;
    bool soundEffectsEnabled;
    int unitsOfTime;
    int speedRangeBottomBound;
    int speedRangeTopBound;

public slots:
    void start();
    void toggleSettingsPanel();
    void toggleTrafficLights();
    void toggleSoundEffects();

private:
    QGraphicsRectItem* drawPanel(int x, int y, int width, int height, QColor color, double opacity);
    void drawGUI();

};

#endif // SIMULATION_H
