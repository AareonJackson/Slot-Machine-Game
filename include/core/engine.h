#pragma once
#include "ui/game_window.h"
#include "core/rng.h"
#include "core/payout.h"
#include <memory>
#include <vector>
#include <string>

class Engine {
public:
    Engine();
    ~Engine();

    void run();

private:
    void update();
    void render();
    void spin();
    std::vector<std::vector<std::string>> generateSpinGrid();

    std::unique_ptr<GameWindow> m_window;

    RNG m_rng;
    PayoutCalculator m_payoutCalculator;

    double m_balance = 1000.0;
    double m_currentBet = 10.0;
    double m_lastWin = 0.0;
};
