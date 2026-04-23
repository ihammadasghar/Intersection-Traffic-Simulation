#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H

#include <QObject>
#include <QGraphicsRectItem>

class QGraphicsTextItem;
class Button;

class SettingsPanel : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    SettingsPanel(int screenWidth, int screenHeight, int btnPadding,
                  QGraphicsItem* parent = nullptr);

    // --- Simulation parameters ---
    int vehiclesPerSec;
    int speedRangeLowerBound;
    int speedRangeUpperBound;
    int timerLimit;
    bool vehicleDetailsEnabled;

    // --- Safety strategies ---
    bool algorithmEnabled;
    bool trafficLightsEnabled;
    bool stopSignEnabled;
    bool speedBumpEnabled;
    bool yieldEnabled;
    bool speedLimitEnabled;

signals:
    void restartRequested();
    void algorithmToggled(bool enabled);

public slots:
    void toggle();

private slots:
    // Strategy toggles
    void toggleAlgorithm();
    void toggleTrafficLights();
    void toggleStopSign();
    void toggleSpeedBump();
    void toggleYield();
    void toggleSpeedLimit();

    // Parameter adjustments
    void toggleVehicleDetails();
    void incrementVehiclesPerSec();
    void decrementVehiclesPerSec();
    void incrementTimerLimit();
    void decrementTimerLimit();
    void incrementSpeedRangeLowerBound();
    void decrementSpeedRangeLowerBound();
    void incrementSpeedRangeUpperBound();
    void decrementSpeedRangeUpperBound();
    void restart();

private:
    void updateSpeedDisplay();
    QGraphicsTextItem* createSectionHeader(const QString& text, int x, int y);
    QGraphicsTextItem* createSettingLabel(const QString& text, int x, int y);
    Button* createToggleButton(const QString& label, int x, int y, int w, int h, bool initialState);

    int panelX, panelY, panelW, panelH;

    // Strategy display labels
    QGraphicsTextItem* algorithmLabel;
    QGraphicsTextItem* trafficLightsLabel;
    QGraphicsTextItem* stopSignLabel;
    QGraphicsTextItem* speedBumpLabel;
    QGraphicsTextItem* yieldLabel;
    QGraphicsTextItem* speedLimitLabel;

    // Toggle buttons (for visual state tracking)
    Button* algorithmBtn;
    Button* trafficLightsBtn;
    Button* stopSignBtn;
    Button* speedBumpBtn;
    Button* yieldBtn;
    Button* speedLimitBtn;

    // Parameter display labels
    QGraphicsTextItem* vehiclesPerSecLabel;
    QGraphicsTextItem* vehicleDetailsLabel;
    QGraphicsTextItem* speedRangeLabel;
    QGraphicsTextItem* timerLimitLabel;
};

#endif // SETTINGSPANEL_H
