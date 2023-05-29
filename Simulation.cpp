#include <QtMath>
#include <QtWidgets>
#include <QTimer>
#include "Simulation.h"
#include "Vehicle.h"
#include "string"
#include "Algorithm.h"

static constexpr int screenWidth = 600;
static constexpr int screenHeight = 800;
static constexpr int vehicleCount = 1;
static constexpr int btnPadding = 10;
static constexpr int initialSpeedRangeLowerBound = 80;
static constexpr int initialSpeedRangeUpperBound = 100;
static constexpr int vehiclesPerSec = 1;
static constexpr int trafficLightsEnabled = false;
static constexpr int soundEffectsEnabled = true;

SpawnOption* spawnOptions[16] = {
    new SpawnOption(0,310,0,"right","left"),
    new SpawnOption(0,340,0,"right","right"),
    new SpawnOption(600,260,180,"left","right"),
    new SpawnOption(600,290,180,"left","left"),
    new SpawnOption(260,0,270,"down","right"),
    new SpawnOption(290,0,270,"down","left"),
    new SpawnOption(310,550,90,"up","left"),
    new SpawnOption(340,550,90,"up","right"),
    new SpawnOption(0,310,0,"right","straight"),
    new SpawnOption(0,340,0,"right","straight"),
    new SpawnOption(600,260,180,"left","straight"),
    new SpawnOption(600,290,180,"left","straight"),
    new SpawnOption(260,0,270,"down","straight"),
    new SpawnOption(290,0,270,"down","straight"),
    new SpawnOption(310,550,90,"up","straight"),
    new SpawnOption(340,550,90,"up","straight")
};

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
    isStarted = false;
    mm= ss= 0;

    drawGUI();
    
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(addVehicle()));
    connect(timer,SIGNAL(timeout()),this,SLOT(incrementTimer()));
    Algorithm* a = new Algorithm("intersecting lines");
    a->run();

    show();
}

void Simulation::startToggle(){
    isStarted = !isStarted;
    if(isStarted){
        playButton->setText("Stop");
        playButton->setColor(Qt::red);
        timer->start(1000/(settingsPanel->vehiclesPerSec));
    }else{
        playButton->setText("Start");
        playButton->setColor(Qt::darkGreen);
        timer->stop();
    }
}

void Simulation::destroyAllVehicles(){
    foreach(Vehicle* v, aliveVehicles){
        v->selfDestruct();
    }
}

void Simulation::destroyCollidingVehicles(QList<QGraphicsItem *> list){

    foreach(QGraphicsItem* i, list){
        Vehicle * item= dynamic_cast<Vehicle *>(i);
        if (item)
        {
            item->selfDestruct();
            statisticsPanel->incrementCollisions();
        }

    }
}

void Simulation::decrementCarsOnScreen(){
    statisticsPanel->decrementCarsOnScreen();
}

void Simulation::addVehicle(){
    statisticsPanel->incrementTotalCarsSpawned();
    statisticsPanel->incrementCarsOnScreen();
    int pickedSpawnOption = (rand() % 16);
    Vehicle *vehicle = new Vehicle(settingsPanel->speedRangeLowerBound, settingsPanel->speedRangeUpperBound, spawnOptions[pickedSpawnOption]);
    scene->addItem(vehicle);
    aliveVehicles.append(vehicle);
}

void Simulation::drawGUI(){
    // Settings panel
    settingsPanel = new SettingsPanel(screenWidth, screenHeight, btnPadding, trafficLightsEnabled, soundEffectsEnabled, vehiclesPerSec, initialSpeedRangeLowerBound, initialSpeedRangeUpperBound);
    scene->addItem(settingsPanel);

    int settingsBtnW = 50;
    int settingsBtnH = 50;
    int settingsBtnX = screenWidth - (screenWidth/10);
    int settingsBtnY = (screenHeight/10) - settingsBtnH;
    Button* settingsBtn = new Button(QString("⚙️"), Qt::blue,20, settingsBtnW, settingsBtnH,0, 0);

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
    int playBtnX = bottomPanelX + btnPadding;
    int playBtnY = screenHeight -(bottomPanelH/1.5);
    int playBtnW = (bottomPanelW/12) + (btnPadding*2);
    int playBtnH = (bottomPanelH/3) - (btnPadding*2);

    playButton = new Button(QString("Start"), Qt::darkGreen,20, playBtnW, playBtnH, 0, 0, bottomPanel);
    playButton->setPos(playBtnX,playBtnY);
    connect(playButton,SIGNAL(clicked()),this,SLOT(startToggle()));

    //End simulation button in Bottom Panel
    int endSimBtnX = playBtnX + playBtnW+ btnPadding;
    int endSimBtnY = playBtnY;
    int endSimBtnW = (bottomPanelW/7) + (btnPadding*2);
    int endSimBtnH = (bottomPanelH/3) - (btnPadding*2);

    endSimButton = new Button(QString("Finish"), Qt::blue, 20,endSimBtnW, endSimBtnH, 0, 0, bottomPanel);
    endSimButton->setPos(endSimBtnX,endSimBtnY);
    //connect(resultsButton,SIGNAL(clicked()),this,SLOT(startToggle()));

    results = new Results(screenWidth, screenHeight, btnPadding);
    scene->addItem(results);
    results->setVisible(false);

    //Resuls button in Bottom Panel
    int resultsBtnX = playBtnX + btnPadding;
    int resultsBtnY = playBtnY + playBtnH + btnPadding;
    int resultsBtnW = playBtnW + endSimBtnW - btnPadding;
    int resultsBtnH = (bottomPanelH/3) - (btnPadding*2);

    resultsButton = new Button(QString("Results"), Qt::green, 20, resultsBtnW, resultsBtnH, 0, 0, bottomPanel);
    resultsButton->setPos(resultsBtnX,resultsBtnY);
    connect(resultsButton,SIGNAL(clicked()),results,SLOT(resultsPanel()));

    statisticsPanel = new StatisticsPanel(screenWidth, screenHeight, btnPadding, bottomPanel);

    scene->addItem(bottomPanel);
    drawTimer();

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

void Simulation::drawTimer(){
    QFont f;
    f.setPointSize(40);

    int timerX = screenWidth/10;
    int timerY = screenWidth;
    displayTimer = new QGraphicsTextItem(QString::number(mm)+ QString(":")+ QString::number(ss), bottomPanel);
    displayTimer->setPos(timerX, timerY);
    displayTimer->setFont(f);
    displayTimer->setDefaultTextColor(Qt::white);
}

void Simulation::incrementTimer(){
    ss++;
    if(ss == 60){
        mm++;
        ss = 0;
    }
    displayTimer->setPlainText(QString::number(mm)+ QString(":")+ QString::number(ss));
}

void Simulation::resetTimer(){
    timer->stop();
    mm= ss= 0;
    displayTimer->setPlainText(QString::number(mm)+ QString(":")+ QString::number(ss));
}
