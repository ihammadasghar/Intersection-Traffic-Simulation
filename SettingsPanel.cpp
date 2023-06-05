#include "SettingsPanel.h"
#include "Simulation.h"

extern Simulation * simulation;

SettingsPanel::SettingsPanel(int screenWidth, int screenHeight, int btnPadding, bool initialAlgorithmEnabled, bool initialVehicleDetailsEnabled, int initialVehiclesPerSec, int initialSpeedRangeLowerBound, int initialSpeedRangeUpperBound, int initialTimerLimit, QGraphicsItem *parent): QObject(), QGraphicsRectItem(parent){
    trafficLightsEnabled = false;
    algorithmEnabled = initialAlgorithmEnabled;
    vehicleDetailsEnabled = initialVehicleDetailsEnabled;
    vehiclesPerSec = initialVehiclesPerSec;
    speedRangeLowerBound = initialSpeedRangeLowerBound;
    speedRangeUpperBound = initialSpeedRangeUpperBound;
    timerLimit = initialTimerLimit;

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
    // Settings Display Texts
    QFont f;
    f.setPointSize(20);

    int settingX = settingsPanelX + btnPadding;
    int settingY = settingsPanelY + btnPadding;

    int numOfSettings = 6;
    QGraphicsTextItem** displayTextGraphics[numOfSettings] = {
        &algorithmSetting,
        &vehicleDetailsSetting,
        &vehiclesPerSecSetting,
        &speedRangeSetting,
        &timerLimitSetting,
        &trafficLightsSetting
    };
    QString displayTextsStrings[numOfSettings] = {
        QString("Algorithm: "+ QString(algorithmEnabled ? "On" : "Off")),
        QString("Vehicle Details: ") + QString(vehicleDetailsEnabled ? "On" : "Off"),
        QString("Cars Per Second: ") + QString::number(vehiclesPerSec),
        QString("Speed: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound),
        QString("Timer Limit: ") + QString::number(timerLimit) + " mins",
        QString("Traffic Lights: ") + QString(trafficLightsEnabled ? "On" : "Off"),
    };

    for(int i = 0; i < numOfSettings; i++){
        *displayTextGraphics[i] = new QGraphicsTextItem(displayTextsStrings[i], this);
        (*displayTextGraphics[i])->setPos(settingX, settingY + (i*settingsPanelH/(numOfSettings+1)));
        (*displayTextGraphics[i])->setFont(f);
        (*displayTextGraphics[i])->setDefaultTextColor(Qt::white);
    }

    // Buttons
    int btnHeight = (settingsPanelH/(numOfSettings+1)) - (btnPadding*2);
    int btnX = settingsPanelX + (settingsPanelW/2) + btnPadding;

    // Algorithm Toggle
    int algorithmBtnW = (settingsPanelW/2) - (btnPadding*2);
    int algorithmBtnY = algorithmSetting->y();
    Button* algorithmBtn = new Button(QString("O/I"), Qt::green, 20, algorithmBtnW, btnHeight, 0, 0, this);
    algorithmBtn->setPos(btnX, algorithmBtnY);
    connect(algorithmBtn,SIGNAL(clicked()),this,SLOT(toggleAlgorithm()));

    // Vehicle Details Setting
    int vehicleDetailsBtnW = (settingsPanelW/2) - (btnPadding*2);
    int vehicleDetailsBtnY = vehicleDetailsSetting->y();
    Button* vehicleDetailsBtn = new Button(QString("O/I"), Qt::green,20, vehicleDetailsBtnW, btnHeight, 0, 0, this);
    vehicleDetailsBtn->setPos(btnX, vehicleDetailsBtnY);
    connect(vehicleDetailsBtn,SIGNAL(clicked()),this,SLOT(toggleVehicleDetails()));

    // vehicles per sec setting
    int vehiclesPerSecIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/2;
    int vehiclesPerSecIncBtnX = btnX;
    int vehiclesPerSecIncBtnY = vehiclesPerSecSetting->y();
    Button* vehiclesPerSecIncBtn = new Button(QString("+"), Qt::green,20, vehiclesPerSecIncBtnW, btnHeight, 0, 0, this);
    vehiclesPerSecIncBtn->setPos(btnX, vehiclesPerSecIncBtnY);
    connect(vehiclesPerSecIncBtn,SIGNAL(clicked()),this,SLOT(incrementVehiclesPerSec()));

    int vehiclesPerSecDecBtnW = vehiclesPerSecIncBtnW;
    int vehiclesPerSecDecBtnX = vehiclesPerSecIncBtnX + vehiclesPerSecIncBtnW;
    int vehiclesPerSecDecBtnY = vehiclesPerSecIncBtnY;
    Button* vehiclesPerSecDecBtn = new Button(QString("-"), Qt::red,20, vehiclesPerSecDecBtnW, btnHeight, 0, 0, this);
    vehiclesPerSecDecBtn->setPos(vehiclesPerSecDecBtnX, vehiclesPerSecDecBtnY);
    connect(vehiclesPerSecDecBtn,SIGNAL(clicked()),this,SLOT(decrementVehiclesPerSec()));

    // Speed Range setting
    int speedRangeLowerBoundIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeLowerBoundIncBtnX = btnX;
    int speedRangeLowerBoundIncBtnY = speedRangeSetting->y();
    Button* speedRangeLowerBoundIncBtn = new Button(QString("+"), Qt::green,20, speedRangeLowerBoundIncBtnW, btnHeight, 0, 0, this);
    speedRangeLowerBoundIncBtn->setPos(speedRangeLowerBoundIncBtnX, speedRangeLowerBoundIncBtnY);
    connect(speedRangeLowerBoundIncBtn,SIGNAL(clicked()),this,SLOT(incrementSpeedRangeLowerBound()));

    int speedRangeLowerBoundDecBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeLowerBoundDecBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + speedRangeLowerBoundIncBtnW;
    int speedRangeLowerBoundDecBtnY = speedRangeSetting->y();
    Button* speedRangeLowerBoundDecBtn = new Button(QString("-"), Qt::red,20, speedRangeLowerBoundDecBtnW, btnHeight, 0, 0, this);
    speedRangeLowerBoundDecBtn->setPos(speedRangeLowerBoundDecBtnX, speedRangeLowerBoundDecBtnY);
    connect(speedRangeLowerBoundDecBtn,SIGNAL(clicked()),this,SLOT(decrementSpeedRangeLowerBound()));

    int speedRangeUpperBoundIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeUpperBoundIncBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + (speedRangeLowerBoundIncBtnW*2);
    int speedRangeUpperBoundIncBtnY = speedRangeSetting->y();
    Button* speedRangeUpperBoundIncBtn = new Button(QString(" +"), Qt::green,20, speedRangeUpperBoundIncBtnW, btnHeight, 0, 0, this);
    speedRangeUpperBoundIncBtn->setPos(speedRangeUpperBoundIncBtnX, speedRangeUpperBoundIncBtnY);
    connect(speedRangeUpperBoundIncBtn,SIGNAL(clicked()),this,SLOT(incrementSpeedRangeUpperBound()));

    int speedRangeUpperBoundDecBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeUpperBoundDecBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + (speedRangeLowerBoundIncBtnW*3);
    int speedRangeUpperBoundDecBtnY = speedRangeSetting->y();
    Button* speedRangeUpperBoundDecBtn = new Button(QString("-"), Qt::red,20, speedRangeUpperBoundDecBtnW, btnHeight, 0, 0, this);
    speedRangeUpperBoundDecBtn->setPos(speedRangeUpperBoundDecBtnX, speedRangeUpperBoundDecBtnY);
    connect(speedRangeUpperBoundDecBtn,SIGNAL(clicked()),this,SLOT(decrementSpeedRangeUpperBound()));

    // timer Limit setting
    int timerLimitIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/2;
    int timerLimitIncBtnX = btnX;
    int timerLimitIncBtnY = timerLimitSetting->y();
    Button* timerLimitIncBtn = new Button(QString("+"), Qt::green,20, timerLimitIncBtnW, btnHeight, 0, 0, this);
    timerLimitIncBtn->setPos(btnX, timerLimitIncBtnY);
    connect(timerLimitIncBtn,SIGNAL(clicked()),this,SLOT(incrementTimerLimit()));

    int timerLimitDecBtnW = timerLimitIncBtnW;
    int timerLimitDecBtnX = timerLimitIncBtnX + timerLimitIncBtnW;
    int timerLimitDecBtnY = timerLimitIncBtnY;
    Button* timerLimitDecBtn = new Button(QString("-"), Qt::red,20, timerLimitDecBtnW, btnHeight, 0, 0, this);
    timerLimitDecBtn->setPos(timerLimitDecBtnX, timerLimitDecBtnY);
    connect(timerLimitDecBtn,SIGNAL(clicked()),this,SLOT(decrementTimerLimit()));

    // Apply Changes Button
    int restartBtnW = settingsPanelW - (settingsPanelW/2);
    int restartBtnX = settingsPanelX + (settingsPanelW/4);
    int restartBtnY = settingsPanelY + settingsPanelH - btnHeight - btnPadding;
    Button* restartBtn = new Button(QString("Restart"), Qt::yellow, 20,restartBtnW, btnHeight, 0, 0, this);
    restartBtn->setPos(restartBtnX, restartBtnY);
    connect(restartBtn,SIGNAL(clicked()),this,SLOT(restart()));

    // TrafficLights Toggle
    int trafficLightsBtnW = (settingsPanelW/2) - (btnPadding*2);
    int trafficLightsBtnY = trafficLightsSetting->y();
    Button* trafficLightsBtn = new Button(QString("O/I"), Qt::green, 20, trafficLightsBtnW, btnHeight, 0, 0, this);
    trafficLightsBtn->setPos(btnX, trafficLightsBtnY);
    connect(trafficLightsBtn,SIGNAL(clicked()),this,SLOT(toggleTrafficLights()));

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
    if(algorithmEnabled) simulation->algorithm->run();
    algorithmSetting->setPlainText(QString("Algorithm: ") + QString(algorithmEnabled ? "On" : "Off"));
}

void SettingsPanel::toggleTrafficLights(){
    trafficLightsEnabled = !trafficLightsEnabled;
    trafficLightsSetting->setPlainText(QString("Traffic Lights: ") + QString(trafficLightsEnabled ? "On" : "Off"));
}

void SettingsPanel::toggleVehicleDetails(){
    vehicleDetailsEnabled = !vehicleDetailsEnabled;
    vehicleDetailsSetting->setPlainText(QString("Vehicle Details: ") + QString(vehicleDetailsEnabled ? "On" : "Off"));
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

void SettingsPanel::incrementTimerLimit(){
    timerLimit++;
    timerLimitSetting->setPlainText(QString("Timer Limit: ") + QString::number(timerLimit) + " mins");
}

void SettingsPanel::decrementTimerLimit(){
    if(timerLimit == 1) return;
    timerLimit--;
    timerLimitSetting->setPlainText(QString("Timer Limit: ") + QString::number(timerLimit) + " mins");
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
