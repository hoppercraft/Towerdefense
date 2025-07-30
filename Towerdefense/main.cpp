#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameConstants.h"
#include "Towers.h"
#include "Renders.h"
#include "Enemy.h"
#include "Bullet.h"
#include "WaveManager.h"
#include <cstdlib>

// Local PlayerInfo class definition (to avoid conflicts with gamesession files)
class GamePlayerInfo {
public:
    GamePlayerInfo(int coin = 500, int health = 5);
    void draw(sf::RenderWindow& window);
    void turrentplaced(int cost);
    void enemypassed(int healthlost);
    bool enoughmoney(int cost);
    void notEnoughMoneywarning();
    void coinsearned(int m);
    bool gameover();

private:
    sf::Font font;
    sf::Text cointext;
    sf::Text hearttext;
    sf::Text warningtext;
    sf::CircleShape coinShape;
    sf::ConvexShape heartShape;
    int coins;
    int health;
    int i;
    sf::Clock timer;
    sf::Time offset;
    sf::Time elapsed;
};

// GamePlayerInfo implementation
GamePlayerInfo::GamePlayerInfo(int coin, int health)
    : coins(coin), health(health), cointext(font, std::to_string(coin), 80), hearttext(font, std::to_string(health), 80), warningtext(font, "", 170) {
    font.openFromFile("ARIAL.ttf");
    hearttext.setScale({ 0.105f, 0.105f });
    hearttext.setFillColor(sf::Color::White);
    hearttext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE + 30 ,Game::MAP_HEIGHT * Game::TILE_SIZE - 15 });
    heartShape.setPointCount(9);
    heartShape.setPoint(0, sf::Vector2f(110.f, -40.f));   // Bottom point
    heartShape.setPoint(1, sf::Vector2f(180.f, 40.f));   // Right low
    heartShape.setPoint(2, sf::Vector2f(200.f, 80.f));   // Right curve outer
    heartShape.setPoint(3, sf::Vector2f(150.f, 150.f));  // Top right bump
    heartShape.setPoint(4, sf::Vector2f(100.f, 120.f));  // Center dip
    heartShape.setPoint(5, sf::Vector2f(50.f, 150.f));   // Top left bump
    heartShape.setPoint(6, sf::Vector2f(0.f, 80.f));    // Left curve outer
    heartShape.setPoint(7, sf::Vector2f(20.f, 40.f));
    heartShape.setPoint(8, sf::Vector2f(90.f, -40.f));
    heartShape.setRotation(sf::degrees(180.f));
    heartShape.setScale({ 0.06f,0.06f });
    // Style it
    heartShape.setFillColor(sf::Color(255, 50, 50)); // Red heart
    heartShape.setOutlineThickness(10.0f);
    heartShape.setOutlineColor(sf::Color::Black);
    // Position it next to heart text
    heartShape.setPosition({
        hearttext.getPosition().x - 4,
        hearttext.getPosition().y + 10
        });
    cointext.setScale({ 0.105f, 0.105f });
    cointext.setFillColor(sf::Color::White);
    cointext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE + 30 ,Game::MAP_HEIGHT * Game::TILE_SIZE - 30 });
    coinShape.setRadius(5.f);
    coinShape.setFillColor(sf::Color(255, 215, 0));  // Gold color
    coinShape.setPointCount(10);
    coinShape.setOutlineColor(sf::Color(218, 165, 32));  // Darker gold
    coinShape.setOutlineThickness(1.5f);
    coinShape.setPosition({ cointext.getPosition().x - 15,cointext.getPosition().y });
    warningtext.setPosition({ Game::MAP_WIDTH * Game::TILE_SIZE / 2 - 50, Game::MAP_HEIGHT * Game::TILE_SIZE / 2 - 50 });
    warningtext.setScale({ 0.105f, 0.105f });
    warningtext.setFillColor(sf::Color(255, 0, 0));
    warningtext.setStyle(sf::Text::Bold);
}

void GamePlayerInfo::draw(sf::RenderWindow& window) {
    window.draw(cointext);
    window.draw(coinShape);
    window.draw(hearttext);
    window.draw(heartShape);
    if (timer.getElapsedTime().asSeconds() <= 1.f) {
        window.draw(warningtext);
    }
}

void GamePlayerInfo::turrentplaced(int cost) {
    coins -= cost;
    cointext.setString(std::to_string(coins));
}

void GamePlayerInfo::coinsearned(int m) {
    coins += m;
    cointext.setString(std::to_string(coins));
}

void GamePlayerInfo::enemypassed(int healthlost) {
    health -= healthlost;
    hearttext.setString(std::to_string(health));
}

bool GamePlayerInfo::enoughmoney(int cost) {
    if (coins >= cost) {
        return(true);
    }
    else {
        return(false);
    }
}

void GamePlayerInfo::notEnoughMoneywarning() {
    warningtext.setString("Not enough money");
    timer.restart();
}

bool GamePlayerInfo::gameover() {
    if (health <= 0) {
        return(true);
    }
    else {
        return(false);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({ (Game::MAP_WIDTH + 3) * Game::TILE_SIZE, Game::MAP_HEIGHT * Game::TILE_SIZE }), "Tower Defense Map - Multi Tower Edition");
    window.setFramerateLimit(60);

    std::vector<Enemy*> enemies;
    WaveManager waveManager;  // Create wave manager

    Shop shop;
    const float speed = 25.0f;
    GamePlayerInfo playerinfo;
    sf::Clock deltaClock;

    // Load tile texture
    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("Sprites/tile.png")) {
        std::cerr << "Failed to load tile texture.\n";
        return -1;
    }

    sf::IntRect tileRects[4];
    for (int i = 0; i < 4; ++i) {
        tileRects[i] = sf::IntRect({ 20 * i + 1, 0 }, { 20, 20 });
    }
    sf::Sprite tile(tileTexture);

    // Main game loop
    while (window.isOpen()) {
        // Handle events
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
                // Optional: Allow player to start next wave early with spacebar
                else if (keypressed->scancode == sf::Keyboard::Scancode::Space) {
                    waveManager.startNextWave();
                }
                // Optional: Reset game with R key
                else if (keypressed->scancode == sf::Keyboard::Scancode::R) {
                    // Clear all enemies
                    for (auto* enemy : enemies) {
                        delete enemy;
                    }
                    enemies.clear();
                    waveManager.reset();
                }
            }
            // Handle shop events (tower selection, dragging, deployment)
            shop.handleEvent(event.value(), window);
        }

        // Update game state
        float deltaTime = deltaClock.restart().asSeconds();

        // Update wave manager (handles enemy spawning)
        waveManager.update(deltaTime, enemies);

        // Update shop (tower targeting, bullet updates)
        shop.Towertarget(enemies, deltaTime);
        shop.update(window, deltaTime);

        // Handle tower purchases and integrate with player info
        // This should be handled by connecting shop events to playerinfo
        // You may need to modify your Shop class to use playerinfo.enoughmoney() and playerinfo.turrentplaced()

        // Update enemies
        for (auto& enemy : enemies) {
            enemy->update(speed * deltaTime);
        }

        // Render everything
        window.clear();

        // Draw map tiles
        for (int row = 0; row < Game::MAP_HEIGHT; ++row) {
            for (int col = 0; col < Game::MAP_WIDTH; ++col) {
                int tileType = Game::Map1[row][col];
                tile.setTextureRect(tileRects[tileType]);
                tile.setPosition({ static_cast<float>(col * Game::TILE_SIZE), static_cast<float>(row * Game::TILE_SIZE) });
                window.draw(tile);
            }
        }

        // Update and draw enemies
        for (auto it = enemies.begin(); it != enemies.end(); ) {
            if (!(*it)->isAlive) {
                // Give coins when enemy dies (you may need to adjust the coin amount)
                playerinfo.coinsearned(10); // Example: 10 coins per enemy killed
                delete* it;
                it = enemies.erase(it);
            }
            else {
                (*it)->update(speed * deltaTime);
                if (!(*it)->isMoving) {
                    // Enemy reached the end - reduce player health
                    playerinfo.enemypassed(1); // Lose 1 health per enemy that passes
                    (*it)->isAlive = false;
                }
                (*it)->draw(window);
                ++it;
            }
        }

        // Draw shop UI and towers
        shop.draw(window);

        // Draw player info
        playerinfo.draw(window);

        // Draw wave manager UI
        waveManager.draw(window);

        // Check for game over conditions
        if (waveManager.allWavesComplete()) {
            // You could add victory screen logic here
            // For now, just display victory message in console
            static bool victoryShown = false;
            if (!victoryShown) {
                std::cout << "Victory! All waves completed!\n";
                victoryShown = true;
            }
        }

        // Check for game over (player health <= 0)
        if (playerinfo.gameover()) {
            // You could add game over screen logic here
            // For now, just display game over message in console
            static bool gameOverShown = false;
            if (!gameOverShown) {
                std::cout << "Game Over! Player health reached 0!\n";
                gameOverShown = true;
            }
        }

        window.display();
    }

    // Clean up enemies
    for (auto* enemy : enemies) {
        delete enemy;
    }

    return 0;
}