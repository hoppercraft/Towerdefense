// SoundManager.h
#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class SoundManager {
public:
    static SoundManager& getInstance();

    bool loadSound(const std::string& name, const std::string& filename);
    void playSound(const std::string& name, float volume = 100.0f);
    void setGlobalVolume(float volume);

private:
    SoundManager() = default;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    float globalVolume = 100.0f;
};

// SoundManager.cpp
#include "SoundManager.h"
#include <iostream>

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

bool SoundManager::loadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        std::cerr << "Failed to load sound: " << filename << std::endl;
        return false;
    }

    soundBuffers[name] = std::move(buffer);
    sounds[name].setBuffer(soundBuffers[name]);
    return true;
}

void SoundManager::playSound(const std::string& name, float volume) {
    auto it = sounds.find(name);
    if (it != sounds.end()) {
        it->second.setVolume(volume * globalVolume / 100.0f);
        it->second.play();
    }
}

void SoundManager::setGlobalVolume(float volume) {
    globalVolume = std::max(0.0f, std::min(100.0f, volume));
}
