#include <QtMath>
#include <QtWidgets>
#include <QTimer>
#include "Simulation.h"
#include "Vehicle.h"
#include "Button.h"
#include "string"

static constexpr int ScreenWidth = 1024;
static constexpr int ScreenHeight = 768;
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

    drawGUI();

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
    trafficLightSetting->setPlainText(QString("-> Traffic Lights: ") + QString(trafficLightsEnabled ? "On" : "Off"));
}

void Simulation::toggleSoundEffects(){
    soundEffectsEnabled = !soundEffectsEnabled;
    soundEffectsSetting->setPlainText(QString("-> Sound Effects: ") + QString(soundEffectsEnabled ? "On" : "Off"));
}

void Simulation::incrementUnitsOfTime(){
    unitsOfTime++;
    unitsOfTimeSetting->setPlainText(QString("-> Units Of Time: ") + QString::number(unitsOfTime));
}
void Simulation::decrementUnitsOfTime(){
    unitsOfTime--;
    unitsOfTimeSetting->setPlainText(QString("-> Units Of Time: ") + QString::number(unitsOfTime));
}

void Simulation::incrementSpeedRangeLowerBound(){
    // Can go higher than Upper bound
    if(speedRangeUpperBound >= speedRangeLowerBound+1){
        speedRangeLowerBound++;
        speedRangeSetting->setPlainText(QString("-> Speed Range: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
    }
}

void Simulation::decrementSpeedRangeLowerBound(){
    speedRangeLowerBound--;
    speedRangeSetting->setPlainText(QString("-> Speed Range: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
}

void Simulation::incrementSpeedRangeUpperBound(){
    speedRangeUpperBound++;
    speedRangeSetting->setPlainText(QString("-> Speed Range: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
}

void Simulation::decrementSpeedRangeUpperBound(){
    // Cant go lower than lower bound
    if(speedRangeLowerBound <= speedRangeUpperBound-1){
        speedRangeUpperBound--;
        speedRangeSetting->setPlainText(QString("-> Speed Range: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound));
    }
}

void Simulation::drawGUI(){
    // Settings panel toggle button
    int settingsBtnW = 50;
    int settingsBtnH = 50;
    int settingsBtnX = ScreenWidth - (ScreenWidth/10);
    int settingsBtnY = (ScreenHeight/10) - settingsBtnH;
    Button* settingsBtn = new Button(QString("⚙️"), Qt::yellow, settingsBtnW, settingsBtnH, 0, 0);
    settingsBtn->setPos(settingsBtnX, settingsBtnY);
    connect(settingsBtn,SIGNAL(clicked()),this,SLOT(toggleSettingsPanel()));
    scene->addItem(settingsBtn);

    // settingsPanel panel
    int settingsPanelX = ScreenWidth/10;
    int settingsPanelY = ScreenHeight/10;
    int settingsPanelW = ScreenWidth - (ScreenWidth/5);
    int settingsPanelH = ScreenHeight - (ScreenHeight/5) - (ScreenHeight/4);
    settingsPanel = drawPanel(settingsPanelX, settingsPanelY, settingsPanelW, settingsPanelH, Qt::black, 0.7);

    // Add settings to panel

    // Traffic Light Toggle
    QFont f;
    f.setPointSize(22);

    int settingX = settingsPanelX + btnPadding;
    int settingY = settingsPanelY + btnPadding;
    trafficLightSetting = new QGraphicsTextItem(QString("-> Traffic Lights: ") + QString(trafficLightsEnabled ? "On" : "Off"), settingsPanel);
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
    soundEffectsSetting = new QGraphicsTextItem(QString("-> Sound Effects: ") + QString(soundEffectsEnabled ? "On" : "Off"), settingsPanel);
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
    unitsOfTimeSetting = new QGraphicsTextItem(QString("-> Units Of Time: ") + QString::number(unitsOfTime), settingsPanel);
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
    speedRangeSetting = new QGraphicsTextItem(QString("-> Speed Range: ") + QString::number(speedRangeLowerBound) + QString(" to ") + QString::number(speedRangeUpperBound), settingsPanel);
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
    Button* speedRangeUpperBoundIncBtn = new Button(QString("+"), Qt::green, speedRangeUpperBoundIncBtnW, speedRangeUpperBoundIncBtnH, 0, 0, settingsPanel);
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
    int applyChangesBtnW = settingsPanelW - (btnPadding*2);
    int applyChangesBtnH = settingsPanelH/5;
    int applyChangesBtnX = settingsPanelX + btnPadding;
    int applyChangesBtnY = settingsPanelY + settingsPanelH - applyChangesBtnH - btnPadding;
    Button* applyChangesBtn = new Button(QString("Apply Changes"), Qt::yellow, applyChangesBtnW, applyChangesBtnH, 0, 0, settingsPanel);
    applyChangesBtn->setPos(applyChangesBtnX, applyChangesBtnY);
    connect(applyChangesBtn,SIGNAL(clicked()),this,SLOT(toggleSettingsPanel()));

    settingsPanel->setVisible(false);
    scene->addItem(settingsPanel);

    // Bottom panel
    int bottomPanelX = 0;
    int bottomPanelY = ScreenHeight - (ScreenHeight/4);
    int bottomPanelW = ScreenWidth;
    int bottomPanelH = ScreenHeight/4;
    QGraphicsRectItem* bottomPanel = drawPanel(bottomPanelX, bottomPanelY, bottomPanelW, bottomPanelH, Qt::black, 0.7);

    // Start/Stop button in Bottom Panel
    int playBtnX = bottomPanelX + btnPadding;
    int playBtnY = bottomPanelY + btnPadding;
    int playBtnW = (bottomPanelW/4) + (btnPadding*2);
    int playBtnH = bottomPanelH - (btnPadding*2);
    Button* playButton = new Button(QString("Play"), Qt::yellow, playBtnW, playBtnH, 0, 0, bottomPanel);
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
