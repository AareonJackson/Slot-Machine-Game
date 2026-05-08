#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class SoundManager {
public:
    bool loadSound(const std::string& name, const std::string& filePath);
    void playSound(const std::string& name);
    void setVolume(float volume);
    
private:
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::map<std::string, sf::Sound> m_sounds;
    float m_volume = 100.0f;

};
