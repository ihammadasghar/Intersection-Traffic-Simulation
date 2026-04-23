#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QColor>

// --- Screen dimensions ---
static constexpr int kScreenWidth = 600;
static constexpr int kScreenHeight = 800;
static constexpr int kRoadAreaBottom = 550;

// --- Intersection zone (center box) ---
static constexpr int kIntersectionLeft = 230;
static constexpr int kIntersectionRight = 370;
static constexpr int kIntersectionTop = 230;
static constexpr int kIntersectionBottom = 370;
// Approach zone: how far before intersection to start braking
static constexpr int kApproachZoneSize = 120;

// --- Timing ---
static constexpr int kTargetFPS = 30;
static constexpr int kFrameIntervalMs = 1000 / kTargetFPS;

// --- UI ---
static constexpr int kBtnPadding = 10;

// --- Default simulation settings ---
static constexpr int kDefaultSpeedRangeLower = 150;
static constexpr int kDefaultSpeedRangeUpper = 180;
static constexpr int kDefaultVehiclesPerSec = 2;
static constexpr int kDefaultTimerLimit = 1;
static constexpr bool kDefaultAlgorithmEnabled = false;
static constexpr bool kDefaultVehicleDetailsEnabled = false;

// --- Vehicle constraints ---
static constexpr int kMinVehicleSpeed = 80;
static constexpr int kMaxVehicleSpeed = 200;

// --- Traffic lights ---
static constexpr int kTrafficLightCycleSeconds = 5;
static constexpr int kNumDirections = 4;
static constexpr int kSpawnOptionCount = 16;

// --- Turn curve parameters ---
static constexpr float kCurveCenter = 320.0f;
static constexpr float kCurveAmplitude = 1700.0f;
static constexpr float kCurveWidth = 30.0f;

// --- Algorithm ---
static constexpr float kProjectionLineLength = 800.0f;
static constexpr double kAccelerationPerSec = 20.0;

// --- Safety strategy parameters ---
static constexpr int kStopSignPauseMs = 1500;       // How long vehicles stop at sign
static constexpr float kSpeedBumpSpeedFactor = 0.3f; // Speed reduced to 30% in bump zone
static constexpr int kSpeedLimitZoneSpeed = 100;     // Max speed in speed-limit zone
static constexpr float kYieldCheckRadius = 80.0f;    // How far to check for yielding

// --- Modern UI color palette ---
namespace Colors {
    // Panel backgrounds
    inline QColor panelDark()    { return QColor(24, 24, 36); }
    inline QColor panelMedium()  { return QColor(32, 34, 48); }
    inline QColor panelLight()   { return QColor(44, 46, 64); }

    // Accent colors
    inline QColor accentBlue()   { return QColor(66, 133, 244); }
    inline QColor accentGreen()  { return QColor(52, 199, 89); }
    inline QColor accentRed()    { return QColor(235, 77, 75); }
    inline QColor accentYellow() { return QColor(255, 196, 0); }
    inline QColor accentPurple() { return QColor(147, 112, 219); }
    inline QColor accentOrange() { return QColor(255, 149, 0); }
    inline QColor accentTeal()   { return QColor(48, 209, 188); }

    // Text
    inline QColor textPrimary()   { return QColor(240, 240, 245); }
    inline QColor textSecondary() { return QColor(160, 165, 180); }
    inline QColor textMuted()     { return QColor(100, 105, 120); }

    // Button states
    inline QColor btnDefault()   { return QColor(55, 58, 80); }
    inline QColor btnHover()     { return QColor(70, 74, 100); }
    inline QColor btnActive()    { return QColor(66, 133, 244); }
}

#endif // CONSTANTS_H
