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
    settingsPanel = new SettingsPanel(screenWidth, screenHeight, btnPadding, trafficLightsEnabled, soundEffectsEnabled, unitsOfTime, initialSpeedRangeLowerBound, initialSpeedRangeUpperBound);
    scene->addItem(settingsPanel);


    // Settings panel toggle button
    int settingsBtnW = 50;
    int settingsBtnH = 50;
    int settingsBtnX = screenWidth - (screenWidth/10);
    int settingsBtnY = (screenHeight/10) - settingsBtnH;
    Button* settingsBtn = new Button(QString("⚙️"), Qt::yellow, settingsBtnW, settingsBtnH,0, 0);
    settingsBtn->setPos(settingsBtnX, settingsBtnY);
    connect(settingsBtn,SIGNAL(clicked()),settingsPanel,SLOT(toggle()));
    scene->addItem(settingsBtn);

    simulationStarted = false;
    collisons = 0;
    collisonsAvoided=0;
    carsOnScreen =0;
    totalCarsSpawned=0;

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(addVehicle()));

    drawGUI();
    drawStatistics();

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
    carsOnScreen--;
    carsOnScreenDisplay->setPlainText(QString("Cars On Screen: ") + QString::number(carsOnScreen));
}

void Simulation::addVehicle(){
    totalCarsSpawned++;
    carsOnScreen++;
    totalCarsSpawnedDisplay->setPlainText(QString("Total Cars Spawned: ") + QString::number(totalCarsSpawned));
    carsOnScreenDisplay->setPlainText(QString("Cars On Screen: ") + QString::number(carsOnScreen));
    Vehicle *vehicle = new Vehicle(settingsPanel->speedRangeLowerBound, settingsPanel->speedRangeUpperBound);
    scene->addItem(vehicle);
}

void Simulation::drawGUI(){
    // Bottom panel
    int bottomPanelX = 0;
    int bottomPanelY = screenHeight - (screenHeight/4);
    int bottomPanelW = screenWidth;
    int bottomPanelH = screenHeight/4;
    QGraphicsRectItem* bottomPanel = drawPanel(bottomPanelX, bottomPanelY, bottomPanelW, bottomPanelH, Qt::darkGray, 1);
    bottomPanel->setZValue(1);

    // Start/Stop button in Bottom Panel
    int playBtnX = bottomPanelX + bottomPanelW/30;
    int playBtnY = screenHeight -(bottomPanelH/2);
    int playBtnW = (bottomPanelW/6) + (btnPadding*2);
    int playBtnH = (bottomPanelH/2) - (btnPadding*2);
    playButton = new Button(QString("Start "), Qt::darkGreen, playBtnW, playBtnH, 0, 0, bottomPanel);
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
    int statisticsPanelY = screenHeight - (screenHeight/4) +10;
    int statisticsPanelW = screenWidth - 180;
    int statisticsPanelH = (screenHeight)/3 -80;
    QGraphicsRectItem* statisticsPanel = drawPanel(statisticsPanelX, statisticsPanelY, statisticsPanelW, statisticsPanelH, Qt::lightGray, 0.7);
    statisticsPanel->setZValue(2);

    int statisticsTextSize = 4;

    QGraphicsTextItem** statisticsTextsItems[4] = {&collisonsDisplay, &collisonsAvoidedDisplay, &totalCarsSpawnedDisplay, &carsOnScreenDisplay};
    QString statisticsTexts[statisticsTextSize] = {"Collisons: ", "Collisons Avoided: ", "Cars On Screen: ", "Total Cars Spawned: "};
    int statisticsValues[statisticsTextSize] = {collisons, collisonsAvoided, carsOnScreen, totalCarsSpawned};

    for(int i=0; i<statisticsTextSize; i++){
        *statisticsTextsItems[i] = new QGraphicsTextItem(QString(statisticsTexts[i]) + QString::number(statisticsValues[i]), statisticsPanel);
        (*statisticsTextsItems[i])->setPos(statisticsPanelX +btnPadding, statisticsPanelY + (i*statisticsPanelH/4) );
        (*statisticsTextsItems[i])->setFont(f);
        (*statisticsTextsItems[i])->setDefaultTextColor(Qt::white);
    }

    scene->addItem(statisticsPanel);
}
