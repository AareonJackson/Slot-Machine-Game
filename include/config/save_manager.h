#pragma once

#include "config/save_data.h"
#include <string>

class SaveManager {
public:
    bool load(const std::string& filePath, SaveData& saveData) const;
    bool save(const std::string& filePath, const SaveData& saveData) const;
};
