#include "Results.h"
#include "Constants.h"
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QFont>

Results::Results(int screenWidth, int screenHeight, int btnPadding,
                 QGraphicsItem* parent)
    : QObject()
    , QGraphicsRectItem(parent)
{
    int panelX = 0;
    int panelY = 0;
    int panelW = screenWidth;
    int panelH = screenWidth;
    setRect(panelX, panelY, panelW, panelH);
    panelPadding = 80;

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Colors::panelDark());
    setBrush(brush);
    QPen pen(QColor(255, 255, 255, 20));
    pen.setWidth(1);
    setPen(pen);
    setOpacity(0.95);
    setZValue(3);

    // Title
    QFont titleFont("Segoe UI", 20);
    titleFont.setWeight(QFont::Bold);
    QGraphicsTextItem* title = new QGraphicsTextItem("Simulation Results", this);
    title->setPos(panelW / 2 - 120, 20);
    title->setFont(titleFont);
    title->setDefaultTextColor(Colors::accentBlue());

    QFont f("Segoe UI", 11);

    columnNames[0] = "Run";
    columnNames[1] = "Collisions";
    columnNames[2] = "Cars";
    columnNames[3] = "CPS";
    columnNames[4] = "Speed";
    columnNames[5] = "Strategies";
    columnNames[6] = "Collision %";

    for (int col = 0; col < kColumnCount; col++) {
        columnDisplays[col] = new QGraphicsTextItem(columnNames[col] + "\n", this);
        columnDisplays[col]->setPos(panelPadding * col + 10, 65);
        columnDisplays[col]->setFont(f);

        // Color code: header gets accent, data gets primary
        if (col == 6)
            columnDisplays[col]->setDefaultTextColor(Colors::accentRed());
        else if (col == 5)
            columnDisplays[col]->setDefaultTextColor(Colors::accentTeal());
        else
            columnDisplays[col]->setDefaultTextColor(Colors::textPrimary());
    }

    setVisible(false);
}

void Results::toggleResults()
{
    drawRecords();
    setVisible(!isVisible());
}

void Results::addRecord(int collisions, int totalCarsSpawned, int vehiclesPerSec,
                        int speedLower, int speedUpper, const QString& activeStrategies)
{
    QList<QString> record;
    record.append(QString::number(recordsList.length() + 1));
    record.append(QString::number(collisions));
    record.append(QString::number(totalCarsSpawned));
    record.append(QString::number(vehiclesPerSec));
    record.append(QString::number(speedLower) + "-" + QString::number(speedUpper));
    record.append(activeStrategies.isEmpty() ? "None" : activeStrategies);

    int percentage = totalCarsSpawned > 0
        ? static_cast<int>((static_cast<float>(collisions) / totalCarsSpawned) * 100)
        : 0;
    record.append(QString::number(percentage) + "%");

    recordsList.append(record);
}

void Results::drawRecords()
{
    for (int col = 0; col < kColumnCount; col++) {
        QString text = columnNames[col] + "\n";
        for (int row = 0; row < recordsList.length(); row++) {
            text += recordsList[row][col] + "\n";
        }
        columnDisplays[col]->setPlainText(text);
    }
}
