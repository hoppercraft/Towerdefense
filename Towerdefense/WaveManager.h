#pragma once
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
    void startNextWave();
    void reset();
    bool allWavesComplete() const { return currentWaveNumber > maxWaves; }

private:
    static const float cooldownDuration;
    static const int maxWaves = 10;

    // Wave management
    int currentWaveNumber;
    bool currentWaveActive;
    int enemiesSpawnedInWave;
    int totalEnemiesInWave;
    float currentSpawnInterval;
    float waveCooldownTime;
    bool inCooldown;

    // Wave configurations
    std::vector<WaveConfig> waves;
    void initializeWaves();
    WaveConfig generateWaveConfig(int waveNumber);
    void spawnEnemy(std::vector<Enemy*>& enemies, bool isFast, float healthMultiplier, float speedMultiplier);

    // Timers
    sf::Clock spawnTimer;
    sf::Clock waveTimer;

    // UI elements
    sf::Font font;
    bool fontLoaded;
    std::optional<sf::Text> waveText;
    std::optional<sf::Text> cooldownText;

    sf::RectangleShape waveProgressBackground;
    sf::RectangleShape waveProgressBar;

    void updateUI();
};