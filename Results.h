#ifndef RESULTS_H
#define RESULTS_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsTextItem>

class Results: public QObject,public QGraphicsRectItem{
    Q_OBJECT
public:
    Results(int screenWidth, int screenHeight, int btnPadding, QGraphicsItem * parent=NULL);
    QList<QList<QString>> recordsList;
    QGraphicsTextItem* numberColDisplay;
    QGraphicsTextItem* collisionsColDisplay;
    QGraphicsTextItem* algorithmColDisplay;
    QGraphicsTextItem* carsColDisplay;
    QGraphicsTextItem* cpsColDisplay;
    QGraphicsTextItem* speedColDisplay;
    QGraphicsTextItem* percentageColDisplay;
    int panelPadding;
    int ResultsX;
    int ResultsY;


public slots:
    void toggleResults();
    void addRecord();
    void drawRecords();

};

#endif // RESULTS_H

