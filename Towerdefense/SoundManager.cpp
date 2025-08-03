#include "SoundManager.h"
#include <iostream>
#include <algorithm>

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

bool SoundManager::loadSounds() {
    if (soundsLoaded) return true;


    if (bulletSoundBuffer.loadFromFile("Sounds\\bull.wav")) {
        std::cout << "Bullet shoot sound loaded successfully from: Sounds\\bull.wav" << "\n";
    }
    if (cannonSoundBuffer.loadFromFile("Sounds\\cannon.wav")) {
        std::cout << "Bullet shoot sound loaded successfully from: Sounds\\cannon.wav" << "\n";
    }
    if (teslaSoundBuffer.loadFromFile("Sounds\\Tesla.wav")) {
        soundsLoaded = true;
        std::cout << "Bullet shoot sound loaded successfully from: Sounds\\Tesla.wav" << "\n";
        return true;
    }

    std::cerr << "Failed to load bullet shoot sound from all attempted paths\n";
    std::cerr << "Please place bull.wav in location:\n";
    std::cerr << "..\\Towerdefense\\Sounds\\bull.wav\n";
    return false;
}

void SoundManager::playBulletSound() {
    if (!soundsLoaded) return;

    auto sound = std::make_unique<sf::Sound>(bulletSoundBuffer);
    sound->setVolume(15.f);
    sound->play();

    // Store it so it doesn't get destroyed immediately
    activeSounds.push_back(std::move(sound));
}

void SoundManager::playCannonSound() {
    if (!soundsLoaded) return;

    auto sound = std::make_unique<sf::Sound>(cannonSoundBuffer);
    sound->setVolume(50.f);
    sound->play();

    // Store it so it doesn't get destroyed immediately
    activeSounds.push_back(std::move(sound));
}

void SoundManager::playTeslaSound() {
    if (!soundsLoaded) return;

    auto sound = std::make_unique<sf::Sound>(teslaSoundBuffer);
    sound->setVolume(15.f);
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