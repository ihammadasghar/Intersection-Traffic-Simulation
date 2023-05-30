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
    panelPadding = 80;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    setBrush(brush);
    setOpacity(0.7);
    setZValue(2);

    // Add settings to panel
    QFont f;
    f.setPointSize(13);

    QGraphicsTextItem** statsColDisplays[7] = {&numberColDisplay, &collisionsColDisplay, &carsColDisplay, &cpsColDisplay, &speedColDisplay, &algorithmColDisplay, &percentageColDisplay};
    QString statsColName[7] = {"No.","Collisions","Cars","CPS","Speed", "Algorithm", "Collision %"};

    for(int col =0; col<7; col++){
        QString tempText = statsColName[col] + "\n";
        for(int row = 0; row < recordsList.length(); row++){
            tempText += recordsList[row][col] + "\n";
        }
        (*statsColDisplays[col]) = new QGraphicsTextItem(tempText, this);
        (*statsColDisplays[col])->setPos(panelPadding*col, 80);
        (*statsColDisplays[col])->setFont(f);
        (*statsColDisplays[col])->setDefaultTextColor(Qt::white);
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

    record.append(QString::number(recordsList.length()+1));
    record.append(QString::number(collisions));
    record.append(QString::number(totalCarsSpawned));
    record.append(QString::number(simulation->settingsPanel->vehiclesPerSec));
    record.append(QString::number(simulation->settingsPanel->speedRangeLowerBound)+ " to " +QString::number(simulation->settingsPanel->speedRangeUpperBound));
    record.append(simulation->settingsPanel->algorithmEnabled ? QString("On") : QString("Off"));
    record.append(QString::number((((float)collisions/((float)totalCarsSpawned))*100)) + "%");

    recordsList.append(record);

}

void Results::drawRecords(){
    QGraphicsTextItem** statsColDisplays[7] = {&numberColDisplay, &collisionsColDisplay, &carsColDisplay, &cpsColDisplay, &speedColDisplay, &algorithmColDisplay, &percentageColDisplay};
    QString statsColName[7] = {"No.","Collisions","Cars","CPS","Speed", "Algorithm", "Collision %"};

    for(int col =0; col<7; col++){
        QString tempText = statsColName[col] + "\n";
        for(int row = 0; row < recordsList.length(); row++){
            tempText += recordsList[row][col] + "\n";
        }
        (*statsColDisplays[col])->setPlainText(tempText);
    }

}
