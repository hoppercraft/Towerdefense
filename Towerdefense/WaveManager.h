#pragma once
#pragma warning(disable: 4275) // Suppress DLL interface warning
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include "Enemy.h"

struct WaveConfig {
    int normalEnemies;
    int fastEnemies;
    float spawnInterval;
    float enemyHealthMultiplier;
    float enemySpeedMultiplier;
};

class WaveManager {
public:
    WaveManager();
    void update(float deltaTime, std::vector<Enemy*>& enemies);
    void draw(sf::RenderWindow& window);
    bool isWaveActive() const { return currentWaveActive; }
    int getCurrentWave() const { return currentWaveNumber; }
    float getWaveCooldownTime() const { return waveCooldownTime; }
    bool isInCooldown() const { return inCooldown; }

    bool allWavesComplete() const { return currentWaveNumber > maxWaves; }
    void startNextWave();

    void loadSavedWaveState(int wave, bool active, float cooldown, bool cooldownState) {
        currentWaveNumber = wave;
        currentWaveActive = active;
        waveCooldownTime = cooldown;
        inCooldown = cooldownState;

        if (currentWaveNumber <= maxWaves) {
            const WaveConfig& waveConfig = waves[currentWaveNumber - 1];
        }

        updateUI();
    }


    
private:
    // Wave configuration
    std::vector<WaveConfig> waves;

    // Current wave state
    int currentWaveNumber;
    bool currentWaveActive;
    int enemiesSpawnedInWave;
    int totalEnemiesInWave;

    // Timing
    sf::Clock spawnTimer;
    sf::Clock waveTimer;
    float currentSpawnInterval;
    float waveCooldownTime;
    bool inCooldown;

    // UI elements - using optional for SFML 3.0 compatibility
    sf::Font font;
    std::optional<sf::Text> waveText;
    std::optional<sf::Text> cooldownText;
    bool fontLoaded;
    sf::RectangleShape waveProgressBar;
    sf::RectangleShape waveProgressBackground;

    // Constants
    static const int maxWaves = 50;
    static const float cooldownDuration;

    // Helper methods
    void initializeWaves();
    void spawnEnemy(std::vector<Enemy*>& enemies, bool isFast, float healthMultiplier, float speedMultiplier);
    void updateUI();
    WaveConfig generateWaveConfig(int waveNumber);
};