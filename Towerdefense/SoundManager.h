#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>

class SoundManager {
public:
    static SoundManager& getInstance();

    bool loadSounds();
    void playBulletSound();
    void update(); // Call this in your main loop to clean up finished sounds
    void cleanup();

private:
    SoundManager() = default;

    sf::SoundBuffer bulletSoundBuffer;
    std::vector<std::unique_ptr<sf::Sound>> activeSounds;
    bool soundsLoaded = false;

    void removeFinishedSounds();
};