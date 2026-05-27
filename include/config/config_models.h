#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

// 1. Define the standard C++ struct
struct GameConfig {
    std::string gameTitle = "The Slot Game";
    int windowWidth = 1200;
    int windowHeight = 720;

    double startingBalance = 1000.0;
    double defaultBet = 10.0;
    double minBet = 1.0;
    double maxBet = 100.0;
    double betStep = 5.0;

    float spinDurationSeconds = 3.2f;
    float firstReelStopTime = 0.8f;
    float delayBetweenReels = 1.0f;

    float autoplayDelaySeconds = 0.75f;

    std::string freeSpinTriggerSymbol = "BELL";
    int freeSpinTriggerCount = 3;
    int freeSpinsAwarded = 5;
    float freeSpinDelaySeconds = 0.9f;
};

struct ReelsConfig {
    int num_reels;
    int num_visible_rows;
    std::vector<std::vector<std::string>> reel_strips;
};

struct PaylinesConfig {
    // each payline is a vector of integers, representing row indices per reel
    std::vector<std::vector<int>> paylines;
};

struct PayoutRule {
    std::string symbol;
    int match_count;
    int multiplier; // the multiplier applied to the player's bet
};

struct PaytableConfig {
    std::vector<PayoutRule> payouts;
};



NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(
    GameConfig,
    gameTitle,
    windowWidth,
    windowHeight,
    startingBalance,
    defaultBet,
    minBet,
    maxBet,
    betStep,
    spinDurationSeconds,
    firstReelStopTime,
    delayBetweenReels,
    autoplayDelaySeconds,
    freeSpinTriggerSymbol,
    freeSpinTriggerCount,
    freeSpinsAwarded,
    freeSpinDelaySeconds
)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ReelsConfig, num_reels, num_visible_rows, reel_strips)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PaylinesConfig, paylines)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PayoutRule, symbol, match_count, multiplier)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PaytableConfig, payouts)
// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FeaturesConfig, )


