#include "vehicle.h"
#include "./ui_vehicle.h"

vehicle::vehicle(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::vehicle)
{
    ui->setupUi(this);
}

vehicle::~vehicle()
{
    delete ui;
}

