#ifndef RESULTS_H
#define RESULTS_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QList>
#include <QString>

class QGraphicsTextItem;

class Results : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    Results(int screenWidth, int screenHeight, int btnPadding,
            QGraphicsItem* parent = nullptr);

    void addRecord(int collisions, int totalCarsSpawned, int vehiclesPerSec,
                   int speedLower, int speedUpper, const QString& activeStrategies);

public slots:
    void toggleResults();

private:
    static constexpr int kColumnCount = 7;

    void drawRecords();

    QList<QList<QString>> recordsList;
    QGraphicsTextItem* columnDisplays[kColumnCount];
    QString columnNames[kColumnCount];
    int panelPadding;
};

#endif // RESULTS_H
