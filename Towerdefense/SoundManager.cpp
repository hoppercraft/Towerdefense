#include "SoundManager.h"
#include <iostream>
#include <algorithm>

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

bool SoundManager::loadSounds() {
    if (soundsLoaded) return true;

    // Try multiple paths for the bullet sound
    std::vector<std::string> paths = {
        "..\\Sounds\\bull.wav",
        "D:\\new1\\Sounds\\bull.wav",
        "bull.wav",
        "Sounds\\bull.wav"
    };

    for (const auto& path : paths) {
        if (bulletSoundBuffer.loadFromFile(path)) {
            soundsLoaded = true;
            std::cout << "Bullet shoot sound loaded successfully from: " << path << "\n";
            return true;
        }
    }

    std::cerr << "Failed to load bullet shoot sound from all attempted paths\n";
    std::cerr << "Please place bull.wav in one of these locations:\n";
    std::cerr << "  - D:\\new1\\Towerdefense\\bull.wav\n";
    std::cerr << "  - D:\\new1\\Towerdefense\\Sounds\\bull.wav\n";
    std::cerr << "  - D:\\new1\\Sounds\\bull.wav\n";
    return false;
}

void SoundManager::playBulletSound() {
    if (!soundsLoaded) return;

    // Create a new sound instance
    auto sound = std::make_unique<sf::Sound>(bulletSoundBuffer);
    sound->setVolume(30.f); // Adjust volume as needed (0-100)
    sound->play();

    // Store it so it doesn't get destroyed immediately
    activeSounds.push_back(std::move(sound));
}

void SoundManager::update() {
    removeFinishedSounds();
}

void SoundManager::removeFinishedSounds() {
    activeSounds.erase(
        std::remove_if(activeSounds.begin(), activeSounds.end(),
            [](const std::unique_ptr<sf::Sound>& sound) {
                return sound->getStatus() == sf::SoundSource::Status::Stopped;
            }),
        activeSounds.end()
    );
}

void SoundManager::cleanup() {
    activeSounds.clear();
    soundsLoaded = false;
}