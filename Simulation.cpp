#include <QtMath>
#include <QtWidgets>
#include <QTimer>
#include "Simulation.h"
#include "Vehicle.h"
#include "Button.h"
#include "string"

static constexpr int ScreenWidth = 680;
static constexpr int ScreenHeight = 715;
static constexpr int VehicleCount = 7;
static constexpr int btnPadding = 10;

Simulation::Simulation(QWidget *parent){
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,ScreenWidth,ScreenHeight);
    setBackgroundBrush(QBrush(QImage(":static/images/backgroundNoLights.png")));

    // Visualise
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(ScreenWidth,ScreenHeight);

    // Initial Settings
    trafficLightsEnabled = false;
    soundEffectsEnabled = true;
    unitsOfTime = 60;
    speedRangeLowerBound = 5;
    speedRangeUpperBound = 10;
    collisons = 0;
    collisonsAvoided=0;
    carsOnScreen =0;
    totalCarsSpawned=0;

    drawGUI();
    drawStatistics();
    drawTrafficLights();

    show();
}

void Simulation::start(){
    for (int i = 0; i < VehicleCount; ++i) {
        Vehicle *vehicle = new Vehicle;
        scene->addItem(vehicle);
    }
}

void Simulation::toggleSettingsPanel(){
    settingsPanel->setVisible(!settingsPanel->isVisible());
}

void Simulation::toggleTrafficLights(){
    trafficLightsEnabled = !trafficLightsEnabled;
    trafficLightSetting->setPlainText(QString("Traffic Lights: ") + QString(trafficLightsEnabled ? "On" : "Off"));
}

void Simulation::toggleSoundEffects(){
    soundEffectsEnabled = !soundEffectsEnabled;
    soundEffectsSetting->setPlainText(QString("Sound Effects: ") + QString(soundEffectsEnabled ? "On" : "Off"));
}

void Simulation::incrementUnitsOfTime(){
    unitsOfTime++;
    unitsOfTimeSetting->setPlainText(QString("Units Of Time: ") + QString::number(unitsOfTime));
}

void Simulation::decrementUnitsOfTime(){
    unitsOfTime--;
    unitsOfTimeSetting->setPlainText(QString("Units Of Time: ") + QString::number(unitsOfTime));
}

void Simulation::incrementSpeedRangeLowerBound(){
    // Can go higher than Upper bound
    if(speedRangeUpperBound >= speedRangeLowerBound+1){
        speedRangeLowerBound++;
        speedRangeSetting->setPlainText(QString("Speed Range: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
    }
}

void Simulation::decrementSpeedRangeLowerBound(){
    speedRangeLowerBound--;
    speedRangeSetting->setPlainText(QString("Speed Range: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
}

void Simulation::incrementSpeedRangeUpperBound(){
    speedRangeUpperBound++;
    speedRangeSetting->setPlainText(QString("Speed Range: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
}

void Simulation::decrementSpeedRangeUpperBound(){
    // Cant go lower than lower bound
    if(speedRangeLowerBound <= speedRangeUpperBound-1){
        speedRangeUpperBound--;
        speedRangeSetting->setPlainText(QString("Speed Range: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
    }
}

void Simulation::drawGUI(){
    // Settings panel toggle button
    int settingsBtnW = 50;
    int settingsBtnH = 50;
    int settingsBtnX = ScreenWidth - (ScreenWidth/10);
    int settingsBtnY = (ScreenHeight/10) - settingsBtnH;
    Button* settingsBtn = new Button(QString("⚙️"), Qt::blue, settingsBtnW, settingsBtnH,0, 0);
    settingsBtn->setPos(settingsBtnX, settingsBtnY);
    connect(settingsBtn,SIGNAL(clicked()),this,SLOT(toggleSettingsPanel()));
    scene->addItem(settingsBtn);

    // settingsPanel panel
    int settingsPanelX = ScreenWidth/10;
    int settingsPanelY = ScreenHeight/10;
    int settingsPanelW = ScreenWidth - (ScreenWidth/5);
    int settingsPanelH = ScreenHeight - (ScreenHeight/5) - (ScreenHeight/4);
    settingsPanel = drawPanel(settingsPanelX, settingsPanelY, settingsPanelW, settingsPanelH, Qt::black, 0.7);
    settingsPanel->setZValue(2);

    // Add settings to panel
    QFont f;
    f.setPointSize(20);

    int settingX = settingsPanelX + btnPadding;
    int settingY = settingsPanelY + btnPadding;

    // Traffic Light Toggle
    trafficLightSetting = new QGraphicsTextItem(QString("Traffic Lights: ") + QString(trafficLightsEnabled ? "On" : "Off"), settingsPanel);
    trafficLightSetting->setPos(settingX, settingY);
    trafficLightSetting->setFont(f);
    trafficLightSetting->setDefaultTextColor(Qt::white);

    int trafficLightBtnW = (settingsPanelW/2) - (btnPadding*2);
    int trafficLightBtnH = (settingsPanelH/5) - (btnPadding*2);
    int trafficLightBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int trafficLightBtnY = settingY;
    Button* trafficLightBtn = new Button(QString("O/I"), Qt::green, trafficLightBtnW, trafficLightBtnH, 0, 0, settingsPanel);
    trafficLightBtn->setPos(trafficLightBtnX, trafficLightBtnY);
    connect(trafficLightBtn,SIGNAL(clicked()),this,SLOT(toggleTrafficLights()));

    // Sound Effects Setting
    settingY += settingsPanelH/5;

    soundEffectsSetting = new QGraphicsTextItem(QString("Sound Effects: ") + QString(soundEffectsEnabled ? "On" : "Off"), settingsPanel);
    soundEffectsSetting->setPos(settingX, settingY);
    soundEffectsSetting->setFont(f);
    soundEffectsSetting->setDefaultTextColor(Qt::white);


    int soundEffectsBtnW = (settingsPanelW/2) - (btnPadding*2);
    int soundEffectsBtnH = (settingsPanelH/5) - (btnPadding*2);
    int soundEffectsBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int soundEffectsBtnY = settingY;
    Button* soundEffectsBtn = new Button(QString("O/I"), Qt::green, soundEffectsBtnW, soundEffectsBtnH, 0, 0, settingsPanel);
    soundEffectsBtn->setPos(soundEffectsBtnX, soundEffectsBtnY);
    connect(soundEffectsBtn,SIGNAL(clicked()),this,SLOT(toggleSoundEffects()));

    // Units of Time setting
    settingY += settingsPanelH/5;

    unitsOfTimeSetting = new QGraphicsTextItem(QString("Units Of Time: ") + QString::number(unitsOfTime), settingsPanel);
    unitsOfTimeSetting->setPos(settingX, settingY);
    unitsOfTimeSetting->setFont(f);
    unitsOfTimeSetting->setDefaultTextColor(Qt::white);

    int unitsOfTimeIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/2;
    int unitsOfTimeIncBtnH = (settingsPanelH/5) - (btnPadding*2);
    int unitsOfTimeIncBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int unitsOfTimeIncBtnY = settingY;
    Button* unitsOfTimeIncBtn = new Button(QString("+"), Qt::green, unitsOfTimeIncBtnW, unitsOfTimeIncBtnH, 0, 0, settingsPanel);
    unitsOfTimeIncBtn->setPos(unitsOfTimeIncBtnX, unitsOfTimeIncBtnY);
    connect(unitsOfTimeIncBtn,SIGNAL(clicked()),this,SLOT(incrementUnitsOfTime()));

    int unitsOfTimeDecBtnW = unitsOfTimeIncBtnW;
    int unitsOfTimeDecBtnH = (settingsPanelH/5) - (btnPadding*2);
    int unitsOfTimeDecBtnX = unitsOfTimeIncBtnX + unitsOfTimeIncBtnW;
    int unitsOfTimeDecBtnY = unitsOfTimeIncBtnY;
    Button* unitsOfTimeDecBtn = new Button(QString("-"), Qt::red, unitsOfTimeDecBtnW, unitsOfTimeDecBtnH, 0, 0, settingsPanel);
    unitsOfTimeDecBtn->setPos(unitsOfTimeDecBtnX, unitsOfTimeDecBtnY);
    connect(unitsOfTimeDecBtn,SIGNAL(clicked()),this,SLOT(decrementUnitsOfTime()));

    // Speed Range setting
    settingY += settingsPanelH/5;
    speedRangeSetting = new QGraphicsTextItem(QString("Speed Range: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound), settingsPanel);

    speedRangeSetting->setPos(settingX, settingY);
    speedRangeSetting->setFont(f);
    speedRangeSetting->setDefaultTextColor(Qt::white);

    int speedRangeLowerBoundIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeLowerBoundIncBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeLowerBoundIncBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int speedRangeLowerBoundIncBtnY = settingY;
    Button* speedRangeLowerBoundIncBtn = new Button(QString("+"), Qt::green, speedRangeLowerBoundIncBtnW, speedRangeLowerBoundIncBtnH, 0, 0, settingsPanel);
    speedRangeLowerBoundIncBtn->setPos(speedRangeLowerBoundIncBtnX, speedRangeLowerBoundIncBtnY);
    connect(speedRangeLowerBoundIncBtn,SIGNAL(clicked()),this,SLOT(incrementSpeedRangeLowerBound()));

    int speedRangeLowerBoundDecBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeLowerBoundDecBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeLowerBoundDecBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + speedRangeLowerBoundIncBtnW;
    int speedRangeLowerBoundDecBtnY = settingY;
    Button* speedRangeLowerBoundDecBtn = new Button(QString("-"), Qt::red, speedRangeLowerBoundDecBtnW, speedRangeLowerBoundDecBtnH, 0, 0, settingsPanel);
    speedRangeLowerBoundDecBtn->setPos(speedRangeLowerBoundDecBtnX, speedRangeLowerBoundDecBtnY);
    connect(speedRangeLowerBoundDecBtn,SIGNAL(clicked()),this,SLOT(decrementSpeedRangeLowerBound()));

    int speedRangeUpperBoundIncBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeUpperBoundIncBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeUpperBoundIncBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + (speedRangeLowerBoundIncBtnW*2);
    int speedRangeUpperBoundIncBtnY = settingY;
    Button* speedRangeUpperBoundIncBtn = new Button(QString(" +"), Qt::green, speedRangeUpperBoundIncBtnW, speedRangeUpperBoundIncBtnH, 0, 0, settingsPanel);
    speedRangeUpperBoundIncBtn->setPos(speedRangeUpperBoundIncBtnX, speedRangeUpperBoundIncBtnY);
    connect(speedRangeUpperBoundIncBtn,SIGNAL(clicked()),this,SLOT(incrementSpeedRangeUpperBound()));

    int speedRangeUpperBoundDecBtnW = ((settingsPanelW/2) - (btnPadding*2))/4;
    int speedRangeUpperBoundDecBtnH = (settingsPanelH/5) - (btnPadding*2);
    int speedRangeUpperBoundDecBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding + (speedRangeLowerBoundIncBtnW*3);
    int speedRangeUpperBoundDecBtnY = settingY;
    Button* speedRangeUpperBoundDecBtn = new Button(QString("-"), Qt::red, speedRangeUpperBoundDecBtnW, speedRangeUpperBoundDecBtnH, 0, 0, settingsPanel);
    speedRangeUpperBoundDecBtn->setPos(speedRangeUpperBoundDecBtnX, speedRangeUpperBoundDecBtnY);
    connect(speedRangeUpperBoundDecBtn,SIGNAL(clicked()),this,SLOT(decrementSpeedRangeUpperBound()));

    // Apply Changes Button
    int applyChangesBtnW = settingsPanelW - (settingsPanelW/2);
    int applyChangesBtnH = settingsPanelH/5;
    int applyChangesBtnX = settingsPanelX + (settingsPanelW/4);
    int applyChangesBtnY = settingsPanelY + settingsPanelH - applyChangesBtnH - btnPadding;
    Button* applyChangesBtn = new Button(QString("Apply"), Qt::yellow, applyChangesBtnW, applyChangesBtnH, 0, 0, settingsPanel);
    applyChangesBtn->setPos(applyChangesBtnX, applyChangesBtnY);
    connect(applyChangesBtn,SIGNAL(clicked()),this,SLOT(toggleSettingsPanel()));

    settingsPanel->setVisible(false);
    scene->addItem(settingsPanel);

    // Bottom panel
    int bottomPanelX = 0;
    int bottomPanelY = ScreenHeight - (ScreenHeight/4);
    int bottomPanelW = ScreenWidth;
    int bottomPanelH = ScreenHeight/4;
    bottomPanel = drawPanel(bottomPanelX, bottomPanelY, bottomPanelW, bottomPanelH, Qt::darkGray, 1);
    bottomPanel->setZValue(1);


    // Start/Stop button in Bottom Panel
    int playBtnX = bottomPanelX + bottomPanelW/30;
    int playBtnY = ScreenHeight -(bottomPanelH/2);
    int playBtnW = (bottomPanelW/6) + (btnPadding*2);
    int playBtnH = (bottomPanelH/2) - (btnPadding*2);
    Button* playButton = new Button(QString("Start  "), Qt::darkGreen, playBtnW, playBtnH, 0, 1, bottomPanel);
    playButton->setPos(playBtnX,playBtnY);
    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));

    scene->addItem(bottomPanel);

}

QGraphicsRectItem* Simulation::drawPanel(int x, int y, int width, int height, QColor color, double opacity){
    QGraphicsRectItem* panel = new QGraphicsRectItem(x,y,width,height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    return panel;
}

void Simulation::drawStatistics(){
    QFont f;
    f.setPointSize(20);

    // Statistics panel
    int statisticsPanelX = 168;
    int statisticsPanelY = ScreenHeight - (ScreenHeight/4) +10;
    int statisticsPanelW = ScreenWidth - 180;
    int statisticsPanelH = (ScreenHeight)/3 -80;
    QGraphicsRectItem* statisticsPanel = drawPanel(statisticsPanelX, statisticsPanelY, statisticsPanelW, statisticsPanelH, Qt::lightGray, 0.7);
    statisticsPanel->setZValue(2);

    int statisticsTextSize = 4;
    QGraphicsTextItem* statisticsTextsItems[statisticsTextSize] = {collisonsDisplay, collisonsAvoidedDisplay, carsOnScreenDisplay, totalCarsSpawnedDisplay};
    QString statisticsTexts[statisticsTextSize] = {"Collisons: ", "Collisons Avoided: ", "Cars On Screen: ", "Total Cars Spawned: "};
    int statisticsValues[statisticsTextSize] = {collisons, collisonsAvoided, carsOnScreen, totalCarsSpawned};

    for(int i=0; i<statisticsTextSize; i++){
        statisticsTextsItems[i] = new QGraphicsTextItem(QString(statisticsTexts[i]) + QString::number(statisticsValues[i]), statisticsPanel);
        statisticsTextsItems[i]->setPos(statisticsPanelX +btnPadding, statisticsPanelY + (i*statisticsPanelH/4) );
        statisticsTextsItems[i]->setFont(f);
        statisticsTextsItems[i]->setDefaultTextColor(Qt::white);
    }


    scene->addItem(statisticsPanel);
}


void Simulation::drawTrafficLights(){
    Button* trafficLight1;
    Button* trafficLight2;
    Button* trafficLight3;
    Button* trafficLight4;

    Button* trafficLightsList[4] = {trafficLight1, trafficLight2, trafficLight3, trafficLight4};

    // create with loop and add location manually
    for(int i=0; i<4;i++){
    int trafficLightW = 25;
    int trafficLightH = 25;
    if(i%2 ==0){
    trafficLightsList[i] = new Button(QString(""), Qt::green, trafficLightW, trafficLightH, 0, 0, bottomPanel);
    }
    else{
        trafficLightsList[i] = new Button(QString(""), Qt::red, trafficLightW, trafficLightH, 0, 0, bottomPanel);
    }
    trafficLightsList[i]->setOpacity(1);
    trafficLightsList[i]->setEnabled(false);
}
    /*
    trafficLightsList[0]->setPos(ScreenWidth/3.5, ScreenWidth/5.3);
    trafficLightsList[1]->setPos(ScreenWidth/1.5, ScreenWidth/5.5);
    trafficLightsList[2]->setPos(ScreenWidth/1.5, ScreenWidth/1.7);
    trafficLightsList[3]->setPos(ScreenWidth/3.5, ScreenWidth/1.8);
    */
    //Green lights
    trafficLightsList[0]->setPos(ScreenWidth -(ScreenWidth- ScreenWidth/3.55), ScreenWidth/6);
    trafficLightsList[2]->setPos(ScreenWidth/1.55, ScreenWidth/1.65);

    //Red lights
    trafficLightsList[1]->setPos(ScreenWidth - ScreenWidth/3.2, ScreenWidth/4.8);
    trafficLightsList[3]->setPos(ScreenWidth/4, ScreenWidth/1.8);
}


