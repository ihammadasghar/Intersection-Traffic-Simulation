#ifndef VEHICLE_H
#define VEHICLE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class vehicle; }
QT_END_NAMESPACE

class vehicle : public QMainWindow
{
    Q_OBJECT

public:
    vehicle(QWidget *parent = nullptr);
    ~vehicle();

private:
    Ui::vehicle *ui;
};
#endif // VEHICLE_H
