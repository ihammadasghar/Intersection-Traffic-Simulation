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
static constexpr int vehiclesPerSec = 2;
static constexpr int algorithmEnabled = false;
static constexpr int soundEffectsEnabled = true;
static constexpr int timerLimit = 2;
SpawnOption* spawnOptions[16] = {
    new SpawnOption(0,310,0,"right","left"),
    new SpawnOption(0,340,0,"right","right"),
    new SpawnOption(600,260,180,"left","right"),
    new SpawnOption(600,290,180,"left","left"),
    new SpawnOption(260,0,90,"down","right"),
    new SpawnOption(290,0,90,"down","left"),
    new SpawnOption(310,550,270,"up","left"),
    new SpawnOption(340,550,270,"up","right"),
    new SpawnOption(0,310,0,"right","straight"),
    new SpawnOption(0,340,0,"right","straight"),
    new SpawnOption(600,260,180,"left","straight"),
    new SpawnOption(600,290,180,"left","straight"),
    new SpawnOption(260,0,90,"down","straight"),
    new SpawnOption(290,0,90,"down","straight"),
    new SpawnOption(310,550,270,"up","straight"),
    new SpawnOption(340,550,270,"up","straight")
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
    seconds = 0;
    algorithm = new Algorithm("intersecting lines");

    // Setup Timers
    vehicleSpawnTimer = new QTimer(this);
    connect(vehicleSpawnTimer,SIGNAL(timeout()),this,SLOT(addVehicle()));
    
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(incrementTimer()));

    drawGUI();
    show();
}

void Simulation::startToggle(){
    isStarted = !isStarted;
    if(isStarted){
        playButton->setText("Stop");
        playButton->setColor(Qt::blue);
        vehicleSpawnTimer->start(1000/(settingsPanel->vehiclesPerSec));
        timer->start(1000);

        // Enable movement
        foreach(Vehicle* v, aliveVehicles){
            v->movementTimer->start(33);
        }

        // Run anti collision algorithm
        if(settingsPanel->algorithmEnabled) algorithm->run();

    }else{
        playButton->setText("Start");
        playButton->setColor(Qt::darkGreen);
        vehicleSpawnTimer->stop();
        timer->stop();
    }
}


void Simulation::destroyAllVehicles(){
    foreach(Vehicle* v, aliveVehicles){
        v->selfDestruct();
    }
}

bool Simulation::destroyCollidingVehicles(Vehicle* car){
    QList<QGraphicsItem *> list = scene->collidingItems(car);
    foreach(QGraphicsItem* i, list){
        Vehicle * item= dynamic_cast<Vehicle *>(i);
        if (item)
        {
            item->selfDestruct();
            statisticsPanel->incrementCollisions();
            return true;
        }

    }
    return false;
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
    settingsPanel = new SettingsPanel(screenWidth, screenHeight, btnPadding, algorithmEnabled, soundEffectsEnabled, vehiclesPerSec, initialSpeedRangeLowerBound, initialSpeedRangeUpperBound, timerLimit);
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
    bottomPanel = drawPanel(bottomPanelX, bottomPanelY, bottomPanelW, bottomPanelH, Qt::black, 1);

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

    endSimButton = new Button(QString("Finish"), Qt::red, 20,endSimBtnW, endSimBtnH, 0, 0, bottomPanel);
    endSimButton->setPos(endSimBtnX,endSimBtnY);
    //connect(resultsButton,SIGNAL(clicked()),this,SLOT(startToggle()));

    //Resuls button in Bottom Panel
    int resultsBtnX = playBtnX + btnPadding;
    int resultsBtnY = playBtnY + playBtnH + btnPadding;
    int resultsBtnW = playBtnW + endSimBtnW - btnPadding;
    int resultsBtnH = (bottomPanelH/3) - (btnPadding*2);

    resultsButton = new Button(QString("Results"), Qt::yellow, 20, resultsBtnW, resultsBtnH, 0, 0, bottomPanel);
    resultsButton->setPos(resultsBtnX,resultsBtnY);
    //connect(resultsButton,SIGNAL(clicked()),this,SLOT(startToggle()));

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

    int timerX = 4*btnPadding;
    int timerY = screenWidth;
    displayTimer = new QGraphicsTextItem("00:00", bottomPanel);
    displayTimer->setPos(timerX, timerY);
    displayTimer->setFont(f);
    displayTimer->setDefaultTextColor(Qt::white);
}

void Simulation::incrementTimer(){
    seconds++;

    int ss = seconds%60;
    int mm = seconds/60;
    QString minutes = mm < 10 ? "0" + QString::number(mm) : QString::number(mm);
    QString seconds = ss < 10 ? "0" + QString::number(ss) : QString::number(ss);
    displayTimer->setPlainText(minutes + ":" + seconds);

    // NOTE: should finish the simulation and open records panel in the future
    if(mm==settingsPanel->timerLimit) startToggle();
}

void Simulation::resetTimer(){
    timer->stop();
    seconds= 0;
    displayTimer->setPlainText("00:00");
}
