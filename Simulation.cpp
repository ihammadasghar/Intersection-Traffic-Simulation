#include "Simulation.h"
#include "Vehicle.h"
#include "Button.h"
#include "SettingsPanel.h"
#include "StatisticsPanel.h"
#include "SpawnOption.h"
#include "Results.h"
#include "Algorithm.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QScreen>
#include <QApplication>
#include <QtMath>
#include <QPainter>
#include <QLinearGradient>

static QPixmap generateIntersectionBackground()
{
    QPixmap bg(kScreenWidth, kScreenHeight);
    QPainter p(&bg);
    p.setRenderHint(QPainter::Antialiasing);

    const int roadL = kIntersectionLeft;
    const int roadR = kIntersectionRight;
    const int roadT = kIntersectionTop;
    const int roadB = kIntersectionBottom;
    const int roadW = roadR - roadL;
    const int centerY = (roadT + roadB) / 2;
    const int centerX = (roadL + roadR) / 2;
    const int sw = 10;  // sidewalk width

    // ---- Grass background ----
    QLinearGradient grassGrad(0, 0, kScreenWidth, kRoadAreaBottom);
    grassGrad.setColorAt(0.0, QColor(82, 148, 58));
    grassGrad.setColorAt(0.5, QColor(75, 138, 52));
    grassGrad.setColorAt(1.0, QColor(68, 130, 48));
    p.fillRect(0, 0, kScreenWidth, kRoadAreaBottom, grassGrad);

    // Grass texture (subtle darker patches)
    p.setPen(Qt::NoPen);
    uint seed = 42;
    for (int i = 0; i < 400; i++) {
        seed = seed * 1103515245 + 12345;
        int rx = (seed >> 16) % kScreenWidth;
        seed = seed * 1103515245 + 12345;
        int ry = (seed >> 16) % kRoadAreaBottom;
        bool onRoad = (ry >= roadT && ry <= roadB) || (rx >= roadL && rx <= roadR);
        if (!onRoad) {
            p.setBrush(QColor(60, 120, 42, 40));
            p.drawEllipse(rx, ry, 4 + (seed >> 24) % 6, 3 + (seed >> 28) % 4);
        }
    }

    // Bottom panel area
    p.fillRect(0, kRoadAreaBottom, kScreenWidth, kScreenHeight - kRoadAreaBottom, Colors::panelDark());

    // ---- Sidewalks ----
    QColor sidewalkColor(195, 190, 180);
    QColor sidewalkDark(175, 170, 160);
    // Horizontal
    p.fillRect(0, roadT - sw, roadL - sw, sw, sidewalkColor);
    p.fillRect(roadR + sw, roadT - sw, kScreenWidth - roadR - sw, sw, sidewalkColor);
    p.fillRect(0, roadB, roadL - sw, sw, sidewalkColor);
    p.fillRect(roadR + sw, roadB, kScreenWidth - roadR - sw, sw, sidewalkColor);
    // Vertical
    p.fillRect(roadL - sw, 0, sw, roadT - sw, sidewalkColor);
    p.fillRect(roadR, 0, sw, roadT - sw, sidewalkColor);
    p.fillRect(roadL - sw, roadB + sw, sw, kRoadAreaBottom - roadB - sw, sidewalkColor);
    p.fillRect(roadR, roadB + sw, sw, kRoadAreaBottom - roadB - sw, sidewalkColor);

    // Corner sidewalk pieces (curved)
    QColor corners[] = { sidewalkColor, sidewalkDark };
    int cornerPositions[][2] = {
        {roadL - sw, roadT - sw}, {roadR, roadT - sw},
        {roadL - sw, roadB}, {roadR, roadB}
    };
    for (int i = 0; i < 4; i++) {
        p.fillRect(cornerPositions[i][0], cornerPositions[i][1], sw, sw, sidewalkColor);
    }

    // Curb lines
    QPen curbPen(QColor(140, 135, 125), 1);
    p.setPen(curbPen);
    p.drawLine(0, roadT - sw, roadL - sw, roadT - sw);
    p.drawLine(roadR + sw, roadT - sw, kScreenWidth, roadT - sw);
    p.drawLine(0, roadB + sw, roadL - sw, roadB + sw);
    p.drawLine(roadR + sw, roadB + sw, kScreenWidth, roadB + sw);
    p.drawLine(roadL - sw, 0, roadL - sw, roadT - sw);
    p.drawLine(roadR + sw, 0, roadR + sw, roadT - sw);
    p.drawLine(roadL - sw, roadB + sw, roadL - sw, kRoadAreaBottom);
    p.drawLine(roadR + sw, roadB + sw, roadR + sw, kRoadAreaBottom);

    // ---- Asphalt roads ----
    QColor asphalt(58, 60, 64);
    QColor asphaltDark(50, 52, 56);
    // Horizontal road
    QLinearGradient hRoadGrad(0, roadT, 0, roadB);
    hRoadGrad.setColorAt(0, asphaltDark);
    hRoadGrad.setColorAt(0.5, asphalt);
    hRoadGrad.setColorAt(1, asphaltDark);
    p.fillRect(0, roadT, kScreenWidth, roadW, hRoadGrad);
    // Vertical road
    QLinearGradient vRoadGrad(roadL, 0, roadR, 0);
    vRoadGrad.setColorAt(0, asphaltDark);
    vRoadGrad.setColorAt(0.5, asphalt);
    vRoadGrad.setColorAt(1, asphaltDark);
    p.fillRect(roadL, 0, roadW, kRoadAreaBottom, vRoadGrad);
    // Intersection fill (solid)
    p.fillRect(roadL, roadT, roadW, roadW, asphalt);

    // Asphalt texture (subtle speckles)
    p.setPen(Qt::NoPen);
    seed = 123;
    for (int i = 0; i < 600; i++) {
        seed = seed * 1103515245 + 12345;
        int rx = (seed >> 16) % kScreenWidth;
        seed = seed * 1103515245 + 12345;
        int ry = (seed >> 16) % kRoadAreaBottom;
        bool onHRoad = (ry >= roadT && ry <= roadB);
        bool onVRoad = (rx >= roadL && rx <= roadR && ry < kRoadAreaBottom);
        if (onHRoad || onVRoad) {
            int alpha = 15 + ((seed >> 24) % 20);
            p.setBrush(QColor(80, 82, 86, alpha));
            p.drawEllipse(rx, ry, 2, 2);
        }
    }

    // ---- Center dividers (yellow, dashed) ----
    QPen yellowDash(QColor(230, 190, 45), 2);
    yellowDash.setDashPattern({8, 6});
    p.setPen(yellowDash);
    // Horizontal center
    p.drawLine(0, centerY, roadL - 2, centerY);
    p.drawLine(roadR + 2, centerY, kScreenWidth, centerY);
    // Vertical center
    p.drawLine(centerX, 0, centerX, roadT - 2);
    p.drawLine(centerX, roadB + 2, centerX, kRoadAreaBottom);

    // ---- Lane dividers (white, dashed) ----
    int laneDiv1 = roadT + roadW / 4;
    int laneDiv2 = roadB - roadW / 4;
    QPen whiteDash(QColor(255, 255, 255, 160), 1.5);
    whiteDash.setDashPattern({5, 8});
    p.setPen(whiteDash);
    // Horizontal lanes
    p.drawLine(0, laneDiv1, roadL - 2, laneDiv1);
    p.drawLine(roadR + 2, laneDiv1, kScreenWidth, laneDiv1);
    p.drawLine(0, laneDiv2, roadL - 2, laneDiv2);
    p.drawLine(roadR + 2, laneDiv2, kScreenWidth, laneDiv2);
    // Vertical lanes
    p.drawLine(laneDiv1, 0, laneDiv1, roadT - 2);
    p.drawLine(laneDiv1, roadB + 2, laneDiv1, kRoadAreaBottom);
    p.drawLine(laneDiv2, 0, laneDiv2, roadT - 2);
    p.drawLine(laneDiv2, roadB + 2, laneDiv2, kRoadAreaBottom);

    // ---- Road edge lines (solid white) ----
    QPen edgePen(QColor(255, 255, 255, 180), 1.5);
    p.setPen(edgePen);
    p.drawLine(0, roadT + 1, roadL, roadT + 1);
    p.drawLine(roadR, roadT + 1, kScreenWidth, roadT + 1);
    p.drawLine(0, roadB - 1, roadL, roadB - 1);
    p.drawLine(roadR, roadB - 1, kScreenWidth, roadB - 1);
    p.drawLine(roadL + 1, 0, roadL + 1, roadT);
    p.drawLine(roadL + 1, roadB, roadL + 1, kRoadAreaBottom);
    p.drawLine(roadR - 1, 0, roadR - 1, roadT);
    p.drawLine(roadR - 1, roadB, roadR - 1, kRoadAreaBottom);

    // ---- Crosswalks (zebra stripes) ----
    p.setPen(Qt::NoPen);
    QColor zebraW(255, 255, 255, 170);
    int strW = 4, strGap = 5, cwDepth = 16;
    // Top crosswalk
    for (int x = roadL + 4; x < roadR - 4; x += strW + strGap)
        p.fillRect(x, roadT + 3, strW, cwDepth, zebraW);
    // Bottom crosswalk
    for (int x = roadL + 4; x < roadR - 4; x += strW + strGap)
        p.fillRect(x, roadB - cwDepth - 3, strW, cwDepth, zebraW);
    // Left crosswalk
    for (int y = roadT + 4; y < roadB - 4; y += strW + strGap)
        p.fillRect(roadL + 3, y, cwDepth, strW, zebraW);
    // Right crosswalk
    for (int y = roadT + 4; y < roadB - 4; y += strW + strGap)
        p.fillRect(roadR - cwDepth - 3, y, cwDepth, strW, zebraW);

    // ---- Stop lines ----
    QPen stopPen(QColor(255, 255, 255, 200), 2.5);
    p.setPen(stopPen);
    // Left approach (vehicles going right)
    p.drawLine(roadL - 2, roadT, roadL - 2, roadB);
    // Right approach (vehicles going left)
    p.drawLine(roadR + 2, roadT, roadR + 2, roadB);
    // Top approach (vehicles going down)
    p.drawLine(roadL, roadT - 2, roadR, roadT - 2);
    // Bottom approach (vehicles going up)
    p.drawLine(roadL, roadB + 2, roadR, roadB + 2);

    // ---- Decorative details ----
    // Manhole cover in intersection center
    p.setPen(QPen(QColor(80, 82, 86), 1));
    p.setBrush(QColor(70, 72, 76));
    p.drawEllipse(QPointF(centerX, centerY), 6, 6);
    p.setPen(QPen(QColor(90, 92, 96), 0.5));
    p.drawLine(centerX - 4, centerY, centerX + 4, centerY);
    p.drawLine(centerX, centerY - 4, centerX, centerY + 4);

    p.end();
    return bg;
}

Simulation::Simulation(QWidget* parent)
    : QGraphicsView(parent)
{
    // Create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, kScreenWidth, kScreenHeight);
    setBackgroundBrush(QBrush(generateIntersectionBackground()));

    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing);

    // Scale to fit user's screen with some margin
    QScreen* screen = QApplication::primaryScreen();
    QSize available = screen->availableSize();
    int marginH = 80;  // taskbar / title bar margin
    int marginW = 40;
    int maxW = available.width() - marginW;
    int maxH = available.height() - marginH;

    // Maintain aspect ratio
    double scaleW = static_cast<double>(maxW) / kScreenWidth;
    double scaleH = static_cast<double>(maxH) / kScreenHeight;
    double scaleFactor = qMin(scaleW, scaleH);
    if (scaleFactor > 1.0) scaleFactor = 1.0; // Don't upscale beyond native

    int viewW = static_cast<int>(kScreenWidth * scaleFactor);
    int viewH = static_cast<int>(kScreenHeight * scaleFactor);
    setMinimumSize(400, 530);
    resize(viewW, viewH);

    // Initial state
    previousSpawnOption = 0;
    greenLightIndex = 0;
    isStarted = false;
    seconds = 0;

    // Initialize spawn options
    spawnOptions[0]  = new SpawnOption(0,   310, 0,   Direction::Right, TurnDirection::Left);
    spawnOptions[1]  = new SpawnOption(0,   340, 0,   Direction::Right, TurnDirection::Right);
    spawnOptions[2]  = new SpawnOption(0,   310, 0,   Direction::Right, TurnDirection::Straight);
    spawnOptions[3]  = new SpawnOption(0,   340, 0,   Direction::Right, TurnDirection::Straight);
    spawnOptions[4]  = new SpawnOption(600, 260, 180, Direction::Left,  TurnDirection::Right);
    spawnOptions[5]  = new SpawnOption(600, 290, 180, Direction::Left,  TurnDirection::Left);
    spawnOptions[6]  = new SpawnOption(600, 260, 180, Direction::Left,  TurnDirection::Straight);
    spawnOptions[7]  = new SpawnOption(600, 290, 180, Direction::Left,  TurnDirection::Straight);
    spawnOptions[8]  = new SpawnOption(260, 0,   90,  Direction::Down,  TurnDirection::Right);
    spawnOptions[9]  = new SpawnOption(290, 0,   90,  Direction::Down,  TurnDirection::Left);
    spawnOptions[10] = new SpawnOption(260, 0,   90,  Direction::Down,  TurnDirection::Straight);
    spawnOptions[11] = new SpawnOption(290, 0,   90,  Direction::Down,  TurnDirection::Straight);
    spawnOptions[12] = new SpawnOption(310, 550, 270, Direction::Up,    TurnDirection::Left);
    spawnOptions[13] = new SpawnOption(340, 550, 270, Direction::Up,    TurnDirection::Right);
    spawnOptions[14] = new SpawnOption(310, 550, 270, Direction::Up,    TurnDirection::Straight);
    spawnOptions[15] = new SpawnOption(340, 550, 270, Direction::Up,    TurnDirection::Straight);

    // Algorithm
    algorithm = new Algorithm(this);

    // Setup timers
    vehicleSpawnTimer = new QTimer(this);
    connect(vehicleSpawnTimer, &QTimer::timeout, this, &Simulation::addVehicle);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Simulation::incrementTimer);

    drawGUI();
    show();
}

// --- Vehicle lifecycle ---

void Simulation::addVehicle()
{
    statisticsPanel->incrementTotalCarsSpawned();
    statisticsPanel->incrementCarsOnScreen();

    int picked = (rand() % kSpawnOptionCount);
    if (settingsPanel->trafficLightsEnabled) {
        Direction greenDir = kDirectionOrder[greenLightIndex];
        while (spawnOptions[picked]->initialDirection != greenDir
               || previousSpawnOption == picked) {
            picked = (rand() % kSpawnOptionCount);
        }
    }

    previousSpawnOption = picked;
    Vehicle* vehicle = new Vehicle(
        settingsPanel->speedRangeLowerBound,
        settingsPanel->speedRangeUpperBound,
        spawnOptions[picked],
        this
    );
    scene->addItem(vehicle);
    aliveVehicles.append(vehicle);
}

void Simulation::removeVehicle(Vehicle* vehicle)
{
    vehicle->movementTimer->stop();
    statisticsPanel->decrementCarsOnScreen();
    aliveVehicles.removeOne(vehicle);
    scene->removeItem(vehicle);
    vehicle->deleteLater();
}

void Simulation::onVehicleExitedRoad(Vehicle* vehicle)
{
    statisticsPanel->incrementCollisionsAvoided();
    removeVehicle(vehicle);
}

bool Simulation::handleCollisions(Vehicle* car)
{
    QList<QGraphicsItem*> colliding = scene->collidingItems(car);
    for (QGraphicsItem* item : colliding) {
        Vehicle* other = dynamic_cast<Vehicle*>(item);
        if (other) {
            removeVehicle(other);
            statisticsPanel->incrementCollisions();
            return true;
        }
    }
    return false;
}

void Simulation::destroyAllVehicles()
{
    while (!aliveVehicles.isEmpty()) {
        removeVehicle(aliveVehicles.first());
    }
}

// --- Simulation control ---

void Simulation::startToggle()
{
    isStarted = !isStarted;
    if (isStarted) {
        playButton->setText("Stop");
        playButton->setColor(Colors::accentRed());
        vehicleSpawnTimer->start(1000 / settingsPanel->vehiclesPerSec);
        timer->start(1000);

        for (Vehicle* v : aliveVehicles)
            v->movementTimer->start(kFrameIntervalMs);

        if (settingsPanel->algorithmEnabled)
            algorithm->run();
        if (results->isVisible())
            results->toggleResults();
    } else {
        playButton->setText("Start");
        playButton->setColor(Colors::accentGreen());
        vehicleSpawnTimer->stop();
        timer->stop();
    }
    updateStatusBar();
}

void Simulation::finishSimulation()
{
    results->addRecord(
        statisticsPanel->collisions,
        statisticsPanel->totalCarsSpawned,
        settingsPanel->vehiclesPerSec,
        settingsPanel->speedRangeLowerBound,
        settingsPanel->speedRangeUpperBound,
        getActiveStrategiesString()
    );
    results->toggleResults();
    resetTimer();
    statisticsPanel->reset();
    destroyAllVehicles();
    if (isStarted) startToggle();
    settingsPanel->toggle();
}

QString Simulation::getActiveStrategiesString() const
{
    QStringList strategies;
    if (settingsPanel->trafficLightsEnabled) strategies << "Lights";
    if (settingsPanel->stopSignEnabled) strategies << "Stop";
    if (settingsPanel->speedBumpEnabled) strategies << "Bumps";
    if (settingsPanel->yieldEnabled) strategies << "Yield";
    if (settingsPanel->speedLimitEnabled) strategies << "Limit";
    if (settingsPanel->algorithmEnabled) strategies << "AI";
    return strategies.isEmpty() ? "None" : strategies.join("+");
}

// --- Timer ---

void Simulation::incrementTimer()
{
    seconds++;
    if (seconds % kTrafficLightCycleSeconds == 0 && settingsPanel->trafficLightsEnabled)
        changeGreenLight();

    int ss = seconds % 60;
    int mm = seconds / 60;
    QString minutesStr = mm < 10 ? "0" + QString::number(mm) : QString::number(mm);
    QString secondsStr = ss < 10 ? "0" + QString::number(ss) : QString::number(ss);
    displayTimer->setPlainText(minutesStr + ":" + secondsStr);

    updateStatusBar();

    if (mm == settingsPanel->timerLimit)
        startToggle();
}

void Simulation::resetTimer()
{
    timer->stop();
    seconds = 0;
    displayTimer->setPlainText("00:00");
    updateStatusBar();
}

// --- Traffic lights ---

void Simulation::changeGreenLight()
{
    trafficLightButtons[greenLightIndex]->setColor(Colors::accentRed());
    greenLightIndex = (greenLightIndex + 1) % kNumDirections;
    trafficLightButtons[greenLightIndex]->setColor(Colors::accentGreen());

    Direction greenDir = kDirectionOrder[greenLightIndex];
    for (int i = 0; i < aliveVehicles.length(); i++) {
        if (aliveVehicles[i]->distanceCovered < 100
            && aliveVehicles[i]->spawnOption->initialDirection != greenDir) {
            aliveVehicles[i]->movementTimer->stop();
        } else {
            aliveVehicles[i]->movementTimer->start(kFrameIntervalMs);
        }
    }
}

// --- GUI ---

void Simulation::drawGUI()
{
    drawTrafficLights();

    // Settings panel
    settingsPanel = new SettingsPanel(kScreenWidth, kScreenHeight, kBtnPadding);
    scene->addItem(settingsPanel);

    // Connect settings panel signals
    connect(settingsPanel, &SettingsPanel::restartRequested, this, [this]() {
        resetTimer();
        statisticsPanel->reset();
        destroyAllVehicles();
        if (isStarted) startToggle();
    });
    connect(settingsPanel, &SettingsPanel::algorithmToggled, this, [this](bool enabled) {
        if (enabled) algorithm->run();
    });

    // Settings gear button
    int settingsBtnW = 45;
    int settingsBtnH = 45;
    int settingsBtnX = kScreenWidth - settingsBtnW - 8;
    int settingsBtnY = 8;
    Button* settingsBtn = new Button(QString("⚙️"), Colors::panelLight(), 18,
                                      settingsBtnW, settingsBtnH, 0, 0);
    settingsBtn->setPos(settingsBtnX, settingsBtnY);
    connect(settingsBtn, &Button::clicked, settingsPanel, &SettingsPanel::toggle);
    scene->addItem(settingsBtn);

    drawStatusBar();
    drawBottomPanel();
    drawTimerDisplay();
}

void Simulation::drawTrafficLights()
{
    // Vertical traffic lights (top and bottom of intersection)
    for (int i = 0; i < 2; i++) {
        Button* button = new Button("", Colors::accentRed(), 0, 22, 22, 0, 0);
        button->setPos(160, 400 - (i * 220));
        scene->addItem(button);
        trafficLightButtons.append(button);
    }

    // Horizontal traffic lights (left and right of intersection)
    for (int i = 0; i < 2; i++) {
        Button* button = new Button("", Colors::accentRed(), 0, 22, 22, 0, 0);
        button->setPos(420, 180 + (i * 220));
        scene->addItem(button);
        trafficLightButtons.append(button);
    }

    trafficLightButtons[0]->setColor(Colors::accentGreen());
}

void Simulation::drawStatusBar()
{
    // Top-left status bar showing active strategies
    statusBar = createPanel(0, 0, kScreenWidth, 32, Colors::panelDark(), 0.8);
    statusBar->setZValue(1);
    scene->addItem(statusBar);

    QFont f("Segoe UI", 10);
    statusText = new QGraphicsTextItem("Strategies: None | Status: Stopped", statusBar);
    statusText->setPos(10, 2);
    statusText->setFont(f);
    statusText->setDefaultTextColor(Colors::textSecondary());
}

void Simulation::updateStatusBar()
{
    QString strategies = getActiveStrategiesString();
    QString status = isStarted ? "Running" : "Stopped";
    statusText->setPlainText("Active: " + strategies + "  |  " + status);
}

void Simulation::drawBottomPanel()
{
    int panelX = 0;
    int panelY = kScreenHeight - (kScreenHeight / 4);
    int panelW = kScreenWidth;
    int panelH = kScreenHeight / 4;

    bottomPanel = createPanel(panelX, panelY, panelW, panelH, Colors::panelDark(), 1);
    bottomPanel->setZValue(1);

    // Play/Stop button
    int playBtnX = panelX + kBtnPadding;
    int playBtnY = kScreenHeight - static_cast<int>(panelH / 1.5);
    int playBtnW = 70;
    int playBtnH = 40;

    playButton = new Button("Start", Colors::accentGreen(), 14,
                             playBtnW, playBtnH, 0, 0, bottomPanel);
    playButton->setPos(playBtnX, playBtnY);
    connect(playButton, &Button::clicked, this, &Simulation::startToggle);

    // Finish button
    int endBtnX = playBtnX + playBtnW + kBtnPadding;
    int endBtnY = playBtnY;
    int endBtnW = 80;
    int endBtnH = 40;

    endSimButton = new Button("Finish", Colors::accentRed(), 14,
                               endBtnW, endBtnH, 0, 0, bottomPanel);
    endSimButton->setPos(endBtnX, endBtnY);
    connect(endSimButton, &Button::clicked, this, &Simulation::finishSimulation);

    // Results overlay
    results = new Results(kScreenWidth, kScreenHeight, kBtnPadding);
    scene->addItem(results);
    results->setVisible(false);

    // Results button
    int resultsBtnX = playBtnX;
    int resultsBtnY = playBtnY + playBtnH + kBtnPadding;
    int resultsBtnW = playBtnW + endBtnW + kBtnPadding;
    int resultsBtnH = 35;

    resultsButton = new Button("Results", Colors::accentPurple(), 13,
                                resultsBtnW, resultsBtnH, 0, 0, bottomPanel);
    resultsButton->setPos(resultsBtnX, resultsBtnY);
    connect(resultsButton, &Button::clicked, results, &Results::toggleResults);

    // Statistics panel
    statisticsPanel = new StatisticsPanel(kScreenWidth, kScreenHeight, kBtnPadding, bottomPanel);

    scene->addItem(bottomPanel);
}

void Simulation::drawTimerDisplay()
{
    QFont f("Segoe UI", 36);
    f.setWeight(QFont::Light);

    int timerX = kBtnPadding;
    int timerY = kScreenHeight - (kScreenHeight / 4);

    displayTimer = new QGraphicsTextItem("00:00", bottomPanel);
    displayTimer->setPos(timerX, timerY);
    displayTimer->setFont(f);
    displayTimer->setDefaultTextColor(Colors::textPrimary());
}

QGraphicsRectItem* Simulation::createPanel(int x, int y, int width, int height,
                                            QColor color, double opacity)
{
    QGraphicsRectItem* panel = new QGraphicsRectItem(x, y, width, height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    panel->setPen(Qt::NoPen);
    return panel;
}

void Simulation::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}
