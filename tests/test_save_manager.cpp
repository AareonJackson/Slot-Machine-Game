#include "config/save_manager.h"
#include "config/save_data.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

void runSaveManagerTests() {
    std::cout << "Running SaveManager tests...\n";

    const std::string testDirectory = "test_output";
    const std::string savePath = testDirectory + "/player_stats_tests.json";

    std::filesystem::remove_all(testDirectory);

    SaveManager saveManager;

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

    {
        std::ofstream corruptFile(savePath);
        corruptFile << "{ invalid json ";
        corruptFile.close();

        SaveData loadedData;
        bool loaded = saveManager.load(savePath, loadedData);

        assert(!loaded);
        assert(loadedData.balance == 1000.0);
    }

    std::filesystem::remove_all(testDirectory);

    std::cout << "SaveManager tests passes.\n";
}