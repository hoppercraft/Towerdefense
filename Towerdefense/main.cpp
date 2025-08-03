#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameConstants.h"
#include "Towers.h"
#include "Renders.h"
#include "Enemy.h"
#include "Bullet.h"
#include <cstdlib>
#include"gamesession.h"
#include "SoundManager.h"
#include"WaveManager.h"
#include<SFML/Audio.hpp>
int main() {
    sf::RenderWindow window(sf::VideoMode({ (Game::MAP_WIDTH + 3) * Game::TILE_SIZE, Game::MAP_HEIGHT * Game::TILE_SIZE }), "Tower Defense Map");
    window.setFramerateLimit(60);

    std::vector<Enemy*> enemies;
    WaveManager waveManager;

    sf::Music backgroundMusic;
    bool musicLoaded = false;

    // Load bullet sounds using SoundManager
    if (!SoundManager::getInstance().loadSounds()) {
        std::cerr << "Warning: Could not load bullet sounds. Game will continue without bullet sound effects.\n";
    }

    sf::Clock spawnClock;
    float spawnInterval = 2.f;
    int maxEnemies = 20;
    int spawnedEnemies = 0;

    Shop shop;
    const float speed = 7.0f;
    PlayerInfo playerinfo;
    sf::Clock deltaClock;

    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("Sprites/tile.png")) {
        std::cerr << "Failed to load tile texture.\n";
        return -1;
    }

    if (backgroundMusic.openFromFile("Sounds\\background.wav")) {
        backgroundMusic.setLooping(true);
        backgroundMusic.setVolume(40);
        backgroundMusic.play();
        musicLoaded = true;
        std::cout << "Background music started.\n";
    }
    else {
        std::cerr << "Could not load background music.\n";
    }

    sf::IntRect tileRects[4];
    for (int i = 0; i < 4; ++i) {
        tileRects[i] = sf::IntRect({ 20 * i + 1,0 }, { 20,20 });
    }

    sf::Sprite tile(tileTexture);

    sf::Font font;
    if (!font.openFromFile("font\\Arial.ttf")) {
        std::cerr << "Failed to load font file!\n";
        return -1;
    }
    sf::Text gameovertext(font,"GAME OVER", 170);
    gameovertext.setScale({ 0.105f, 0.105f });
    gameovertext.setStyle(sf::Text::Bold);
    gameovertext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE / 2-50, Game::MAP_HEIGHT * Game::TILE_SIZE / 2 -50});
    gameovertext.setFillColor(sf::Color(255, 0, 0));

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                else if (keypressed->scancode == sf::Keyboard::Scancode::Space) {
                    waveManager.startNextWave();
                }
                else if (keypressed->scancode == sf::Keyboard::Scancode::M) {
                    if (musicLoaded) {
                        if (backgroundMusic.getStatus() == sf::SoundSource::Status::Playing) {
                            backgroundMusic.pause();
                        }
                        else {
                            backgroundMusic.play();
                        }
                    }
                }
            }
            
            shop.handleEvent(event.value(), window, &playerinfo);
        }
        
        SoundManager::getInstance().update();

        if (!playerinfo.gameover()) {
            float deltaTime = deltaClock.restart().asSeconds();
            waveManager.update(deltaTime, enemies);
            shop.Towertarget(enemies, deltaTime);
            shop.update(window, deltaTime);
            

            for (auto& enemy : enemies)
                enemy->update(speed * deltaTime);

            window.clear();

            for (int row = 0; row < Game::MAP_HEIGHT; ++row) {
                for (int col = 0; col < Game::MAP_WIDTH; ++col) {
                    int tileType = Game::Map1[row][col];
                    tile.setTextureRect(tileRects[tileType]);
                    tile.setPosition({ static_cast<float>(col * Game::TILE_SIZE), static_cast<float>(row * Game::TILE_SIZE) });
                    window.draw(tile);
                }
            }
            for (auto it = enemies.begin(); it != enemies.end(); ) {
                if (!(*it)->isAlive) {
                    playerinfo.coinsearned((*it)->coindropped, (*it)->coindropped);
                    delete* it;
                    it = enemies.erase(it);
                }
                else {
                    (*it)->update(speed * deltaTime);
                    if (!(*it)->isMoving) {
                        playerinfo.enemypassed(1);
                        (*it)->isAlive = false;
                        delete* it;
                        it = enemies.erase(it);
                        continue;
                    }
                    (*it)->draw(window);
                    ++it;
                }
            }
            for (auto it = enemies.begin(); it != enemies.end(); ) {
                if (!(*it)->isAlive) {
                    delete* it;
                    it = enemies.erase(it);
                    playerinfo.coinsearned(50);
                }
                else {
                    (*it)->update(speed * deltaTime);
                    if (!(*it)->isMoving) {
                        playerinfo.enemypassed(1);
                        (*it)->isAlive = false;
                        delete* it;
                        it = enemies.erase(it);
                        continue;
                    }
                    (*it)->draw(window);
                    ++it;
                }
            }
        }
        
        shop.draw(window);
        playerinfo.draw(window);
        waveManager.draw(window);
        if (waveManager.allWavesComplete()) {
            static bool victoryShown = false;
            if (!victoryShown) {
                std::cout << "Victory! All waves completed!\n";
                victoryShown = true;
            }
        }
        if (playerinfo.gameover()) {
            window.draw(gameovertext);
        }
        window.display();
    }
    if (musicLoaded) {
        backgroundMusic.stop();
    }

    SoundManager::getInstance().cleanup();

    return 0;
}
