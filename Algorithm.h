#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QString>
#include <QObject>

class Algorithm: public QObject{
   Q_OBJECT
public:
    Algorithm(QString name);
    QString name;
    void run();

public slots:
    void intersectingLines();

};

#endif // ALGORITHM_H
