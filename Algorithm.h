#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QString>
#include <QObject>
#include <QTimer>

class Algorithm: public QObject{
   Q_OBJECT
public:
    Algorithm(QString name);
    QString name;
    void run();
    QTimer* timer;

public slots:
    void intersectingLines();

};

#endif // ALGORITHM_H
