#include "core/payout.h"
#include "config/config_manager.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

void runPayoutTests() {
    std::cout << "Running PayoutCalculator tests...\n";

    bool configsLoaded = ConfigManager::getInstance().loadAllConfigs("assets/config");
    assert(configsLoaded);

    PayoutCalculator payoutCalculator;

    {
        std::vector<std::vector<std::string>> grid = {
            {"CHERRY", "LEMON", "BAR"},
            {"CHERRY", "BELL", "SEVEN"},
            {"CHERRY", "BAR", "LEMON"}
        };

        double betAmount = 10.0;
        std::vector<WinLine> wins = payoutCalculator.calculateWin(grid, betAmount);

        assert(!wins.empty());

        bool foundCherryWin = false;
        for (const auto& win : wins) {
            if (win.symbol == "CHERRY" && win.match_count >= 3) {
                foundCherryWin = true;
                assert(win.win_amount > 0.0);
            }
        }

        assert(foundCherryWin);
    }

    {
        std::vector<std::vector<std::string>> grid = {
            {"CHERRY", "LEMON", "BAR"},
            {"SEVEN", "BELL", "CHERRY"},
            {"BAR", "SEVEN", "LEMON"}
        };

        double betAmount = 10.0;
        std::vector<WinLine> wins = payoutCalculator.calculateWin(grid, betAmount);

        // Depending on the paylines, this test may or may not produce a win.
        // This test primarily ensures no crash on mixed symbols.
        for (const auto& win : wins) {
            assert(win.win_amount >= 0.0);
            assert(win.match_count > 0);
        }
    }

    std::cout << "PayoutCalculator tests passed.\n";
}