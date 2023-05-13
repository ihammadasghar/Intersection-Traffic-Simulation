#include <QtMath>
#include <QtWidgets>
#include <QTimer>
#include "Simulation.h"
#include "Vehicle.h"
#include "Button.h"

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
    speedRangeBottomBound = 5;
    speedRangeTopBound = 10;

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
}

void Simulation::toggleSoundEffects(){
    soundEffectsEnabled = !soundEffectsEnabled;
}

void Simulation::drawGUI(){
    // Settings panel toggle button
    int settingsBtnW = 50;
    int settingsBtnH = 50;
    int settingsBtnX = ScreenWidth - (ScreenWidth/10);
    int settingsBtnY = (ScreenHeight/10) - settingsBtnH;
    Button* settingsBtn = new Button(QString("Settings"), Qt::yellow, settingsBtnW, settingsBtnH, 0, 0);
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
    QGraphicsTextItem* trafficLightSetting = new QGraphicsTextItem("-> Traffic Lights", settingsPanel);
    trafficLightSetting->setPos(settingX, settingY);
    trafficLightSetting->setFont(f);
    trafficLightSetting->setDefaultTextColor(Qt::white);

    int trafficLightBtnW = (settingsPanelW/2) - (btnPadding*2);
    int trafficLightBtnH = (settingsPanelH/5) - (btnPadding*2);
    int trafficLightBtnX = settingsPanelX + (settingsPanelW/2) + btnPadding;
    int trafficLightBtnY = settingY;
    Button* trafficLightBtn = new Button(QString(trafficLightsEnabled ? "Turn off" : "Turn On"), Qt::yellow, trafficLightBtnW, trafficLightBtnH, 0, 0, settingsPanel);
    trafficLightBtn->setPos(trafficLightBtnX, trafficLightBtnY);
    connect(trafficLightBtn,SIGNAL(clicked()),this,SLOT(toggleTrafficLights()));


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
