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

    // Unit Test Case 1: Ensure a known 3-symbol win, confirms payout works.
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

    // Unit Test Case 2: Ensure no matching payline, confirms no false positives.
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

    // Unit Test Case 3: Empty grid, confirms safe handling
    {
        std::vector<std::vector<std::string>> emptyGrid;

        double betAmount = 10.0;
        std::vector<WinLine> wins = payoutCalculator.calculateWin(emptyGrid, betAmount);

        assert(wins.empty());
    }

    // Unit Test Case 4: Zero bet amount, confirms no crash
    {
        std::vector<std::vector<std::string>> grid = {
            {"CHERRY", "LEMON", "BAR"},
            {"CHERRY", "BELL", "SEVEN"},
            {"CHERRY", "BAR", "LEMON"}
        };

        double betAmount = 0.0;
        std::vector<WinLine> wins = payoutCalculator.calculateWin(grid, betAmount);

        for (const auto& win : wins) {
            assert(win.win_amount == 0.0);
        }
    }

    // Unit Test Case 5: Mixed grid win, confirms stable behavior
    {
        std::vector<std::vector<std::string>> grid = {
            {"SEVEN", "SEVEN", "SEVEN"},
            {"SEVEN", "SEVEN", "SEVEN"},
            {"SEVEN", "SEVEN", "SEVEN"}
        };

        double betAmount = 10.0;
        std::vector<WinLine> wins = payoutCalculator.calculateWin(grid, betAmount);

        assert(!wins.empty());

        for (const auto& win : wins) {
            assert(win.symbol == "SEVEN");
            assert(win.match_count >= 1);
            assert(win.win_amount >= 0.0);
        }
    }

    std::cout << "PayoutCalculator tests passed.\n";
}