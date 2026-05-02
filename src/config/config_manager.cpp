#include "config/config_manager.h"

#include <fstream>
#include <iostream>
#include <type_traits>

using json = nlohmann::json;

bool ConfigManager::loadAllConfigs(const std::string& configDirectory) {
    auto loadJsonFile = [](const std::string& path, auto& configObject) -> bool {
        std::ifstream file(path);

        if (!file.is_open()) {
            std::cerr << "Failed to open config file: " << path << std::endl;
            return false;
        }

        try {
            json data;
            file >> data;

            using ConfigType = std::remove_reference_t<decltype(configObject)>;
            configObject = data.get<ConfigType>();

            return true;
        } catch (const json::exception& error) {
            std::cerr << "JSON error in " << path << ": " << error.what() << std::endl;
            return false;
        }
    };

    bool success = true;

    success &= loadJsonFile(configDirectory + "/game_config.json", gameConfig);
    success &= loadJsonFile(configDirectory + "/reels_config.json", reelsConfig);
    success &= loadJsonFile(configDirectory + "/paylines_config.json", paylinesConfig);
    success &= loadJsonFile(configDirectory + "/paytable_config.json", paytableConfig);

    if (success) {
        std::cout << "Successfully loaded all configuration files." << std::endl;
    } else {
        std::cerr << "One or more required configuration files failed to load." << std::endl;
    }

    return success;
}