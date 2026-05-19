#pragma once
#include "ui/game_window.h"
#include "core/rng.h"
#include "core/payout.h"
#include "audio/sound_manager.h"
#include "config/save_manager.h"
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
    void toggleAutoPlay();
    void updateAutoPlay();
    bool canStartSpin() const;
    void resetProgress();
    void increaseBet();
    void decreaseBet();
    void refreshStatusText();
    void refreshStatsText();
    void refreshFreeSpinsText();
    void refreshModeText();
    std::string getCurrentModeText() const;
    void loadPlayerSave();
    void savePlayerSave() const;
    bool isFreeSpinActive() const;
    bool checkFreeSpinTrigger(const std::vector<WinLine>& wins) const;
    void startNextFreeSpinIfNeeded();
    std::string buildPaytableText() const;
    std::vector<std::vector<std::string>> generateSpinGrid();
    std::vector<std::vector<std::string>> generateRandomDisplayGrid();
    std::vector<std::vector<std::string>> generateAnimatedDisplayGrid(float elapsedSeconds);
    std::vector<std::vector<bool>> buildWinningCellHighlights(const std::vector<WinLine>& wins);

    std::unique_ptr<GameWindow> m_window;

    RNG m_rng;
    PayoutCalculator m_payoutCalculator;
    SoundManager m_soundManager;
    SaveManager m_saveManager;

    const std::string m_saveFilePath = "save/player_stats.json";

    GameState m_state = GameState::Idle;
    sf::Clock m_spinClock;
    sf::Clock m_autoPlayClock;
    std::vector<std::vector<std::string>> m_pendingSpinGrid;
    std::vector<bool> m_loggedStoppedReels;

    double m_balance = 1000.0;
    double m_currentBet = 10.0;
    double m_lastWin = 0.0;
    double m_minBet = 1.0;
    double m_maxBet = 100.0;
    double m_betStep = 5.0;

    const double m_defaultBalance = 1000.0;
    const double m_defaultBet = 10.0;

    int m_totalSpins = 0;
    double m_totalWagered = 0.0;
    double m_totalWon = 0.0;
    double m_biggestWin = 0.0;

    int m_freeSpinsRemaining = 0;
    int m_freeSpinsAwardAmount = 5;
    std::string m_freeSpinTriggerSymbol = "BELL";
    int m_freeSpinTriggerCount = 3;
    bool m_currentSpinIsFree = false;
    sf::Clock m_freeSpinDelayClock;
    float m_freeSpinDelaySeconds = 0.9f;
    bool m_autoPlayEnabled = false;
    float m_autoPlayDelaySeconds = 0.75f;
};
