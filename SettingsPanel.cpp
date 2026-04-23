#include "SettingsPanel.h"
#include "Button.h"
#include "Constants.h"
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QFont>

SettingsPanel::SettingsPanel(int screenWidth, int screenHeight, int btnPadding,
                             QGraphicsItem* parent)
    : QObject()
    , QGraphicsRectItem(parent)
{
    // Initialize defaults
    algorithmEnabled = kDefaultAlgorithmEnabled;
    vehicleDetailsEnabled = kDefaultVehicleDetailsEnabled;
    vehiclesPerSec = kDefaultVehiclesPerSec;
    speedRangeLowerBound = kDefaultSpeedRangeLower;
    speedRangeUpperBound = kDefaultSpeedRangeUpper;
    timerLimit = kDefaultTimerLimit;
    trafficLightsEnabled = false;
    stopSignEnabled = false;
    speedBumpEnabled = false;
    yieldEnabled = false;
    speedLimitEnabled = false;

    // Panel dimensions — slightly wider for new content
    panelX = screenWidth / 12;
    panelY = screenHeight / 14;
    panelW = screenWidth - (screenWidth / 6);
    panelH = screenHeight - (screenHeight / 7) - (screenHeight / 4);
    setRect(panelX, panelY, panelW, panelH);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Colors::panelDark());
    setBrush(brush);
    QPen pen(QColor(255, 255, 255, 30));
    pen.setWidth(1);
    setPen(pen);
    setOpacity(0.95);
    setZValue(3);

    int padding = btnPadding;
    int contentX = panelX + padding;
    int rightColX = panelX + (panelW / 2) + padding;
    int toggleBtnW = (panelW / 2) - (padding * 3);
    int rowHeight = 38;
    int curY = panelY + padding;

    // ========== TITLE ==========
    QFont titleFont("Segoe UI", 22);
    titleFont.setWeight(QFont::Bold);
    QGraphicsTextItem* title = new QGraphicsTextItem("Simulation Settings", this);
    title->setPos(contentX + panelW / 6, curY);
    title->setFont(titleFont);
    title->setDefaultTextColor(Colors::textPrimary());
    curY += 50;

    // ========== SECTION: Safety Strategies ==========
    createSectionHeader("Safety Strategies", contentX, curY);
    curY += 32;

    // --- Traffic Lights ---
    trafficLightsLabel = createSettingLabel("Traffic Lights: Off", contentX, curY);
    trafficLightsBtn = createToggleButton("Off", rightColX, curY, toggleBtnW, rowHeight - 4, false);
    connect(trafficLightsBtn, &Button::clicked, this, &SettingsPanel::toggleTrafficLights);
    curY += rowHeight;

    // --- Stop Signs ---
    stopSignLabel = createSettingLabel("Stop Signs: Off", contentX, curY);
    stopSignBtn = createToggleButton("Off", rightColX, curY, toggleBtnW, rowHeight - 4, false);
    connect(stopSignBtn, &Button::clicked, this, &SettingsPanel::toggleStopSign);
    curY += rowHeight;

    // --- Speed Bumps ---
    speedBumpLabel = createSettingLabel("Speed Bumps: Off", contentX, curY);
    speedBumpBtn = createToggleButton("Off", rightColX, curY, toggleBtnW, rowHeight - 4, false);
    connect(speedBumpBtn, &Button::clicked, this, &SettingsPanel::toggleSpeedBump);
    curY += rowHeight;

    // --- Yield Signs ---
    yieldLabel = createSettingLabel("Yield Signs: Off", contentX, curY);
    yieldBtn = createToggleButton("Off", rightColX, curY, toggleBtnW, rowHeight - 4, false);
    connect(yieldBtn, &Button::clicked, this, &SettingsPanel::toggleYield);
    curY += rowHeight;

    // --- Speed Limit Zone ---
    speedLimitLabel = createSettingLabel("Speed Limit Zone: Off", contentX, curY);
    speedLimitBtn = createToggleButton("Off", rightColX, curY, toggleBtnW, rowHeight - 4, false);
    connect(speedLimitBtn, &Button::clicked, this, &SettingsPanel::toggleSpeedLimit);
    curY += rowHeight;

    // --- Anti-collision Algorithm ---
    algorithmLabel = createSettingLabel("Anti-collision AI: Off", contentX, curY);
    algorithmBtn = createToggleButton("Off", rightColX, curY, toggleBtnW, rowHeight - 4, false);
    connect(algorithmBtn, &Button::clicked, this, &SettingsPanel::toggleAlgorithm);
    curY += rowHeight + 10;

    // ========== SECTION: Simulation Parameters ==========
    createSectionHeader("Simulation Parameters", contentX, curY);
    curY += 32;

    // --- Vehicles Per Second ---
    vehiclesPerSecLabel = createSettingLabel(
        "Cars/Second: " + QString::number(vehiclesPerSec), contentX, curY);
    int halfBtnW = (toggleBtnW - padding) / 2;
    Button* vpsInc = new Button("+", Colors::accentGreen(), 16, halfBtnW, rowHeight - 4, 0, 0, this);
    vpsInc->setPos(rightColX, curY);
    connect(vpsInc, &Button::clicked, this, &SettingsPanel::incrementVehiclesPerSec);
    Button* vpsDec = new Button("-", Colors::accentRed(), 16, halfBtnW, rowHeight - 4, 0, 0, this);
    vpsDec->setPos(rightColX + halfBtnW + padding, curY);
    connect(vpsDec, &Button::clicked, this, &SettingsPanel::decrementVehiclesPerSec);
    curY += rowHeight;

    // --- Speed Range ---
    speedRangeLabel = createSettingLabel(
        "Speed: " + QString::number(speedRangeLowerBound) + " - " + QString::number(speedRangeUpperBound),
        contentX, curY);
    int qBtnW = (toggleBtnW - padding * 3) / 4;
    Button* sLInc = new Button("+", Colors::accentGreen(), 14, qBtnW, rowHeight - 4, 0, 0, this);
    sLInc->setPos(rightColX, curY);
    connect(sLInc, &Button::clicked, this, &SettingsPanel::incrementSpeedRangeLowerBound);
    Button* sLDec = new Button("-", Colors::accentRed(), 14, qBtnW, rowHeight - 4, 0, 0, this);
    sLDec->setPos(rightColX + qBtnW + padding, curY);
    connect(sLDec, &Button::clicked, this, &SettingsPanel::decrementSpeedRangeLowerBound);
    Button* sUInc = new Button("+", Colors::accentGreen(), 14, qBtnW, rowHeight - 4, 0, 0, this);
    sUInc->setPos(rightColX + (qBtnW + padding) * 2, curY);
    connect(sUInc, &Button::clicked, this, &SettingsPanel::incrementSpeedRangeUpperBound);
    Button* sUDec = new Button("-", Colors::accentRed(), 14, qBtnW, rowHeight - 4, 0, 0, this);
    sUDec->setPos(rightColX + (qBtnW + padding) * 3, curY);
    connect(sUDec, &Button::clicked, this, &SettingsPanel::decrementSpeedRangeUpperBound);
    curY += rowHeight;

    // --- Timer Limit ---
    timerLimitLabel = createSettingLabel(
        "Timer Limit: " + QString::number(timerLimit) + " min", contentX, curY);
    Button* tlInc = new Button("+", Colors::accentGreen(), 16, halfBtnW, rowHeight - 4, 0, 0, this);
    tlInc->setPos(rightColX, curY);
    connect(tlInc, &Button::clicked, this, &SettingsPanel::incrementTimerLimit);
    Button* tlDec = new Button("-", Colors::accentRed(), 16, halfBtnW, rowHeight - 4, 0, 0, this);
    tlDec->setPos(rightColX + halfBtnW + padding, curY);
    connect(tlDec, &Button::clicked, this, &SettingsPanel::decrementTimerLimit);
    curY += rowHeight;

    // --- Vehicle Details ---
    vehicleDetailsLabel = createSettingLabel("Vehicle Details: Off", contentX, curY);
    Button* vdBtn = createToggleButton("Off", rightColX, curY, toggleBtnW, rowHeight - 4, false);
    connect(vdBtn, &Button::clicked, this, &SettingsPanel::toggleVehicleDetails);
    curY += rowHeight + 15;

    // ========== Restart Button ==========
    int restartBtnW = panelW / 2;
    int restartBtnX = panelX + (panelW - restartBtnW) / 2;
    Button* restartBtn = new Button("Restart Simulation", Colors::accentOrange(), 14,
                                     restartBtnW, 40, 0, 0, this);
    restartBtn->setPos(restartBtnX, curY);
    connect(restartBtn, &Button::clicked, this, &SettingsPanel::restart);

    setVisible(false);
}

// --- Helper: create section header ---
QGraphicsTextItem* SettingsPanel::createSectionHeader(const QString& text, int x, int y)
{
    QFont headerFont("Segoe UI", 14);
    headerFont.setWeight(QFont::Bold);
    QGraphicsTextItem* header = new QGraphicsTextItem(text, this);
    header->setPos(x, y);
    header->setFont(headerFont);
    header->setDefaultTextColor(Colors::accentBlue());

    // Divider line under header
    QGraphicsRectItem* divider = new QGraphicsRectItem(
        x, y + 26, panelW - 2 * kBtnPadding, 1, this);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(255, 255, 255, 40));
    divider->setBrush(brush);
    divider->setPen(Qt::NoPen);

    return header;
}

// --- Helper: create setting label ---
QGraphicsTextItem* SettingsPanel::createSettingLabel(const QString& text, int x, int y)
{
    QFont settingFont("Segoe UI", 13);
    QGraphicsTextItem* label = new QGraphicsTextItem(text, this);
    label->setPos(x, y + 4);
    label->setFont(settingFont);
    label->setDefaultTextColor(Colors::textPrimary());
    return label;
}

// --- Helper: create toggle button ---
Button* SettingsPanel::createToggleButton(const QString& label, int x, int y, int w, int h, bool initialState)
{
    Button* btn = new Button(label, Colors::btnDefault(), 12, w, h, 0, 0, this);
    btn->setToggleMode(true);
    btn->setPos(x, y);
    if (initialState)
        btn->setToggled(true);
    return btn;
}

// --- Slots ---

void SettingsPanel::toggle()
{
    setVisible(!isVisible());
}

void SettingsPanel::restart()
{
    emit restartRequested();
    toggle();
}

void SettingsPanel::toggleAlgorithm()
{
    algorithmEnabled = algorithmBtn->isToggled();
    algorithmLabel->setPlainText(QString("Anti-collision AI: ") + (algorithmEnabled ? "On" : "Off"));
    algorithmBtn->setText(algorithmEnabled ? "On" : "Off");
    emit algorithmToggled(algorithmEnabled);
}

void SettingsPanel::toggleTrafficLights()
{
    trafficLightsEnabled = trafficLightsBtn->isToggled();
    trafficLightsLabel->setPlainText(QString("Traffic Lights: ") + (trafficLightsEnabled ? "On" : "Off"));
    trafficLightsBtn->setText(trafficLightsEnabled ? "On" : "Off");
}

void SettingsPanel::toggleStopSign()
{
    stopSignEnabled = stopSignBtn->isToggled();
    stopSignLabel->setPlainText(QString("Stop Signs: ") + (stopSignEnabled ? "On" : "Off"));
    stopSignBtn->setText(stopSignEnabled ? "On" : "Off");
}

void SettingsPanel::toggleSpeedBump()
{
    speedBumpEnabled = speedBumpBtn->isToggled();
    speedBumpLabel->setPlainText(QString("Speed Bumps: ") + (speedBumpEnabled ? "On" : "Off"));
    speedBumpBtn->setText(speedBumpEnabled ? "On" : "Off");
}

void SettingsPanel::toggleYield()
{
    yieldEnabled = yieldBtn->isToggled();
    yieldLabel->setPlainText(QString("Yield Signs: ") + (yieldEnabled ? "On" : "Off"));
    yieldBtn->setText(yieldEnabled ? "On" : "Off");
}

void SettingsPanel::toggleSpeedLimit()
{
    speedLimitEnabled = speedLimitBtn->isToggled();
    speedLimitLabel->setPlainText(QString("Speed Limit Zone: ") + (speedLimitEnabled ? "On" : "Off"));
    speedLimitBtn->setText(speedLimitEnabled ? "On" : "Off");
}

void SettingsPanel::toggleVehicleDetails()
{
    vehicleDetailsEnabled = !vehicleDetailsEnabled;
    vehicleDetailsLabel->setPlainText(
        QString("Vehicle Details: ") + (vehicleDetailsEnabled ? "On" : "Off"));
}

void SettingsPanel::incrementVehiclesPerSec()
{
    vehiclesPerSec++;
    vehiclesPerSecLabel->setPlainText("Cars/Second: " + QString::number(vehiclesPerSec));
}

void SettingsPanel::decrementVehiclesPerSec()
{
    if (vehiclesPerSec == 0) return;
    vehiclesPerSec--;
    vehiclesPerSecLabel->setPlainText("Cars/Second: " + QString::number(vehiclesPerSec));
}

void SettingsPanel::incrementTimerLimit()
{
    timerLimit++;
    timerLimitLabel->setPlainText("Timer Limit: " + QString::number(timerLimit) + " min");
}

void SettingsPanel::decrementTimerLimit()
{
    if (timerLimit == 1) return;
    timerLimit--;
    timerLimitLabel->setPlainText("Timer Limit: " + QString::number(timerLimit) + " min");
}

void SettingsPanel::incrementSpeedRangeLowerBound()
{
    if (speedRangeUpperBound >= speedRangeLowerBound + 1) {
        speedRangeLowerBound++;
        updateSpeedDisplay();
    }
}

void SettingsPanel::decrementSpeedRangeLowerBound()
{
    if (speedRangeLowerBound == 0) return;
    speedRangeLowerBound--;
    updateSpeedDisplay();
}

void SettingsPanel::incrementSpeedRangeUpperBound()
{
    speedRangeUpperBound++;
    updateSpeedDisplay();
}

void SettingsPanel::decrementSpeedRangeUpperBound()
{
    if (speedRangeLowerBound <= speedRangeUpperBound - 1) {
        speedRangeUpperBound--;
        updateSpeedDisplay();
    }
}

void SettingsPanel::updateSpeedDisplay()
{
    speedRangeLabel->setPlainText(
        "Speed: " + QString::number(speedRangeLowerBound)
        + " - " + QString::number(speedRangeUpperBound));
}
