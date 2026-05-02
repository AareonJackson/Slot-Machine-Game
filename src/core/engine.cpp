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

    m_window->updateStatusText(m_balance, m_currentBet, m_lastWin);
    m_window->setSpinCallback([this]() {
        spin();
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
    // TODO: ADD GAME LOGIC (SPINNING REELS, CHECKING PAYLINES, ETC.)

}

void Engine::render() {
    // Have the window draw itself
    m_window->render();
}

void Engine::spin() {
    if (m_balance < m_currentBet) {
        std::cout << "Not enough money to spin! Balance: $" << m_balance << std::endl;
        return;
    }

    m_balance -= m_currentBet;

    std::vector<std::vector<std::string>> spinGrid = generateSpinGrid();
    m_window->updateReels(spinGrid);

    std::vector<WinLine> wins = m_payoutCalculator.calculateWin(spinGrid, m_currentBet);

    double totalWin = 0.0;
    for (const auto& win : wins) {
        totalWin += win.win_amount;

        std::cout << "Winning payline " << win.payline_index << ": $" << win.match_count << "x " << win.symbol
        << " pays $" << win.win_amount << std::endl;
    }

    m_lastWin = totalWin;
    m_balance += totalWin;

    m_window->updateStatusText(m_balance, m_currentBet, m_lastWin);

    std::cout << "Spin complete. Bet: $" << m_currentBet
    << ", Win: $" << totalWin << ", Balance: $" << m_balance << std::endl;
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