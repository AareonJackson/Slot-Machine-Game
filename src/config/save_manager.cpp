#include "config/save_manager.h"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

bool SaveManager::load(const std::string& filePath, SaveData& saveData) const {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cout << "No save file found. Starting with default player data." << std::endl;
        return false;
    }

    try {
        json data;
        file >> data;

        saveData.balance = data.value("balance", saveData.balance);
        saveData.totalSpins = data.value("total_spins", saveData.totalSpins);
        saveData.totalWagered = data.value("total_wagered", saveData.totalWagered);
        saveData.totalWon = data.value("total_won", saveData.totalWon);
        saveData.biggestWin = data.value("biggest_win", saveData.biggestWin);

        std::cout << "Loaded player save data." << std::endl;
        return true;
    } catch (const json::exception& error) {
        std::cerr << "Failed to parse save file: " << error.what() << std::endl;
        std::cerr << "Starting with default player data." << std::endl;
        return false;
    }
}

bool SaveManager::save(const std::string& filePath, const SaveData& saveData) const {
    try {
        std::filesystem::path path(filePath);
        std::filesystem::create_directories(path.parent_path());

        json data;
        data["balance"] = saveData.balance;
        data["total_spins"] = saveData.totalSpins;
        data["total_wagered"] = saveData.totalWagered;
        data["total_won"] = saveData.totalWon;
        data["biggest_win"] = saveData.biggestWin;

        std::ofstream file(filePath);

        if (!file.is_open()) {
            std::cerr << "Failed to open save file for writing: " << filePath << std::endl;
            return false;
        }

        file << data.dump(4);

        return true;
    } catch (const std::exception& error) {
        std::cerr << "Failed to save player data: " << error.what() << std::endl;
        return false;
    }
}