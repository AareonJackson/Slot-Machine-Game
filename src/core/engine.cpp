#include "core/engine.h"
#include "config/config_manager.h"
#include "core/debug_log.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Engine::Engine() {
    auto& config = ConfigManager::getInstance();

    if (!config.loadAllConfigs("assets/config")) {
        std::cerr << "Failed to load configuration files. Using fallback window settings." << std::endl;
        // initialize the window from the game_config.json
        m_window = std::make_unique<GameWindow>(1200, 720, "The Slot Game");
    } else {
        const auto& gameConfig = config.getGameConfig();
        m_window = std::make_unique<GameWindow>(
            gameConfig.windowWidth,
            gameConfig.windowHeight,
            gameConfig.gameTitle
        );
    }

    m_soundManager.loadSound("spin", "assets/audio/spin.wav");
    m_soundManager.loadSound("reel_stop", "assets/audio/reel_stop.wav");
    m_soundManager.loadSound("win", "assets/audio/win.wav");
    m_soundManager.setVolume(70.0f);

    applyGameConfig();
    loadPlayerSave();

    refreshStatusText();
    refreshStatsText();
    refreshFreeSpinsText();
    refreshModeText();
    m_window->updatePaytableText(buildPaytableText());

    m_window->setSpinCallback([this]() {
        spin();
    });

    m_window->setBetUpCallback([this]() {
        increaseBet();
    });

    m_window->setBetDownCallback([this]() {
        decreaseBet();
    });
    
    m_window->setAutoPlayCallback([this]() {
        toggleAutoPlay();
    });

    m_window->setResetCallback([this]() {
        resetProgress();
    });
}

Engine::~Engine() {
    savePlayerSave();
}

void Engine::applyGameConfig() {
    const auto& gameConfig = ConfigManager::getInstance().getGameConfig();

    m_defaultBalance = gameConfig.startingBalance;
    m_defaultBet = gameConfig.defaultBet;

    m_balance = gameConfig.startingBalance;
    m_currentBet = gameConfig.defaultBet;
    m_minBet = gameConfig.minBet;
    m_maxBet = gameConfig.maxBet;
    m_betStep = gameConfig.betStep;

    m_spinDurationSeconds = gameConfig.spinDurationSeconds;
    m_firstReelStopTime = gameConfig.firstReelStopTime;
    m_delayBetweenReels = gameConfig.delayBetweenReels;

    m_autoPlayDelaySeconds = gameConfig.autoplayDelaySeconds;

    m_freeSpinTriggerSymbol = gameConfig.freeSpinTriggerSymbol;
    m_freeSpinTriggerCount = gameConfig.freeSpinTriggerCount;
    m_freeSpinsAwardAmount = gameConfig.freeSpinsAwarded;
    m_freeSpinDelaySeconds = gameConfig.freeSpinDelaySeconds;

    if (m_currentBet < m_minBet) {
        m_currentBet = m_minBet;
    }

    if (m_currentBet > m_maxBet) {
        m_currentBet = m_maxBet;
    }
}

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

        if (m_spinClock.getElapsedTime().asSeconds() >= m_spinDurationSeconds) {
            finishSpin();
        }
    }

    startNextFreeSpinIfNeeded();
    updateAutoPlay();
}

void Engine::render() {
    // Have the window draw itself
    m_window->render();
}

bool Engine::canStartSpin() const {
    return m_state == GameState::Idle && m_balance >= m_currentBet;
}

bool Engine::isLowBalance() const {
    return m_balance < m_currentBet && m_freeSpinsRemaining <= 0;
}

void Engine::showLowBalanceMessage() {
    m_autoPlayEnabled = false;
    refreshModeText();

    if (m_window) {
        m_window->showWinMessage("LOW BALANCE\nPress RESET");
    }

    std::cout << "Low balance. Balance: " << m_balance << ", Bet: " << m_currentBet << std::endl;
}

void Engine::resetProgress() {
    if (m_state == GameState::Spinning) {
        return;
    }

    m_autoPlayEnabled = false;
    m_freeSpinsRemaining = 0;
    m_currentSpinIsFree = false;

    m_balance = m_defaultBalance;
    m_currentBet = m_defaultBet;
    m_lastWin = 0.0;

    m_totalSpins = 0;
    m_totalWagered = 0.0;
    m_totalWon = 0.0;
    m_biggestWin = 0.0;

    if (m_window) {
        m_window->clearHighlightedCells();
        m_window->clearWinMessage();
    }

    refreshStatusText();
    refreshStatsText();
    refreshFreeSpinsText();
    refreshModeText();

    savePlayerSave();

    std::cout << "Progress reset." << std::endl;
}

void Engine::spin() {
    if (m_state == GameState::Spinning) {
        return;
    }

    const bool freeSpin = isFreeSpinActive();
    m_currentSpinIsFree = freeSpin;

    if (!freeSpin && m_balance < m_currentBet) {
        showLowBalanceMessage();
        return;
    }

    if (freeSpin) {
        m_freeSpinsRemaining--;
        refreshFreeSpinsText();
    } else {
        m_balance -= m_currentBet;
    }

    m_lastWin = 0.0;
    refreshStatusText();
    m_window->clearHighlightedCells();
    m_window->clearWinMessage();

    m_pendingSpinGrid = generateSpinGrid();
    m_loggedStoppedReels.assign(m_pendingSpinGrid.size(), false);
    m_state = GameState::Spinning;
    m_spinClock.restart();

    refreshModeText();

    m_soundManager.playSound("spin");

    std::cout << "Spin started. Bet: $" << m_currentBet
              << ", Balance: $" << m_balance
              << ", Free spin: " << (freeSpin ? "YES" : "NO") << std::endl;
}

void Engine::finishSpin() {

    m_window->updateReels(m_pendingSpinGrid);

    std::vector<WinLine> wins = m_payoutCalculator.calculateWin(m_pendingSpinGrid, m_currentBet);

    double totalWin = 0.0;
    for (const auto& win : wins) {
        totalWin += win.win_amount;

        DEBUG_LOG("Winning payline " << win.payline_index << ": $" << win.match_count << "x " << win.symbol
        << " pays $" << win.win_amount);
    }

    m_lastWin = totalWin;
    m_balance += totalWin;
    m_state = GameState::Idle;

    m_totalSpins++;

    if (!m_currentSpinIsFree) {
        m_totalWagered += m_currentBet;
    }

    m_totalWon += totalWin;

    if (totalWin > m_biggestWin) {
        m_biggestWin = totalWin;
    }

    if (!m_freeSpinsRemaining && checkFreeSpinTrigger(wins)) {
        m_freeSpinsRemaining += m_freeSpinsAwardAmount;
        std:: cout << "Bonus triggered! Awarded "
                << m_freeSpinsAwardAmount
                << " free spins" << std::endl;
        refreshFreeSpinsText();
    }

    if (totalWin > 0.0) {
        m_soundManager.playSound("win");
        m_window->showWinMessage(buildWinMessage(totalWin));
    }

    std::vector<std::vector<bool>> highlightedCells = buildWinningCellHighlights(wins);
    m_window->setHighlightedCells(highlightedCells);

    refreshStatusText();
    refreshStatsText();
    refreshFreeSpinsText();
    refreshModeText();

    savePlayerSave();

    if (m_freeSpinsRemaining > 0) {
        m_freeSpinDelayClock.restart();
    }

    if (m_autoPlayEnabled) {
        m_autoPlayClock.restart();
    }

    DEBUG_LOG("Spin complete. Bet: $" << m_currentBet
    << ", Win: $" << totalWin << ", Balance: $" << m_balance);
}

void Engine::animateSpin() {
    float elapsedSeconds = m_spinClock.getElapsedTime().asSeconds();
    std::vector<std::vector<std::string>> animatedGrid = generateAnimatedDisplayGrid(elapsedSeconds);
    m_window->updateReels(animatedGrid);
}

void Engine::toggleAutoPlay() {
    m_autoPlayEnabled = !m_autoPlayEnabled;

    refreshModeText();

    if (m_autoPlayEnabled) {
        if (!canStartSpin()) {
            m_autoPlayEnabled = false;
            return;
        }

        m_autoPlayClock.restart();
        spin();
    }
}

void Engine::updateAutoPlay() {
    if (!m_autoPlayEnabled) {
        return;
    }

    if (m_freeSpinsRemaining > 0) {
        return;
    }

    if (m_state != GameState::Idle) {
        return;
    }

    if (m_balance < m_currentBet) {
        showLowBalanceMessage();
        return;
    }

    if (m_autoPlayClock.getElapsedTime().asSeconds() >= m_autoPlayDelaySeconds) {
        spin();
    }
}

std::vector<std::vector<bool>> Engine::buildWinningCellHighlights(const std::vector<WinLine>& wins) {
    const auto& reelsConfig = ConfigManager::getInstance().getReelsConfig();
    const auto& paylinesConfig = ConfigManager::getInstance().getPaylinesConfig();

    std::vector<std::vector<bool>> highlightedCells(
        reelsConfig.num_reels,
        std::vector<bool>(reelsConfig.num_visible_rows, false)
    );

    for (const auto& win : wins) {
        if (win.payline_index < 0 ||
            win.payline_index >= static_cast<int>(paylinesConfig.paylines.size())) {
            continue;
        }

        const auto& payline = paylinesConfig.paylines[win.payline_index];
        for (int reel = 0; reel < win.match_count && reel < static_cast<int>(payline.size()); ++reel) {
            int row = payline[reel];

            if (reel >= 0 &&
                reel < reelsConfig.num_reels &&
                row >= 0 &&
                row < reelsConfig.num_visible_rows) {
                highlightedCells[reel][row] = true;
            }
        }
    }

    return highlightedCells;
}

void Engine::increaseBet() {
    if (m_state == GameState::Spinning || isFreeSpinActive() || m_autoPlayEnabled) {
        return;
    }

    if (m_currentBet + m_betStep <= m_maxBet && m_currentBet + m_betStep <= m_balance) {
        m_currentBet += m_betStep;
        m_lastWin = 0.0;
        refreshStatusText();
        m_window->clearWinMessage();

       DEBUG_LOG("Bet increased to $" << m_currentBet);
    } else if (isLowBalance()) {
        showLowBalanceMessage();
    }
}

void Engine::decreaseBet() {
    if (m_state == GameState::Spinning || isFreeSpinActive() || m_autoPlayEnabled) {
        return;
    }

    if (m_currentBet - m_betStep >= m_minBet) {
        m_currentBet -= m_betStep;
        m_lastWin = 0.0;
        refreshStatusText();

        if (!isLowBalance()) {
            m_window->clearWinMessage();
        }

        DEBUG_LOG("Bet decreased to $" << m_currentBet);
    }
}

void Engine::refreshStatusText() {
    if (m_window) {
        m_window->updateStatusText(m_balance, m_currentBet, m_lastWin);
    }
}

void Engine::refreshStatsText() {
    if (m_window) {
        m_window->updateStatsText(m_totalSpins, m_totalWagered, m_totalWon, m_biggestWin);
    }
}

void Engine::loadPlayerSave() {
    SaveData saveData;

    if (m_saveManager.load(m_saveFilePath, saveData)) {
        m_balance = saveData.balance;
        m_totalSpins = saveData.totalSpins;
        m_totalWagered = saveData.totalWagered;
        m_totalWon = saveData.totalWon;
        m_biggestWin = saveData.biggestWin;
    }
}

void Engine::savePlayerSave() const {
    SaveData saveData;
    saveData.balance = m_balance;
    saveData.totalSpins = m_totalSpins;
    saveData.totalWagered = m_totalWagered;
    saveData.totalWon = m_totalWon;
    saveData.biggestWin = m_biggestWin;

    m_saveManager.save(m_saveFilePath, saveData);
}

void Engine::refreshFreeSpinsText() {
    if (m_window) {
        m_window->updateFreeSpinsText(m_freeSpinsRemaining);
    }
}

void Engine::refreshModeText() {
    if (m_window) {
        m_window->updateModeText(getCurrentModeText());
    }
}

std::string Engine::getCurrentModeText() const {
    if (m_state == GameState::Spinning) {
        if (m_currentSpinIsFree) {
            return "Free Spin";
        }

        if (m_autoPlayEnabled) {
            return "Autoplay Spinning";
        }

        return "Spinning";
    }

    if (m_freeSpinsRemaining > 0) {
        return "Free Spins";
    }

    if (m_autoPlayEnabled) {
        return "Autoplay";
    }

    return "Manual";
}

std::string Engine::buildWinMessage(double totalWin) const {
    std::ostringstream stream;

    double winMultiplier = 0.0;
    if (m_currentBet > 0.0) {
        winMultiplier = totalWin / m_currentBet;
    }
    if (winMultiplier >= 100.0) {
        stream << "JACKPOT!";
    } else if (winMultiplier >= 20.0) {
        stream << "HUGE WIN!";
    } else if (winMultiplier >= 5.0) {
        stream << "BIG WIN!";
    } else {
        stream << "WIN!";
    }

    stream << " $" << std::fixed << std::setprecision(2) << totalWin;

    return stream.str();
}

bool Engine::isFreeSpinActive() const {
    return m_freeSpinsRemaining > 0;
}

bool Engine::checkFreeSpinTrigger(const std::vector<WinLine>& wins) const {
    for (const auto& win : wins) {
        if (win.symbol == m_freeSpinTriggerSymbol && win.match_count >= m_freeSpinTriggerCount) {
            return true;
        }
    }

    return false;
}

void Engine::startNextFreeSpinIfNeeded() {
    if (m_state != GameState::Idle) {
        return;
    }

    if (m_freeSpinsRemaining <= 0) {
        return;
    }

    if (m_freeSpinDelayClock.getElapsedTime().asSeconds() >= m_freeSpinDelaySeconds) {
        spin();
    }
}

std::string Engine::buildPaytableText() const {
    const auto& paytableConfig = ConfigManager::getInstance().getPaytableConfig();

    std::ostringstream stream;
    stream << "PAYTABLE / HELP\n\n";
    stream << "Match symbols from left to right on active paylines.\n";
    stream <<"Payout = Bet x Multiplier\n\n";
    stream << "PAYOUTS:\n\n";

    for (const auto& rule : paytableConfig.payouts) {
        stream << rule.match_count << "x "
               << rule.symbol
               << " pays "
               << rule.multiplier
               << "x bet\n";
    }

    stream << "\nCONTROLS:\n";
    stream << "SPIN : Start a new spin\n";
    stream << "BET + : Increase bet\n";
    stream << "BET - : Decrease bet\n";
    stream << "HELP : Show/hide this screen\n";

    stream << "\nBONUS FEATURE:\n";
    stream << "Land " << m_freeSpinTriggerCount << " or more "
           << m_freeSpinTriggerSymbol << " symbols on a winning payline to win "
           << m_freeSpinsAwardAmount << " free spins.\n";
    stream << "Free spins do not cost balance but still pay using your current bet.\n";
    stream << "Free spins do not retrigger additional free spins in this version.\n";

    return stream.str();
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


    for (int reel = 0; reel < reelsConfig.num_reels; ++reel) {
        const auto& reelStrip = reelsConfig.reel_strips[reel];
        int reelLength = static_cast<int>(reelStrip.size());

        float reelStopTime = m_firstReelStopTime + static_cast<float>(reel) * m_delayBetweenReels;
        bool reelHasStopped = elapsedSeconds >= reelStopTime;

        if (reelHasStopped &&
            reel < static_cast<int>(m_loggedStoppedReels.size()) &&
            !m_loggedStoppedReels[reel]) {
            DEBUG_LOG("Reel " << reel + 1 << " stopped at "
                      << elapsedSeconds << "s");

            m_soundManager.playSound("reel_stop");

            m_loggedStoppedReels[reel] = true;
        }

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
