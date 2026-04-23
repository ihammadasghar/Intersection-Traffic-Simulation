#include "Vehicle.h"
#include "Simulation.h"
#include "SettingsPanel.h"
#include "StatisticsPanel.h"
#include "Constants.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QLinearGradient>
#include <cmath>

static QPixmap generateCarPixmap(const QColor& bodyColor, bool isTaxi = false)
{
    const int w = 40, h = 18;
    QPixmap pixmap(w + 3, h + 5);
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);
    p.setRenderHint(QPainter::Antialiasing);

    const int ox = 1, oy = 2;

    // Drop shadow
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 0, 0, 40));
    p.drawRoundedRect(ox + 2, oy + 2, w, h, 5, 3);

    // Car body
    QLinearGradient grad(0, oy, 0, oy + h);
    grad.setColorAt(0.0, bodyColor.lighter(118));
    grad.setColorAt(0.45, bodyColor);
    grad.setColorAt(1.0, bodyColor.darker(130));
    p.setBrush(grad);
    p.setPen(QPen(bodyColor.darker(160), 0.8));
    p.drawRoundedRect(ox, oy, w, h, 5, 3);

    // Windows
    QColor winColor(130, 190, 225, 200);
    p.setBrush(winColor);
    p.setPen(QPen(bodyColor.darker(140), 0.5));

    // Windshield (front)
    QPolygonF windshield;
    windshield << QPointF(ox + w * 0.68, oy + 3)
               << QPointF(ox + w * 0.80, oy + 2.5)
               << QPointF(ox + w * 0.80, oy + h - 2.5)
               << QPointF(ox + w * 0.68, oy + h - 3);
    p.drawPolygon(windshield);

    // Rear window
    QPolygonF rearWin;
    rearWin << QPointF(ox + w * 0.20, oy + 3)
            << QPointF(ox + w * 0.30, oy + 2.5)
            << QPointF(ox + w * 0.30, oy + h - 2.5)
            << QPointF(ox + w * 0.20, oy + h - 3);
    p.drawPolygon(rearWin);

    // Side windows
    p.drawRoundedRect(ox + w * 0.33, oy + 2.5, w * 0.32, h - 5, 1, 1);

    // Roof highlight
    p.setPen(QPen(QColor(255, 255, 255, 50), 0.8));
    p.drawLine(QPointF(ox + w * 0.34, oy + h * 0.45),
              QPointF(ox + w * 0.66, oy + h * 0.45));

    // Headlights (front)
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 210, 240));
    p.drawRoundedRect(ox + w - 3, oy + 2, 3, 3, 1, 1);
    p.drawRoundedRect(ox + w - 3, oy + h - 5, 3, 3, 1, 1);

    // Taillights (rear)
    p.setBrush(QColor(220, 30, 30, 230));
    p.drawRoundedRect(ox, oy + 2, 3, 3, 1, 1);
    p.drawRoundedRect(ox, oy + h - 5, 3, 3, 1, 1);

    // Wheels
    p.setBrush(QColor(25, 25, 25));
    p.drawRoundedRect(ox + 5, oy - 1, 8, 3, 1.5, 1);
    p.drawRoundedRect(ox + 5, oy + h - 2, 8, 3, 1.5, 1);
    p.drawRoundedRect(ox + w - 13, oy - 1, 8, 3, 1.5, 1);
    p.drawRoundedRect(ox + w - 13, oy + h - 2, 8, 3, 1.5, 1);

    // Hub caps
    p.setBrush(QColor(130, 130, 130));
    p.drawEllipse(QPointF(ox + 9, oy + 0.5), 1.5, 0.8);
    p.drawEllipse(QPointF(ox + 9, oy + h - 0.5), 1.5, 0.8);
    p.drawEllipse(QPointF(ox + w - 9, oy + 0.5), 1.5, 0.8);
    p.drawEllipse(QPointF(ox + w - 9, oy + h - 0.5), 1.5, 0.8);

    // Taxi roof sign
    if (isTaxi) {
        p.setBrush(QColor(255, 255, 240));
        p.setPen(QPen(QColor(180, 160, 60), 0.5));
        p.drawRoundedRect(ox + w * 0.42, oy + h * 0.25, w * 0.16, h * 0.5, 2, 2);
    }

    p.end();
    return pixmap;
}

Vehicle::Vehicle(int speedRangeLowerBound, int speedRangeUpperBound,
                 SpawnOption* spawnOption, Simulation* simulation,
                 QGraphicsItem* parent)
    : QObject()
    , QGraphicsPixmapItem(parent)
    , simulation(simulation)
{
    // Realistic car color palette
    struct CarStyle { QColor color; bool taxi; };
    CarStyle styles[] = {
        { QColor(190, 30, 35),  false },   // Crimson red
        { QColor(35, 55, 120),  false },   // Navy blue
        { QColor(235, 235, 230), false },  // Pearl white
        { QColor(170, 175, 180), false },  // Silver
        { QColor(40, 42, 45),   false },   // Charcoal black
        { QColor(245, 195, 50), true  },   // Taxi yellow
        { QColor(45, 110, 60),  false },   // Forest green
        { QColor(220, 120, 30), false },   // Burnt orange
    };
    int picked = rand() % 8;

    distanceCovered = 0;
    detailsText = nullptr;
    projectedX = 0;
    projectedY = 0;
    posX = spawnOption->initialX;
    posY = spawnOption->initialY;
    this->spawnOption = spawnOption;
    rotationAngle = spawnOption->initialRotation * M_PI / 180.0f;
    setRotation(spawnOption->initialRotation);
    setPos(posX, posY);
    setPixmap(generateCarPixmap(styles[picked].color, styles[picked].taxi));
    setTransformOriginPoint(0, 0);

    speed = (rand() % (speedRangeUpperBound - speedRangeLowerBound)) + speedRangeLowerBound;
    originalSpeed = speed;
    pixelsPerFrame = static_cast<float>(speed) / kTargetFPS;

    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &Vehicle::move);
    movementTimer->start(kFrameIntervalMs);
}

bool Vehicle::isInIntersection() const
{
    return posX >= kIntersectionLeft && posX <= kIntersectionRight
           && posY >= kIntersectionTop && posY <= kIntersectionBottom;
}

bool Vehicle::isApproachingIntersection() const
{
    switch (spawnOption->initialDirection) {
    case Direction::Right:
        return posX >= (kIntersectionLeft - kApproachZoneSize) && posX < kIntersectionLeft;
    case Direction::Left:
        return posX <= (kIntersectionRight + kApproachZoneSize) && posX > kIntersectionRight;
    case Direction::Down:
        return posY >= (kIntersectionTop - kApproachZoneSize) && posY < kIntersectionTop;
    case Direction::Up:
        return posY <= (kIntersectionBottom + kApproachZoneSize) && posY > kIntersectionBottom;
    }
    return false;
}

void Vehicle::applyStopSign()
{
    if (!simulation->settingsPanel->stopSignEnabled || hasStoppedAtSign)
        return;

    if (isApproachingIntersection() && distanceCovered > 50) {
        if (!stoppedAtSign) {
            stoppedAtSign = true;
            stopTimer = 0;
            movementTimer->stop();
            simulation->statisticsPanel->incrementStopSignStops();
        }
    }
}

void Vehicle::applySpeedBump()
{
    if (!simulation->settingsPanel->speedBumpEnabled)
        return;

    bool wasInZone = inSpeedBumpZone;
    inSpeedBumpZone = isApproachingIntersection();

    if (inSpeedBumpZone && !wasInZone) {
        // Entering speed bump zone — reduce speed significantly
        speed = static_cast<int>(originalSpeed * kSpeedBumpSpeedFactor);
        speed = qBound(kMinVehicleSpeed, speed, kMaxVehicleSpeed);
        pixelsPerFrame = static_cast<float>(speed) / kTargetFPS;
    } else if (!inSpeedBumpZone && wasInZone) {
        restoreSpeed();
    }
}

void Vehicle::applyYielding()
{
    if (!simulation->settingsPanel->yieldEnabled)
        return;

    if (!isApproachingIntersection()) {
        if (isYielding) {
            isYielding = false;
            restoreSpeed();
        }
        return;
    }

    // Check if any other vehicle is currently in the intersection
    for (Vehicle* other : simulation->aliveVehicles) {
        if (other == this) continue;
        if (other->isInIntersection()) {
            if (!isYielding) {
                isYielding = true;
                movementTimer->stop();
                simulation->statisticsPanel->incrementYieldEvents();
            }
            return;
        }
    }

    // No one in intersection — proceed
    if (isYielding) {
        isYielding = false;
        movementTimer->start(kFrameIntervalMs);
    }
}

void Vehicle::applySpeedLimit()
{
    if (!simulation->settingsPanel->speedLimitEnabled)
        return;

    bool wasInZone = inSpeedLimitZone;
    inSpeedLimitZone = isApproachingIntersection() || isInIntersection();

    if (inSpeedLimitZone && !wasInZone) {
        if (speed > kSpeedLimitZoneSpeed) {
            speed = kSpeedLimitZoneSpeed;
            pixelsPerFrame = static_cast<float>(speed) / kTargetFPS;
        }
    } else if (!inSpeedLimitZone && wasInZone) {
        restoreSpeed();
    }
}

void Vehicle::restoreSpeed()
{
    speed = originalSpeed;
    pixelsPerFrame = static_cast<float>(speed) / kTargetFPS;
}

void Vehicle::updateDetails()
{
    if (!simulation->settingsPanel->vehicleDetailsEnabled) {
        if (detailsText) {
            detailsText->setVisible(false);
            detailsText = nullptr;
        }
        return;
    }

    QString status;
    if (stoppedAtSign) status = " [STOPPED]";
    else if (isYielding) status = " [YIELDING]";
    else if (inSpeedBumpZone) status = " [SLOW]";
    else if (inSpeedLimitZone) status = " [LIMIT]";

    QString text = "X:" + QString::number(posX) + ", Y:" + QString::number(posY)
                   + "\nSpeed: " + QString::number(speed) + status;

    if (!detailsText) {
        QFont f("Segoe UI", 9);
        detailsText = new QGraphicsTextItem(text, this);
        detailsText->setPos(0, 20);
        detailsText->setFont(f);
        detailsText->setDefaultTextColor(Colors::accentYellow());
    }

    detailsText->setPlainText(text);
}

void Vehicle::move()
{
    if (!simulation->isStarted) {
        movementTimer->stop();
        return;
    }

    // Handle stop sign pause
    if (stoppedAtSign) {
        stopTimer += kFrameIntervalMs;
        if (stopTimer >= kStopSignPauseMs) {
            stoppedAtSign = false;
            hasStoppedAtSign = true;
            movementTimer->start(kFrameIntervalMs);
        } else {
            return; // Still waiting
        }
    }

    // Apply safety strategies
    applyStopSign();
    if (stoppedAtSign) return; // Just got stopped

    applyYielding();
    if (isYielding) return; // Yielding

    applySpeedBump();
    applySpeedLimit();

    // Detect collisions
    if (simulation->handleCollisions(this)) {
        simulation->removeVehicle(this);
        return;
    }

    // Curve functions for turning
    auto leftTurnSlope = [](float amplitude, float center, float x) {
        return -amplitude / pow(x - center, 2);
    };
    auto rightTurnSlope = [](float amplitude, float center, float width, float x) {
        return amplitude / pow(x + (2 * width) - center, 2);
    };

    // Calculate slope based on turn direction
    float slope = 0.0f;
    if (spawnOption->turnDirection == TurnDirection::Right)
        slope = rightTurnSlope(kCurveAmplitude, kCurveCenter, kCurveWidth, projectedX);
    else if (spawnOption->turnDirection == TurnDirection::Left)
        slope = leftTurnSlope(kCurveAmplitude, kCurveCenter, projectedX);

    float angle = atan(slope);
    float changeInX = pixelsPerFrame * cos(angle);
    float changeInY = pixelsPerFrame * sin(angle);
    projectedX += changeInX;
    projectedY += changeInY;

    rotationAngle = (spawnOption->initialRotation * M_PI / 180.0f) + angle;
    setRotation(rotationAngle * 180.0f / M_PI);

    // Map projected coordinates to screen based on initial direction
    switch (spawnOption->initialDirection) {
    case Direction::Right:
        posX = spawnOption->initialX + static_cast<int>(projectedX);
        posY = spawnOption->initialY + static_cast<int>(projectedY);
        break;
    case Direction::Left:
        posX = spawnOption->initialX - static_cast<int>(projectedX);
        posY = spawnOption->initialY - static_cast<int>(projectedY);
        break;
    case Direction::Up:
        posY = spawnOption->initialY - static_cast<int>(projectedX);
        posX = spawnOption->initialX + static_cast<int>(projectedY);
        break;
    case Direction::Down:
        posY = spawnOption->initialY + static_cast<int>(projectedX);
        posX = spawnOption->initialX - static_cast<int>(projectedY);
        break;
    }

    setPos(posX, posY);
    distanceCovered += pixelsPerFrame;
    updateDetails();

    // Destroy vehicle when it leaves the road area
    if (posY > kRoadAreaBottom || posY < 0 || posX < 0 || posX > kScreenWidth) {
        simulation->onVehicleExitedRoad(this);
        return;
    }
}

void Vehicle::changeSpeed(double acceleration)
{
    int newSpeed = speed + static_cast<int>(acceleration);
    speed = qBound(kMinVehicleSpeed, newSpeed, kMaxVehicleSpeed);
    pixelsPerFrame = static_cast<float>(speed) / kTargetFPS;
}
