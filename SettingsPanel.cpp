#include "SettingsPanel.h"
#include "Simulation.h"

extern Simulation * simulation;

SettingsPanel::SettingsPanel(int screenWidth, int screenHeight, int btnPadding, bool initialTrafficLightsEnabled, bool initialSoundEffectsEnabled, int initialUnitsOfTime, int initialSpeedRangeLowerBound, int initialSpeedRangeUpperBound, QGraphicsItem *parent): QObject(), QGraphicsRectItem(parent){
    trafficLightsEnabled = initialTrafficLightsEnabled;
    soundEffectsEnabled = initialSoundEffectsEnabled;
    unitsOfTime = initialUnitsOfTime;
    speedRangeLowerBound = initialSpeedRangeLowerBound;
    speedRangeUpperBound = initialSpeedRangeUpperBound;

    // this panel
    int settingsPanelX = screenWidth/10;
    int settingsPanelY = screenHeight/10;
    int settingsPanelW = screenWidth - (screenWidth/5);
    int settingsPanelH = screenHeight - (screenHeight/5) - (screenHeight/4);
    setRect(settingsPanelX,settingsPanelY,settingsPanelW,settingsPanelH);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    setBrush(brush);
    setOpacity(0.7);
    setZValue(2);

    // Add settings to panel
    QFont f;
    f.setPointSize(20);

    int settingX = settingsPanelX + btnPadding;
    int settingY = settingsPanelY + btnPadding;

    // Traffic Light Toggle
    trafficLightSetting = new QGraphicsTextItem(QString("Traffic Lights: ") + QString(trafficLightsEnabled ? "On" : "Off"), this);
    trafficLightSetting->setPos(settingX, settingY);
    trafficLightSetting->setFont(f);
    trafficLightSetting->setDefaultTextColor(Qt::white);

    int trafficLightBtnW = (settingsPanelW/2) - (btnPadding*2);
    int trafficLightBtnH = (settingsPanelH/5) - (btnPadding*2);
    int trafficLightBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int trafficLightBtnY = settingY;
    Button* trafficLightBtn = new Button(QString("O/I"), Qt::green, trafficLightBtnW, trafficLightBtnH, 0, 0, this);
    trafficLightBtn->setPos(trafficLightBtnX, trafficLightBtnY);
    connect(trafficLightBtn,SIGNAL(clicked()),this,SLOT(toggleTrafficLights()));

    // Sound Effects Setting
    settingY += settingsPanelH/5;

    soundEffectsSetting = new QGraphicsTextItem(QString("Sound Effects: ") + QString(soundEffectsEnabled ? "On" : "Off"), this);
    soundEffectsSetting->setPos(settingX, settingY);
    soundEffectsSetting->setFont(f);
    soundEffectsSetting->setDefaultTextColor(Qt::white);


    int soundEffectsBtnW = (settingsPanelW/2) - (btnPadding*2);
    int soundEffectsBtnH = (settingsPanelH/5) - (btnPadding*2);
    int soundEffectsBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int soundEffectsBtnY = settingY;
    Button* soundEffectsBtn = new Button(QString("O/I"), Qt::green, soundEffectsBtnW, soundEffectsBtnH, 0, 0, this);
    soundEffectsBtn->setPos(soundEffectsBtnX, soundEffectsBtnY);
    connect(soundEffectsBtn,SIGNAL(clicked()),this,SLOT(toggleSoundEffects()));

    // Units of Time setting
    settingY += settingsPanelH/5;

    unitsOfTimeSetting = new QGraphicsTextItem(QString("Units Of Time: ") + QString::number(unitsOfTime), this);
    unitsOfTimeSetting->setPos(settingX, settingY);
    unitsOfTimeSetting->setFont(f);
    unitsOfTimeSetting->setDefaultTextColor(Qt::white);

    int unitsOfTimeIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/2;
    int unitsOfTimeIncBtnH = (settingsPanelH/5) - (btnPadding*2);
    int unitsOfTimeIncBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int unitsOfTimeIncBtnY = settingY;
    Button* unitsOfTimeIncBtn = new Button(QString("+"), Qt::green, unitsOfTimeIncBtnW, unitsOfTimeIncBtnH, 0, 0, this);
    unitsOfTimeIncBtn->setPos(unitsOfTimeIncBtnX, unitsOfTimeIncBtnY);
    connect(unitsOfTimeIncBtn,SIGNAL(clicked()),this,SLOT(incrementUnitsOfTime()));

    int unitsOfTimeDecBtnW = unitsOfTimeIncBtnW;
    int unitsOfTimeDecBtnH = (settingsPanelH/5) - (btnPadding*2);
    int unitsOfTimeDecBtnX = unitsOfTimeIncBtnX + unitsOfTimeIncBtnW;
    int unitsOfTimeDecBtnY = unitsOfTimeIncBtnY;
    Button* unitsOfTimeDecBtn = new Button(QString("-"), Qt::red, unitsOfTimeDecBtnW, unitsOfTimeDecBtnH, 0, 0, this);
    unitsOfTimeDecBtn->setPos(unitsOfTimeDecBtnX, unitsOfTimeDecBtnY);
    connect(unitsOfTimeDecBtn,SIGNAL(clicked()),this,SLOT(decrementUnitsOfTime()));

    // Speed Range setting
    settingY += settingsPanelH/5;
    speedRangeSetting = new QGraphicsTextItem(QString("Speed: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound), this);

    speedRangeSetting->setPos(settingX, settingY);
    speedRangeSetting->setFont(f);
    speedRangeSetting->setDefaultTextColor(Qt::white);

    int speedRangeLowerBoundIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeLowerBoundIncBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeLowerBoundIncBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int speedRangeLowerBoundIncBtnY = settingY;
    Button* speedRangeLowerBoundIncBtn = new Button(QString("+"), Qt::green, speedRangeLowerBoundIncBtnW, speedRangeLowerBoundIncBtnH, 0, 0, this);
    speedRangeLowerBoundIncBtn->setPos(speedRangeLowerBoundIncBtnX, speedRangeLowerBoundIncBtnY);
    connect(speedRangeLowerBoundIncBtn,SIGNAL(clicked()),this,SLOT(incrementSpeedRangeLowerBound()));

    int speedRangeLowerBoundDecBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeLowerBoundDecBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeLowerBoundDecBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + speedRangeLowerBoundIncBtnW;
    int speedRangeLowerBoundDecBtnY = settingY;
    Button* speedRangeLowerBoundDecBtn = new Button(QString("-"), Qt::red, speedRangeLowerBoundDecBtnW, speedRangeLowerBoundDecBtnH, 0, 0, this);
    speedRangeLowerBoundDecBtn->setPos(speedRangeLowerBoundDecBtnX, speedRangeLowerBoundDecBtnY);
    connect(speedRangeLowerBoundDecBtn,SIGNAL(clicked()),this,SLOT(decrementSpeedRangeLowerBound()));

    int speedRangeUpperBoundIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeUpperBoundIncBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeUpperBoundIncBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + (speedRangeLowerBoundIncBtnW*2);
    int speedRangeUpperBoundIncBtnY = settingY;
    Button* speedRangeUpperBoundIncBtn = new Button(QString(" +"), Qt::green, speedRangeUpperBoundIncBtnW, speedRangeUpperBoundIncBtnH, 0, 0, this);
    speedRangeUpperBoundIncBtn->setPos(speedRangeUpperBoundIncBtnX, speedRangeUpperBoundIncBtnY);
    connect(speedRangeUpperBoundIncBtn,SIGNAL(clicked()),this,SLOT(incrementSpeedRangeUpperBound()));

    int speedRangeUpperBoundDecBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeUpperBoundDecBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeUpperBoundDecBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + (speedRangeLowerBoundIncBtnW*3);
    int speedRangeUpperBoundDecBtnY = settingY;
    Button* speedRangeUpperBoundDecBtn = new Button(QString("-"), Qt::red, speedRangeUpperBoundDecBtnW, speedRangeUpperBoundDecBtnH, 0, 0, this);
    speedRangeUpperBoundDecBtn->setPos(speedRangeUpperBoundDecBtnX, speedRangeUpperBoundDecBtnY);
    connect(speedRangeUpperBoundDecBtn,SIGNAL(clicked()),this,SLOT(decrementSpeedRangeUpperBound()));

    // Apply Changes Button
    int applyChangesBtnW = settingsPanelW - (settingsPanelW/2);
    int applyChangesBtnH = settingsPanelH/5;
    int applyChangesBtnX = settingsPanelX + (settingsPanelW/4);
    int applyChangesBtnY = settingsPanelY + settingsPanelH - applyChangesBtnH - btnPadding;
    Button* applyChangesBtn = new Button(QString("Apply"), Qt::yellow, applyChangesBtnW, applyChangesBtnH, 0, 0, this);
    applyChangesBtn->setPos(applyChangesBtnX, applyChangesBtnY);
    connect(applyChangesBtn,SIGNAL(clicked()),this,SLOT(toggle()));

    this->setVisible(false);
}

void SettingsPanel::toggle(){
    setVisible(!isVisible());
}

void SettingsPanel::toggleTrafficLights(){
    trafficLightsEnabled = !trafficLightsEnabled;
    trafficLightSetting->setPlainText(QString("Traffic Lights: ") + QString(trafficLightsEnabled ? "On" : "Off"));
}

void SettingsPanel::toggleSoundEffects(){
    soundEffectsEnabled = !soundEffectsEnabled;
    soundEffectsSetting->setPlainText(QString("Sound Effects: ") + QString(soundEffectsEnabled ? "On" : "Off"));
}

void SettingsPanel::incrementUnitsOfTime(){
    unitsOfTime++;
    unitsOfTimeSetting->setPlainText(QString("Units Of Time: ") + QString::number(unitsOfTime));
}

void SettingsPanel::decrementUnitsOfTime(){
    unitsOfTime--;
    unitsOfTimeSetting->setPlainText(QString("Units Of Time: ") + QString::number(unitsOfTime));
}

void SettingsPanel::incrementSpeedRangeLowerBound(){
    // Can go higher than Upper bound
    if(speedRangeUpperBound >= speedRangeLowerBound+1){
        speedRangeLowerBound++;
        speedRangeSetting->setPlainText(QString("Speed: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
    }
}

void SettingsPanel::decrementSpeedRangeLowerBound(){
    speedRangeLowerBound--;
    speedRangeSetting->setPlainText(QString("Speed: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
}

void SettingsPanel::incrementSpeedRangeUpperBound(){
    speedRangeUpperBound++;
    speedRangeSetting->setPlainText(QString("Speed: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
}

void SettingsPanel::decrementSpeedRangeUpperBound(){
    // Cant go lower than lower bound
    if(speedRangeLowerBound <= speedRangeUpperBound-1){
        speedRangeUpperBound--;
        speedRangeSetting->setPlainText(QString("Speed: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
    }
}
