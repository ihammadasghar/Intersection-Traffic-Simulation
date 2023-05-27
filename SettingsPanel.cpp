#include "SettingsPanel.h"
#include "Simulation.h"

extern Simulation * simulation;

SettingsPanel::SettingsPanel(int screenWidth, int screenHeight, int btnPadding, bool initialAlgorithmEnabled, bool initialSoundEffectsEnabled, int initialVehiclesPerSec, int initialSpeedRangeLowerBound, int initialSpeedRangeUpperBound, Algorithm* algorithm, QGraphicsItem *parent): QObject(), QGraphicsRectItem(parent){
    algorithmEnabled = initialAlgorithmEnabled;
    soundEffectsEnabled = initialSoundEffectsEnabled;
    vehiclesPerSec = initialVehiclesPerSec;
    speedRangeLowerBound = initialSpeedRangeLowerBound;
    speedRangeUpperBound = initialSpeedRangeUpperBound;
    this->alogrithm = algorithm;

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

    // Algorithm Toggle
    algorithmSetting = new QGraphicsTextItem(QString("Algorithm: On"), this);
    algorithmSetting->setPos(settingX, settingY);
    algorithmSetting->setFont(f);
    algorithmSetting->setDefaultTextColor(Qt::white);

    int algorithmBtnW = (settingsPanelW/2) - (btnPadding*2);
    int algorithmBtnH = (settingsPanelH/5) - (btnPadding*2);
    int algorithmBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int algorithmBtnY = settingY;
    Button* algorithmBtn = new Button(QString("O/I"), Qt::green, 20, algorithmBtnW, algorithmBtnH, 0, 0, this);
    algorithmBtn->setPos(algorithmBtnX, algorithmBtnY);
    connect(algorithmBtn,SIGNAL(clicked()),this,SLOT(toggleAlgorithm()));

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
    Button* soundEffectsBtn = new Button(QString("O/I"), Qt::green,20, soundEffectsBtnW, soundEffectsBtnH, 0, 0, this);
    soundEffectsBtn->setPos(soundEffectsBtnX, soundEffectsBtnY);
    connect(soundEffectsBtn,SIGNAL(clicked()),this,SLOT(toggleSoundEffects()));

    // Units of Time setting
    settingY += settingsPanelH/5;

    vehiclesPerSecSetting = new QGraphicsTextItem(QString("Cars Per Second: ") + QString::number(vehiclesPerSec), this);
    vehiclesPerSecSetting->setPos(settingX, settingY);
    vehiclesPerSecSetting->setFont(f);
    vehiclesPerSecSetting->setDefaultTextColor(Qt::white);

    int vehiclesPerSecIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/2;
    int vehiclesPerSecIncBtnH = (settingsPanelH/5) - (btnPadding*2);
    int vehiclesPerSecIncBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int vehiclesPerSecIncBtnY = settingY;
    Button* vehiclesPerSecIncBtn = new Button(QString("+"), Qt::green,20, vehiclesPerSecIncBtnW, vehiclesPerSecIncBtnH, 0, 0, this);
    vehiclesPerSecIncBtn->setPos(vehiclesPerSecIncBtnX, vehiclesPerSecIncBtnY);
    connect(vehiclesPerSecIncBtn,SIGNAL(clicked()),this,SLOT(incrementVehiclesPerSec()));

    int vehiclesPerSecDecBtnW = vehiclesPerSecIncBtnW;
    int vehiclesPerSecDecBtnH = (settingsPanelH/5) - (btnPadding*2);
    int vehiclesPerSecDecBtnX = vehiclesPerSecIncBtnX + vehiclesPerSecIncBtnW;
    int vehiclesPerSecDecBtnY = vehiclesPerSecIncBtnY;
    Button* vehiclesPerSecDecBtn = new Button(QString("-"), Qt::red,20, vehiclesPerSecDecBtnW, vehiclesPerSecDecBtnH, 0, 0, this);
    vehiclesPerSecDecBtn->setPos(vehiclesPerSecDecBtnX, vehiclesPerSecDecBtnY);
    connect(vehiclesPerSecDecBtn,SIGNAL(clicked()),this,SLOT(decrementVehiclesPerSec()));

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
    Button* speedRangeLowerBoundIncBtn = new Button(QString("+"), Qt::green,20, speedRangeLowerBoundIncBtnW, speedRangeLowerBoundIncBtnH, 0, 0, this);
    speedRangeLowerBoundIncBtn->setPos(speedRangeLowerBoundIncBtnX, speedRangeLowerBoundIncBtnY);
    connect(speedRangeLowerBoundIncBtn,SIGNAL(clicked()),this,SLOT(incrementSpeedRangeLowerBound()));

    int speedRangeLowerBoundDecBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeLowerBoundDecBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeLowerBoundDecBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + speedRangeLowerBoundIncBtnW;
    int speedRangeLowerBoundDecBtnY = settingY;
    Button* speedRangeLowerBoundDecBtn = new Button(QString("-"), Qt::red,20, speedRangeLowerBoundDecBtnW, speedRangeLowerBoundDecBtnH, 0, 0, this);
    speedRangeLowerBoundDecBtn->setPos(speedRangeLowerBoundDecBtnX, speedRangeLowerBoundDecBtnY);
    connect(speedRangeLowerBoundDecBtn,SIGNAL(clicked()),this,SLOT(decrementSpeedRangeLowerBound()));

    int speedRangeUpperBoundIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeUpperBoundIncBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeUpperBoundIncBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + (speedRangeLowerBoundIncBtnW*2);
    int speedRangeUpperBoundIncBtnY = settingY;
    Button* speedRangeUpperBoundIncBtn = new Button(QString(" +"), Qt::green,20, speedRangeUpperBoundIncBtnW, speedRangeUpperBoundIncBtnH, 0, 0, this);
    speedRangeUpperBoundIncBtn->setPos(speedRangeUpperBoundIncBtnX, speedRangeUpperBoundIncBtnY);
    connect(speedRangeUpperBoundIncBtn,SIGNAL(clicked()),this,SLOT(incrementSpeedRangeUpperBound()));

    int speedRangeUpperBoundDecBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeUpperBoundDecBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeUpperBoundDecBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + (speedRangeLowerBoundIncBtnW*3);
    int speedRangeUpperBoundDecBtnY = settingY;
    Button* speedRangeUpperBoundDecBtn = new Button(QString("-"), Qt::red,20, speedRangeUpperBoundDecBtnW, speedRangeUpperBoundDecBtnH, 0, 0, this);
    speedRangeUpperBoundDecBtn->setPos(speedRangeUpperBoundDecBtnX, speedRangeUpperBoundDecBtnY);
    connect(speedRangeUpperBoundDecBtn,SIGNAL(clicked()),this,SLOT(decrementSpeedRangeUpperBound()));

    // Apply Changes Button
    int restartBtnW = settingsPanelW - (settingsPanelW/2);
    int restartBtnH = settingsPanelH/5;
    int restartBtnX = settingsPanelX + (settingsPanelW/4);
    int restartBtnY = settingsPanelY + settingsPanelH - restartBtnH - btnPadding;
    Button* restartBtn = new Button(QString("Restart"), Qt::yellow, 20,restartBtnW, restartBtnH, 0, 0, this);
    restartBtn->setPos(restartBtnX, restartBtnY);
    connect(restartBtn,SIGNAL(clicked()),this,SLOT(restart()));

    this->setVisible(false);
}

void SettingsPanel::toggle(){
    setVisible(!isVisible());
}

void SettingsPanel::restart(){
    simulation->resetTimer();
    simulation->statisticsPanel->reset();
    simulation->destroyAllVehicles();
    if(simulation->isStarted) simulation->startToggle();
    toggle();
}

void SettingsPanel::toggleAlgorithm(){
    algorithmEnabled = !algorithmEnabled;
    algorithmSetting->setPlainText(QString("Algorithm: ") + QString(algorithmEnabled ? "On" : "Off"));
}

void SettingsPanel::toggleSoundEffects(){
    soundEffectsEnabled = !soundEffectsEnabled;
    soundEffectsSetting->setPlainText(QString("Sound Effects: ") + QString(soundEffectsEnabled ? "On" : "Off"));
}

void SettingsPanel::incrementVehiclesPerSec(){
    vehiclesPerSec++;
    vehiclesPerSecSetting->setPlainText(QString("Cars Per Second: ") + QString::number(vehiclesPerSec));
}

void SettingsPanel::decrementVehiclesPerSec(){
    if(vehiclesPerSec == 0) return;
    vehiclesPerSec--;
    vehiclesPerSecSetting->setPlainText(QString("Cars Per Second: ") + QString::number(vehiclesPerSec));
}

void SettingsPanel::incrementSpeedRangeLowerBound(){
    // Can go higher than Upper bound
    if(speedRangeUpperBound >= speedRangeLowerBound+1){
        speedRangeLowerBound++;
        speedRangeSetting->setPlainText(QString("Speed: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
    }
}

void SettingsPanel::decrementSpeedRangeLowerBound(){
    if(speedRangeLowerBound == 0) return;
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
