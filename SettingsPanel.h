#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include "Algorithm.h"

class SettingsPanel: public QObject,public QGraphicsRectItem{
    Q_OBJECT
public:
    SettingsPanel(int screenWidth, int screenHeight, int btnPadding, bool initialAlgorithmEnabled, bool initialVehicleDetailsEnabled, int initialVehiclesPerSec, int initialSpeedRangeLowerBound, int initialSpeedRangeUpperBound, int initialTimerLimit, QGraphicsItem * parent=NULL);
    bool simulationStarted;
    bool algorithmEnabled;
    bool vehicleDetailsEnabled;
    bool trafficLightsEnabled;
    int vehiclesPerSec;
    int speedRangeLowerBound;
    int speedRangeUpperBound;
    int timerLimit;


    QGraphicsTextItem* vehiclesPerSecSetting;
    QGraphicsTextItem* vehicleDetailsSetting;
    QGraphicsTextItem* algorithmSetting;
    QGraphicsTextItem* speedRangeSetting;
    QGraphicsTextItem* timerLimitSetting;
    QGraphicsTextItem* trafficLightsSetting;

public slots:
    void toggle();
    void toggleAlgorithm();
    void toggleTrafficLights();
    void toggleVehicleDetails();
    void incrementVehiclesPerSec();
    void decrementVehiclesPerSec();
    void incrementTimerLimit();
    void decrementTimerLimit();
    void incrementSpeedRangeLowerBound();
    void decrementSpeedRangeLowerBound();
    void incrementSpeedRangeUpperBound();
    void decrementSpeedRangeUpperBound();
    void restart();

};

#endif // SETTINGSPANEL_H
