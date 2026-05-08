#include "audio/sound_manager.h"
#include <iostream>

bool SoundManager::loadSound(const std::string& name, const std::string& filePath) {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(filePath)) {
        std::cerr << "Failed to load sound: " << filePath << std::endl;
        return false;
    }

    m_soundBuffers[name] = buffer;

    sf::Sound sound;
    sound.setBuffer(m_soundBuffers[name]);
    sound.setVolume(m_volume);

    m_sounds[name] = sound;

    return true;
}

void SoundManager::playSound(const std::string& name) {
    auto soundIt = m_sounds.find(name);

    if (soundIt == m_sounds.end()) {
        std::cerr << "Sound not found: " << name << std::endl;
        return;
    }

    soundIt->second.play();
}

void SoundManager::setVolume(float volume) {
    m_volume = volume;

    for (auto& [name, sound] : m_sounds) {
        sound.setVolume(volume);
    }
}