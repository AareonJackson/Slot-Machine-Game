#pragma once
#include "ui/game_window.h"
#include "core/rng.h"
#include "core/payout.h"
#include <SFML/System/Clock.hpp>
#include <memory>
#include <vector>
#include <string>

class Engine {
public:
    Engine();
    ~Engine();

    void run();

private:
    enum class GameState {
        Idle,
        Spinning
    };

    void update();
    void render();
    void spin();
    void finishSpin();
    void animateSpin();
    void increaseBet();
    void decreaseBet();
    void refreshStatusText();
    std::vector<std::vector<std::string>> generateSpinGrid();
    std::vector<std::vector<std::string>> generateRandomDisplayGrid();
    std::vector<std::vector<std::string>> generateAnimatedDisplayGrid(float elapsedSeconds);

    std::unique_ptr<GameWindow> m_window;

    RNG m_rng;
    PayoutCalculator m_payoutCalculator;

    GameState m_state = GameState::Idle;
    sf::Clock m_spinClock;
    std::vector<std::vector<std::string>> m_pendingSpinGrid;
    std::vector<bool> m_loggedStoppedReels;

    double m_balance = 1000.0;
    double m_currentBet = 10.0;
    double m_lastWin = 0.0;
    double m_minBet = 1.0;
    double m_maxBet = 100.0;
    double m_betStep = 5.0;
};
