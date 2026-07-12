//
// @Description: The config model uses default values for its fields.
// Here is where we will verify that missing fields fall back to their default values.

#include "config/config_models.h"
#include <cassert>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void runConfigModelTests() {
    std::cout << "Running ConfigModel tests...\n";

    {
        json data = {
            {"gameTitle", "Test Game"},
            {"windowWidth", 1024},
            {"windowHeight", 768}
        };

        GameConfig config = data.get<GameConfig>();

        assert(config.gameTitle == "Test Game");
        assert(config.windowWidth == 1024);
        assert(config.windowHeight == 768);

        // Defaults should fill in missing values
        assert(config.startingBalance == 1000.0);
        assert(config.defaultBet == 10.0);
        assert(config.minBet == 1.0);
        assert(config.maxBet == 100.0);
        assert(config.betStep == 5.0);
        assert(config.spinDurationSeconds == 3.2f);
        assert(config.freeSpinTriggerSymbol == "BELL");
        assert(config.freeSpinTriggerCount == 3);
        assert(config.freeSpinsAwarded == 5);
    }

    {
        json data = {
            {"gameTitle", "Custom Game"},
            {"windowWidth", 1200},
            {"windowHeight", 720},
           {"startingBalance", 500.0},
            {"defaultBet", 25.0},
            {"freeSpinTriggerSymbol", "SEVEN"},
            {"freeSpinsAwarded", 10}
        };

        GameConfig config = data.get<GameConfig>();

        assert(config.gameTitle == "Custom Game");
        assert(config.startingBalance == 500.0);
        assert(config.defaultBet == 25.0);
        assert(config.freeSpinTriggerSymbol == "SEVEN");
        assert(config.freeSpinsAwarded == 10);
    }

    std::cout << "ConfigModel tests passed!\n";
}
