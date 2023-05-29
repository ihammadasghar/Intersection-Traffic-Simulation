#include "Results.h"
#include "Simulation.h"

extern Simulation * simulation;

Results::Results(int screenWidth, int screenHeight, int btnPadding, QGraphicsItem *parent): QObject(), QGraphicsRectItem(parent){

    // this panel
    int ResultsX = screenWidth/10;
    int ResultsY = screenHeight/10;
    int ResultsW = screenWidth - (screenWidth/5);
    int ResultsH = screenHeight - (screenHeight/5) - (screenHeight/4);
    setRect(ResultsX,ResultsY,ResultsW,ResultsH);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::yellow);
    setBrush(brush);
    setOpacity(0.7);
    setZValue(2);

    // Add settings to panel
    QFont f;
    f.setPointSize(20);

    int settingX = ResultsX + btnPadding;
    int settingY = ResultsY + btnPadding;


    this->setVisible(false);
}

void Results::resultsPanel(){
    setVisible(!isVisible());
}
