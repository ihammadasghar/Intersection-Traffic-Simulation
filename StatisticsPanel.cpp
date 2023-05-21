#include "StatisticsPanel.h"
#include "Simulation.h"

extern Simulation * simulation;

StatisticsPanel::StatisticsPanel(int screenWidth, int screenHeight, int btnPadding, QGraphicsItem *parent): QObject(), QGraphicsRectItem(parent){
    collisions = 0;
    collisionsAvoided=0;
    carsOnScreen =0;
    totalCarsSpawned=0;

    // this panel
    QFont f;
    f.setPointSize(20);

    // this panel
    int statisticsPanelX = 168;
    int statisticsPanelY = screenHeight - (screenHeight/4) +10;
    int statisticsPanelW = screenWidth - 180;
    int statisticsPanelH = (screenHeight)/3 -80;
    setRect(statisticsPanelX,statisticsPanelY,statisticsPanelW,statisticsPanelH);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::lightGray);
    setBrush(brush);
    setOpacity(0.7);
    setZValue(2);

    int statisticsTextSize = 4;

    QGraphicsTextItem** statisticsTextsItems[statisticsTextSize] = {&collisionsDisplay, &collisionsAvoidedDisplay, &totalCarsSpawnedDisplay, &carsOnScreenDisplay};
    QString statisticsTexts[statisticsTextSize] = {"Collisions: ", "Collisions Avoided: ", "Total Cars Spawned: ", "Cars On Screen: "};
    int statisticsValues[statisticsTextSize] = {collisions, collisionsAvoided, carsOnScreen, totalCarsSpawned};


    for(int i=0; i<statisticsTextSize; i++){
        *statisticsTextsItems[i] = new QGraphicsTextItem(QString(statisticsTexts[i]) + QString::number(statisticsValues[i]), this);
        (*statisticsTextsItems[i])->setPos(statisticsPanelX +btnPadding, statisticsPanelY + (i*statisticsPanelH/4) );
        (*statisticsTextsItems[i])->setFont(f);
        (*statisticsTextsItems[i])->setDefaultTextColor(Qt::white);
    }

}

void StatisticsPanel::reset(){
    collisions = 0;
    collisionsAvoided=0;
    carsOnScreen =0;
    totalCarsSpawned=0;

    int statisticsTextSize = 4;
    QGraphicsTextItem** statisticsTextsItems[statisticsTextSize] = {&collisionsDisplay, &collisionsAvoidedDisplay, &totalCarsSpawnedDisplay, &carsOnScreenDisplay};
    QString statisticsTexts[statisticsTextSize] = {"Collisions: ", "Collisions Avoided: ", "Total Cars Spawned: ", "Cars On Screen: "};
    int statisticsValues[statisticsTextSize] = {collisions, collisionsAvoided, carsOnScreen, totalCarsSpawned};

    for(int i=0; i<statisticsTextSize; i++){
        (*statisticsTextsItems[i])->setPlainText(QString(statisticsTexts[i]) + QString::number(statisticsValues[i]));
    }

}

void StatisticsPanel::incrementCollisions(){
    collisions++;
    collisionsDisplay->setPlainText(QString("Collisions: ") + QString::number(collisions));
}

void StatisticsPanel::incrementTotalCarsSpawned(){
    totalCarsSpawned++;
    totalCarsSpawnedDisplay->setPlainText(QString("Total Cars Spawned: ") + QString::number(totalCarsSpawned));
}

void StatisticsPanel::incrementCarsOnScreen(){
    carsOnScreen++;
    carsOnScreenDisplay->setPlainText(QString("Cars On Screen: ") + QString::number(carsOnScreen));
}

void StatisticsPanel::decrementCarsOnScreen(){
    if(carsOnScreen == 0) return;
    carsOnScreen--;
    carsOnScreenDisplay->setPlainText(QString("Cars On Screen: ") + QString::number(carsOnScreen));
}
