#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

class SettingsPanel: public QObject,public QGraphicsRectItem{
    Q_OBJECT
public:
    SettingsPanel(int screenWidth, int screenHeight, int btnPadding, bool initialTrafficLightsEnabled, bool initialSoundEffectsEnabled, int initialUnitsOfTime, int initialSpeedRangeLowerBound, int initialSpeedRangeUpperBound, QGraphicsItem * parent=NULL);
    bool simulationStarted;
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
    void toggle();
    void toggleTrafficLights();
    void toggleSoundEffects();
    void incrementUnitsOfTime();
    void decrementUnitsOfTime();
    void incrementSpeedRangeLowerBound();
    void decrementSpeedRangeLowerBound();
    void incrementSpeedRangeUpperBound();
    void decrementSpeedRangeUpperBound();

};

#endif // SETTINGSPANEL_H
