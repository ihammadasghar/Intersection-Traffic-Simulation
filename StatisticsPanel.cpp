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
    f.setPointSize(16);

    // this panel
    int statisticsPanelX = 200;
    int statisticsPanelY = screenHeight - (screenHeight/4) +10;
    int statisticsPanelW = screenWidth - 220;
    int statisticsPanelH = (screenHeight)/3 -80;
    setRect(statisticsPanelX,statisticsPanelY,statisticsPanelW,statisticsPanelH);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::lightGray);
    setBrush(brush);
    setOpacity(0.7);
    setZValue(2);

    int statisticsTextSize = 5;
    QGraphicsTextItem** statisticsTextsItems[statisticsTextSize] = {&collisionPercentageDisplay, &collisionsDisplay, &collisionsAvoidedDisplay, &totalCarsSpawnedDisplay, &carsOnScreenDisplay};
    QString statisticsTexts[statisticsTextSize] = {"Collision Percentage: ", "Collisions: ", "Collisions Avoided: ", "Total Cars Spawned: ", "Cars On Screen: "};
    QString statisticsValues[statisticsTextSize] = {"0%", QString::number(collisions), QString::number(collisionsAvoided), QString::number(carsOnScreen), QString::number(totalCarsSpawned)};


    for(int i=0; i<statisticsTextSize; i++){
        *statisticsTextsItems[i] = new QGraphicsTextItem(QString(statisticsTexts[i]) + (statisticsValues[i]), this);
        (*statisticsTextsItems[i])->setPos(statisticsPanelX +btnPadding, statisticsPanelY + (i*statisticsPanelH/statisticsTextSize) );
        (*statisticsTextsItems[i])->setFont(f);
        (*statisticsTextsItems[i])->setDefaultTextColor(Qt::white);
    }

}

void StatisticsPanel::reset(){
    collisions = 0;
    collisionsAvoided=0;
    carsOnScreen =0;
    totalCarsSpawned=0;

    int statisticsTextSize = 5;
    QGraphicsTextItem** statisticsTextsItems[statisticsTextSize] = {&collisionPercentageDisplay, &collisionsDisplay, &collisionsAvoidedDisplay, &totalCarsSpawnedDisplay, &carsOnScreenDisplay};
    QString statisticsTexts[statisticsTextSize] = {"Collision Percentage:", "Collisions: ", "Collisions Avoided: ", "Total Cars Spawned: ", "Cars On Screen: "};
    QString statisticsValues[statisticsTextSize] = {"0%", "0", "0", "0", "0"};

    for(int i=0; i<statisticsTextSize; i++){
        (*statisticsTextsItems[i])->setPlainText(QString(statisticsTexts[i]) + statisticsValues[i]);
    }

}

void StatisticsPanel::updateCollisionPercentage(){
    int collisionPercentage = (((float)collisions/((float)totalCarsSpawned))*100);
    collisionPercentageDisplay->setPlainText(QString("Collision Percentage: ") + QString::number(collisionPercentage) + "%");
}

void StatisticsPanel::incrementCollisionsAvoided(){
    collisionsAvoided++;
    collisionsAvoidedDisplay->setPlainText(QString("Collisions Avoided: ") + QString::number(collisionsAvoided));
}

void StatisticsPanel::incrementCollisions(){
    collisions++;
    collisionsDisplay->setPlainText(QString("Collisions: ") + QString::number(collisions));
    updateCollisionPercentage();
}

void StatisticsPanel::incrementTotalCarsSpawned(){
    totalCarsSpawned++;
    totalCarsSpawnedDisplay->setPlainText(QString("Total Cars Spawned: ") + QString::number(totalCarsSpawned));
    updateCollisionPercentage();
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
