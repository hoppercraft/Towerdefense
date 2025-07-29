#pragma warning(disable: 4275) // Suppress DLL interface warning
#include "WaveManager.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

const float WaveManager::cooldownDuration = 10.0f;

WaveManager::WaveManager() :
    currentWaveNumber(1),
    currentWaveActive(false),
    enemiesSpawnedInWave(0),
    totalEnemiesInWave(0),
    currentSpawnInterval(1.0f),
    waveCooldownTime(0.0f),
    inCooldown(true),
    fontLoaded(false) {

    initializeWaves();

    // Try to initialize UI elements - make it optional
    if (font.openFromFile("arial.ttf")) {
        fontLoaded = true;
    }
    else if (font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        fontLoaded = true;
    }
    else if (font.openFromFile("/System/Library/Fonts/Arial.ttf")) {
        fontLoaded = true;
    }
    else if (font.openFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        fontLoaded = true;
    }

    if (fontLoaded) {
        waveText.emplace(font);
        waveText->setCharacterSize(120);
        waveText->setFillColor(sf::Color::White);
        waveText->setScale({ 0.05f, 0.05f });
        waveText->setPosition({ 0.f, 0.f });  // SFML 3.0: use braced initialization
        waveText->setStyle(sf::Text::Bold);

        cooldownText.emplace(font);
        cooldownText->setCharacterSize(120);
        cooldownText->setFillColor(sf::Color::Yellow);
        cooldownText->setScale({ 0.05f, 0.05f });
        cooldownText->setPosition({ 0.f, 10.f });  // SFML 3.0: use braced initialization
        cooldownText->setStyle(sf::Text::Regular);
    }
    else {
        std::cerr << "Warning: Could not load any font file. Text will not be displayed.\n";
    }

    // Progress bar setup - adjusted position to account for text changes
    waveProgressBackground.setSize({ 40.f, 4.f });  // SFML 3.0: use braced initialization
    waveProgressBackground.setPosition({ 0.f, 20.f });  // SFML 3.0: use braced initialization
    waveProgressBackground.setFillColor(sf::Color(40, 40, 40));
    waveProgressBackground.setOutlineThickness(1.f);
    waveProgressBackground.setOutlineColor(sf::Color::White);

    waveProgressBar.setSize({ 40.f, 4.f });  // SFML 3.0: use braced initialization
    waveProgressBar.setPosition({ 0.f, 20.f });  // SFML 3.0: use braced initialization
    waveProgressBar.setFillColor(sf::Color::Green);
}

void WaveManager::initializeWaves() {
    waves.clear();
    for (int i = 1; i <= maxWaves; ++i) {
        waves.push_back(generateWaveConfig(i));
    }
}

WaveConfig WaveManager::generateWaveConfig(int waveNumber) {
    WaveConfig config;

    // Base enemies increase with each wave
    config.normalEnemies = 5 + (waveNumber - 1) * 3;
    config.fastEnemies = std::max(0, (waveNumber - 2) * 2);

    // Spawn interval decreases (faster spawning) but not too fast
    config.spawnInterval = std::max(0.3f, 1.5f - (waveNumber - 1) * 0.1f);

    // Health and speed multipliers increase gradually
    config.enemyHealthMultiplier = 1.0f + (waveNumber - 1) * 0.2f;
    config.enemySpeedMultiplier = 1.0f + (waveNumber - 1) * 0.1f;

    return config;
}

void WaveManager::update(float deltaTime, std::vector<Enemy*>& enemies) {
    if (allWavesComplete()) {
        return;
    }

    if (inCooldown) {
        waveCooldownTime += deltaTime;
        if (waveCooldownTime >= cooldownDuration) {
            inCooldown = false;
            currentWaveActive = true;
            waveCooldownTime = 0.0f;
            enemiesSpawnedInWave = 0;

            if (currentWaveNumber <= maxWaves) {
                const WaveConfig& wave = waves[currentWaveNumber - 1];
                totalEnemiesInWave = wave.normalEnemies + wave.fastEnemies;
                currentSpawnInterval = wave.spawnInterval;
            }
        }
    }

    if (currentWaveActive && currentWaveNumber <= maxWaves) {
        const WaveConfig& currentWave = waves[currentWaveNumber - 1];

        // Spawn enemies based on timer
        if (spawnTimer.getElapsedTime().asSeconds() >= currentSpawnInterval &&
            enemiesSpawnedInWave < totalEnemiesInWave) {

            // Determine enemy type based on wave configuration
            bool spawnFast = false;
            int normalEnemiesSpawned = std::min(enemiesSpawnedInWave, currentWave.normalEnemies);

            if (enemiesSpawnedInWave >= currentWave.normalEnemies) {
                spawnFast = true;
            }
            else if (currentWave.fastEnemies > 0) {
                // Mix normal and fast enemies
                float ratio = static_cast<float>(currentWave.fastEnemies) /
                    static_cast<float>(currentWave.normalEnemies + currentWave.fastEnemies);
                spawnFast = (static_cast<float>(rand()) / RAND_MAX) < ratio;
            }

            spawnEnemy(enemies, spawnFast, currentWave.enemyHealthMultiplier, currentWave.enemySpeedMultiplier);
            spawnTimer.restart();
            enemiesSpawnedInWave++;
        }

        // Check if wave is complete
        if (enemiesSpawnedInWave >= totalEnemiesInWave) {
            // Check if all enemies from this wave are gone
            bool allEnemiesGone = true;
            for (const auto* enemy : enemies) {
                if (enemy->isAlive) {
                    allEnemiesGone = false;
                    break;
                }
            }

            if (allEnemiesGone || enemies.empty()) {
                currentWaveActive = false;
                currentWaveNumber++;
                inCooldown = true;
                waveCooldownTime = 0.0f;
            }
        }
    }

    updateUI();
}

void WaveManager::spawnEnemy(std::vector<Enemy*>& enemies, bool isFast, float healthMultiplier, float speedMultiplier) {
    Enemy* newEnemy = nullptr;

    if (isFast) {
        newEnemy = new FastEnemy();
    }
    else {
        newEnemy = new Enemy();
    }

    // Apply difficulty modifiers
    newEnemy->Health *= healthMultiplier;
    newEnemy->maxHealth *= healthMultiplier;
    newEnemy->speed *= speedMultiplier;

    enemies.push_back(newEnemy);
}

void WaveManager::updateUI() {
    if (allWavesComplete()) {
        if (fontLoaded && waveText.has_value() && cooldownText.has_value()) {
            waveText->setString("All Waves Complete!");
            cooldownText->setString("");
        }
        waveProgressBar.setSize({ 40.f, 4.f });  // SFML 3.0: use braced initialization
        return;
    }

    // Update UI only if font is loaded and text objects exist
    if (fontLoaded && waveText.has_value() && cooldownText.has_value()) {
        // Update wave text
        waveText->setString("Wave: " + std::to_string(currentWaveNumber) + "/" + std::to_string(maxWaves));

        // Update cooldown text
        if (inCooldown) {
            float remaining = cooldownDuration - waveCooldownTime;
            cooldownText->setString("Next wave in: " + std::to_string(static_cast<int>(remaining + 1)) + "s");
        }
        else if (currentWaveActive) {
            cooldownText->setString("Wave Active - Enemies: " +
                std::to_string(enemiesSpawnedInWave) + "/" +
                std::to_string(totalEnemiesInWave));
        }
    }

    // Update progress bar
    float progress = 0.0f;
    if (inCooldown) {
        progress = waveCooldownTime / cooldownDuration;
    }
    else if (currentWaveActive && totalEnemiesInWave > 0) {
        progress = static_cast<float>(enemiesSpawnedInWave) / static_cast<float>(totalEnemiesInWave);
    }

    waveProgressBar.setSize({ progress * 40.f, 4.f });  // SFML 3.0: use braced initialization

    // Change progress bar color based on state
    if (inCooldown) {
        waveProgressBar.setFillColor(sf::Color::Blue);
    }
    else {
        waveProgressBar.setFillColor(sf::Color::Green);
    }
}

void WaveManager::draw(sf::RenderWindow& window) {
    // Always draw progress bars
    window.draw(waveProgressBackground);
    window.draw(waveProgressBar);

    // Only draw text if font is loaded and text objects exist
    if (fontLoaded && waveText.has_value() && cooldownText.has_value()) {
        window.draw(*waveText);
        window.draw(*cooldownText);
    }
}

void WaveManager::startNextWave() {
    if (inCooldown) {
        inCooldown = false;
        currentWaveActive = true;
        waveCooldownTime = 0.0f;
        enemiesSpawnedInWave = 0;

        if (currentWaveNumber <= maxWaves) {
            const WaveConfig& wave = waves[currentWaveNumber - 1];
            totalEnemiesInWave = wave.normalEnemies + wave.fastEnemies;
            currentSpawnInterval = wave.spawnInterval;
        }
    }
}

void WaveManager::reset() {
    currentWaveNumber = 1;
    currentWaveActive = false;
    enemiesSpawnedInWave = 0;
    totalEnemiesInWave = 0;
    currentSpawnInterval = 1.0f;
    waveCooldownTime = 0.0f;
    inCooldown = true;
    spawnTimer.restart();
    waveTimer.restart();
}