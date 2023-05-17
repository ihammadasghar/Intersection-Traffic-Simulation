#include "StatisticsPanel.h"
#include "Simulation.h"

extern Simulation * simulation;

StatisticsPanel::StatisticsPanel(int screenWidth, int screenHeight, int btnPadding, QGraphicsItem *parent): QObject(), QGraphicsRectItem(parent){
    collisons = 0;
    collisonsAvoided=0;
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

    QGraphicsTextItem** statisticsTextsItems[4] = {&collisonsDisplay, &collisonsAvoidedDisplay, &totalCarsSpawnedDisplay, &carsOnScreenDisplay};
    QString statisticsTexts[statisticsTextSize] = {"Collisons: ", "Collisons Avoided: ", "Cars On Screen: ", "Total Cars Spawned: "};
    int statisticsValues[statisticsTextSize] = {collisons, collisonsAvoided, carsOnScreen, totalCarsSpawned};

    for(int i=0; i<statisticsTextSize; i++){
        *statisticsTextsItems[i] = new QGraphicsTextItem(QString(statisticsTexts[i]) + QString::number(statisticsValues[i]), this);
        (*statisticsTextsItems[i])->setPos(statisticsPanelX +btnPadding, statisticsPanelY + (i*statisticsPanelH/4) );
        (*statisticsTextsItems[i])->setFont(f);
        (*statisticsTextsItems[i])->setDefaultTextColor(Qt::white);
    }

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
    carsOnScreen--;
    carsOnScreenDisplay->setPlainText(QString("Cars On Screen: ") + QString::number(carsOnScreen));
}
