#include <QtMath>
#include <QtWidgets>
#include <QTimer>
#include "Simulation.h"
#include "Vehicle.h"
#include "string"

static constexpr int screenWidth = 680;
static constexpr int screenHeight = 715;
static constexpr int vehicleCount = 1;
static constexpr int btnPadding = 10;
static constexpr int initialSpeedRangeLowerBound = 80;
static constexpr int initialSpeedRangeUpperBound = 100;
static constexpr int vehiclesPerSec = 1;
static constexpr int trafficLightsEnabled = false;
static constexpr int soundEffectsEnabled = true;
static constexpr int unitsOfTime = 60;

Simulation::Simulation(QWidget *parent){
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,screenWidth,screenHeight);
    setBackgroundBrush(QBrush(QImage(":static/images/backgroundNoLights.png")));

    // Visualise
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(screenWidth,screenHeight);

    // Initial Settings
    simulationStarted = false;

    drawGUI();
    
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(addVehicle()));
  
    drawTrafficLights();

    show();
}

void Simulation::start(){
    simulationStarted = !simulationStarted;
    if(simulationStarted){
        playButton->setText("Stop");
        playButton->setColor(Qt::red);
        timer->start(1000/vehiclesPerSec);
    }else{
        playButton->setText("Start");
        playButton->setColor(Qt::darkGreen);
        timer->stop();
    }
}

void Simulation::decrementCarsOnScreen(){
    statisticsPanel->decrementCarsOnScreen();
}

void Simulation::addVehicle(){
    statisticsPanel->incrementTotalCarsSpawned();
    statisticsPanel->incrementCarsOnScreen();

    Vehicle *vehicle = new Vehicle(settingsPanel->speedRangeLowerBound, settingsPanel->speedRangeUpperBound);
    scene->addItem(vehicle);
}

void Simulation::drawGUI(){
    // Settings panel
    settingsPanel = new SettingsPanel(screenWidth, screenHeight, btnPadding, trafficLightsEnabled, soundEffectsEnabled, unitsOfTime, initialSpeedRangeLowerBound, initialSpeedRangeUpperBound);
    scene->addItem(settingsPanel);

    int settingsBtnW = 50;
    int settingsBtnH = 50;
    int settingsBtnX = screenWidth - (screenWidth/10);
    int settingsBtnY = (screenHeight/10) - settingsBtnH;
    Button* settingsBtn = new Button(QString("⚙️"), Qt::blue, settingsBtnW, settingsBtnH,0, 0);

    settingsBtn->setPos(settingsBtnX, settingsBtnY);
    connect(settingsBtn,SIGNAL(clicked()),settingsPanel,SLOT(toggle()));
    scene->addItem(settingsBtn);

    // Bottom panel
    int bottomPanelX = 0;
    int bottomPanelY = screenHeight - (screenHeight/4);
    int bottomPanelW = screenWidth;
    int bottomPanelH = screenHeight/4;
    bottomPanel = drawPanel(bottomPanelX, bottomPanelY, bottomPanelW, bottomPanelH, Qt::darkGray, 1);

    bottomPanel->setZValue(1);


    // Start/Stop button in Bottom Panel
    int playBtnX = bottomPanelX + bottomPanelW/30;
    int playBtnY = screenHeight -(bottomPanelH/2);
    int playBtnW = (bottomPanelW/6) + (btnPadding*2);
    int playBtnH = (bottomPanelH/2) - (btnPadding*2);

    playButton = new Button(QString("Start "), Qt::darkGreen, playBtnW, playBtnH, 0, 0, bottomPanel);
    playButton->setPos(playBtnX,playBtnY);
    connect(playButton,SIGNAL(clicked()),this,SLOT(start()));

    statisticsPanel = new StatisticsPanel(screenWidth, screenHeight, btnPadding, bottomPanel);

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

    //Green lights
    trafficLightsList[0]->setPos(screenWidth -(screenWidth- screenWidth/3.55), screenWidth/6);
    trafficLightsList[2]->setPos(screenWidth/1.55, screenWidth/1.65);

    //Red lights
    trafficLightsList[1]->setPos(screenWidth - screenWidth/3.2, screenWidth/4.8);
    trafficLightsList[3]->setPos(screenWidth/4, screenWidth/1.8);
}

