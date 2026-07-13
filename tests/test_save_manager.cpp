#include "config/save_manager.h"
#include "config/save_data.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

/*
 * @Purpose: Tests the SaveManager class for loading and saving player statistics.
 * These tests verify that missing save files fall back to defaults, valid player
 * data can be saved and loaded, corrupted save files fail safely, and partial
 * save files preserve default values for missing fields.
 */

void runSaveManagerTests() {
    std::cout << "Running SaveManager tests...\n";

    const std::string testDirectory = "test_output";
    const std::string savePath = testDirectory + "/player_stats_tests.json";

    std::filesystem::remove_all(testDirectory);

    SaveManager saveManager;

    // Unit Test Case 1: Loading a missing save file fails safely and keeps default player statistics
    {
        SaveData loadedData;
        bool loaded = saveManager.load(savePath, loadedData);

        assert(!loaded);
        assert(loadedData.balance == 1000.0);
        assert(loadedData.totalSpins == 0);
        assert(loadedData.totalWagered == 0.0);
        assert(loadedData.totalWon == 0.0);
        assert(loadedData.biggestWin == 0.0);
    }

    // Unit Test Case 2: Saving valid player statistics succeeds and creates the save file
    {
        SaveData saveData;
        saveData.balance = 875.0;
        saveData.totalSpins = 12;
        saveData.totalWagered = 120.0;
        saveData.totalWon = 80.0;
        saveData.biggestWin = 40.0;

        bool saved = saveManager.save(savePath, saveData);
        assert(saved);
        assert(std::filesystem::exists(savePath));
    }

    // Unit Test Case 3: Loading a valid save file restores the expected player statistics
    {
        SaveData loadedData;
        bool loaded = saveManager.load(savePath, loadedData);

        assert(loaded);
        assert(loadedData.balance == 875.0);
        assert(loadedData.totalSpins == 12);
        assert(loadedData.totalWagered == 120.0);
        assert(loadedData.totalWon == 80.0);
        assert(loadedData.biggestWin == 40.0);
    }

    // Unit Test Case 4: Loading corrupted JSON fails safely and keeps default values
    {
        std::ofstream corruptFile(savePath);
        corruptFile << "{ invalid json ";
        corruptFile.close();

        SaveData loadedData;
        bool loaded = saveManager.load(savePath, loadedData);

        assert(!loaded);
        assert(loadedData.balance == 1000.0);
        assert(loadedData.totalSpins == 0);
        assert(loadedData.totalWagered == 0.0);
        assert(loadedData.totalWon == 0.0);
        assert(loadedData.biggestWin == 0.0);
    }

    // Unit Test Case 5: Loading partial JSON applies provided values and keeps defaults for missing fields
    {
        std::ofstream partialFile(savePath);
        partialFile << R"({
            "balance": 555.0,
            "total_spins": 8
        })";
        partialFile.close();

        SaveData loadedData;
        bool loaded = saveManager.load(savePath, loadedData);

        assert(loaded);
        assert(loadedData.balance == 555.0);
        assert(loadedData.totalSpins == 8);

        // Missing values should remain defaults.
        assert(loadedData.totalWagered == 0.0);
        assert(loadedData.totalWon == 0.0);
        assert(loadedData.biggestWin == 0.0);
    }

    // Clean up temporary test files so future test runs start from a known state.
    std::filesystem::remove_all(testDirectory);

    std::cout << "SaveManager tests passed.\n";
}