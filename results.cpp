#include "Results.h"
#include "Simulation.h"

extern Simulation * simulation;

Results::Results(int screenWidth, int screenHeight, int btnPadding, QGraphicsItem *parent): QObject(), QGraphicsRectItem(parent){

    // this panel
    ResultsX = 0;
    ResultsY = 0;
    int ResultsW = screenWidth;
    int ResultsH = screenWidth;;
    setRect(ResultsX,ResultsY,ResultsW,ResultsH);
    panelPadding = 10;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::yellow);
    setBrush(brush);
    setOpacity(0.7);
    setZValue(2);

    // Add settings to panel
    QFont f;
    f.setPointSize(13);

    QGraphicsTextItem* statsVar[7] = {displaySim, displayCollisions , displayCollisionsAvoided, displayTotalCars,displayCarsPerSec, displaySpeedRange, displayPercentage};
    QString statsName[7] = {"Sim \nNumber","Total \nCollisions", "Collisions \n Avoided","Total \n Cars","CPS","Speed \n Range","Accuracy \n %"};

    for(int i =0; i<7; i++){
        statsVar[i] = new QGraphicsTextItem(QString(statsName[i]), this);
        statsVar[i]->setPos(ResultsX + 100*i/1.2, ResultsY+ panelPadding);
        statsVar[i]->setFont(f);
        statsVar[i]->setDefaultTextColor(Qt::black);
    }

    this->setVisible(false);
}

void Results::toggleResults(){
    drawRecords();
    setVisible(!isVisible());
}

void Results::addRecord(){
    QList<QString> record;
    int collisions = simulation->statisticsPanel->collisions;
    int totalCarsSpawned = simulation->statisticsPanel->totalCarsSpawned;

    displaySim->setPlainText(QString::number(recordsList.length()+1));
    record.append(QString(displaySim->toPlainText()));
    record.append(QString::number(collisions));
    record.append(QString::number(simulation->statisticsPanel->collisionsAvoided));
    record.append(QString::number(totalCarsSpawned));
    record.append(QString::number(simulation->settingsPanel->vehiclesPerSec));
    record.append(QString::number(simulation->settingsPanel->speedRangeLowerBound)+ " to " +QString::number(simulation->settingsPanel->speedRangeUpperBound));
    record.append(QString::number((((float)collisions/((float)totalCarsSpawned))*100)));

    recordsList.append(record);

}

void Results::drawRecords(){
    int recordsLen = recordsList.length();
    QString tempRecord;

    QGraphicsTextItem* statsVar[7] = {displaySim, displayCollisions , displayCollisionsAvoided, displayTotalCars,displayCarsPerSec, displaySpeedRange, displayPercentage};
    QString statsName[7] = {"Sim \nNumber","Total \nCollisions", "Collisions \n Avoided","Total \n Cars","CPS","Speed \n Range","Accuracy \n %"};

    for(int i=0; i<7; i++){
        for(int j=0; j<recordsLen; j++){
            tempRecord.append("\n");
            tempRecord.append(recordsList[i][j]);
        }
        //statsVar[i]->setPlainText(QString(statsName[i]+ "\n" +tempRecord));
        tempRecord.clear();
    }
}
