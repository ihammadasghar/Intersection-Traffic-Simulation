#ifndef RESULTS_H
#define RESULTS_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>

class Results: public QObject,public QGraphicsRectItem{
    Q_OBJECT
public:
    Results(int screenWidth, int screenHeight, int btnPadding, QGraphicsItem * parent=NULL);


public slots:
    void resultsPanel();


};

#endif // RESULTS_H
