#include "core/engine.h"
#include "config/config_manager.h"
#include <iostream>

Engine::Engine() {
    auto& config = ConfigManager::getInstance();

    if (!config.loadAllConfigs("assets/config")) {
        std::cerr << "Failed to load configuration files. Using fallback window settings." << std::endl;
        // initialize the window from the game_config.json
        m_window = std::make_unique<GameWindow>(800, 600, "The Slot Game");
    } else {
        const auto& gameConfig = config.getGameConfig();
        m_window = std::make_unique<GameWindow>(
            gameConfig.windowWidth,
            gameConfig.windowHeight,
            gameConfig.gameTitle
        );
    }
    refreshStatusText();

    m_window->setSpinCallback([this]() {
        spin();
    });

    m_window->setBetUpCallback([this]() {
        increaseBet();
    });

    m_window->setBetDownCallback([this]() {
        decreaseBet();
    });
}

Engine::~Engine() = default;

void Engine::run() {
    while (m_window->isOpen()) {
        m_window->pollEvents();
        update();
        render();
    }
}

void Engine::update() {
    if (m_state == GameState::Spinning) {
        animateSpin();

        if (m_spinClock.getElapsedTime().asSeconds() >= 1.9f) {
            finishSpin();
        }
    }
}

void Engine::render() {
    // Have the window draw itself
    m_window->render();
}

void Engine::spin() {
    if (m_state == GameState::Spinning) {
        return;
    }

    if (m_balance < m_currentBet) {
        std::cout << "Not enough money to spin! Balance: $" << m_balance << std::endl;
        return;
    }

    m_balance -= m_currentBet;
    m_lastWin = 0.0;
    refreshStatusText();

    m_pendingSpinGrid = generateSpinGrid();
    m_state = GameState::Spinning;
    m_spinClock.restart();

    std::cout << "Spin started. Bet: $" << m_currentBet
              << ", Balance: $" << m_balance << std::endl;
}

void Engine::finishSpin() {

    m_window->updateReels(m_pendingSpinGrid);

    std::vector<WinLine> wins = m_payoutCalculator.calculateWin(m_pendingSpinGrid, m_currentBet);

    double totalWin = 0.0;
    for (const auto& win : wins) {
        totalWin += win.win_amount;

        std::cout << "Winning payline " << win.payline_index << ": $" << win.match_count << "x " << win.symbol
        << " pays $" << win.win_amount << std::endl;
    }

    m_lastWin = totalWin;
    m_balance += totalWin;
    m_state = GameState::Idle;

    refreshStatusText();

    std::cout << "Spin complete. Bet: $" << m_currentBet
    << ", Win: $" << totalWin << ", Balance: $" << m_balance << std::endl;
}

void Engine::animateSpin() {
    float elapsedSeconds = m_spinClock.getElapsedTime().asSeconds();
    std::vector<std::vector<std::string>> animatedGrid = generateAnimatedDisplayGrid(elapsedSeconds);
    m_window->updateReels(animatedGrid);
}

void Engine::increaseBet() {
    if (m_state == GameState::Spinning) {
        return;
    }

    if (m_currentBet + m_betStep <= m_maxBet && m_currentBet + m_betStep <= m_balance) {
        m_currentBet += m_betStep;
        m_lastWin = 0.0;
        refreshStatusText();

        std::cout << "Bet increased to $" << m_currentBet << std::endl;
    }
}

void Engine::decreaseBet() {
    if (m_state == GameState::Spinning) {
        return;
    }

    if (m_currentBet - m_betStep >= m_minBet) {
        m_currentBet -= m_betStep;
        m_lastWin = 0.0;
        refreshStatusText();

        std::cout << "Bet decreased to $" << m_currentBet << std::endl;
    }
}

void Engine::refreshStatusText() {
    if (m_window) {
        m_window->updateStatusText(m_balance, m_currentBet, m_lastWin);
    }
}

std::vector<std::vector<std::string>> Engine::generateSpinGrid() {
    const auto& reelsConfig = ConfigManager::getInstance().getReelsConfig();

    std::vector<int> reelLengths;
    reelLengths.reserve(reelsConfig.reel_strips.size());

    for (const auto& reelStrip : reelsConfig.reel_strips) {
        reelLengths.push_back(static_cast<int>(reelStrip.size()));
    }

    std::vector<int> stops = m_rng.generateStops(reelLengths);

    std::vector<std::vector<std::string>> grid(
        reelsConfig.num_reels,
        std::vector<std::string>(reelsConfig.num_visible_rows)
    );

    for (int reel = 0; reel < reelsConfig.num_reels; ++reel) {
        const auto& reelStrip = reelsConfig.reel_strips[reel];
        int reelLength = static_cast<int>(reelStrip.size());

        for (int row = 0; row < reelsConfig.num_visible_rows; ++row) {
            int symbolIndex = (stops[reel] + row) % reelLength;
            grid[reel][row] = reelStrip[symbolIndex];
        }
    }
    return grid;
}

std::vector<std::vector<std::string>> Engine::generateRandomDisplayGrid() {
    const auto& reelsConfig = ConfigManager::getInstance().getReelsConfig();

    std::vector<std::vector<std::string>> grid(
        reelsConfig.num_reels,
        std::vector<std::string>(reelsConfig.num_visible_rows)
    );

    for (int reel = 0; reel < reelsConfig.num_reels; ++reel) {
        const auto& reelStrip = reelsConfig.reel_strips[reel];
        int reelLength = static_cast<int>(reelStrip.size());

        for (int row = 0; row < reelsConfig.num_visible_rows; ++row) {
            int randomIndex = m_rng.getRandomInt(0, reelLength - 1);
            grid[reel][row] = reelStrip[randomIndex];
        }
    }

    return grid;
}

std::vector<std::vector<std::string>> Engine::generateAnimatedDisplayGrid(float elapsedSeconds) {
    const auto& reelsConfig = ConfigManager::getInstance().getReelsConfig();

    std::vector<std::vector<std::string>> grid(
        reelsConfig.num_reels,
        std::vector<std::string>(reelsConfig.num_visible_rows)
    );

    const float firstReelStopTime = 0.8f;
    const float delayBetweenReels = 0.35f;

    for (int reel = 0; reel < reelsConfig.num_reels; ++reel) {
        const auto& reelStrip = reelsConfig.reel_strips[reel];
        int reelLength = static_cast<int>(reelStrip.size());

        float reelStopTime = firstReelStopTime + (delayBetweenReels);
        bool reelHasStopped = elapsedSeconds >= reelStopTime;

        for (int row = 0; row < reelsConfig.num_visible_rows; ++row) {
            if (reelHasStopped && !m_pendingSpinGrid.empty()) {
                grid[reel][row] = m_pendingSpinGrid[reel][row];
            } else {
                int randomIndex = m_rng.getRandomInt(0, reelLength - 1);
                grid[reel][row] = reelStrip[randomIndex];
            }
        }
    }
    return grid;
}