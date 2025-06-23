#include <SFML/Graphics.hpp>
#include "GameConstants.h"
#include "Enemy.h"
#include "Shop.h"

int main()
{
    // Window setup
    sf::RenderWindow window(
        sf::VideoMode(
            static_cast<unsigned>((Game::MAP_WIDTH + 3) * Game::TILE_SIZE),
            static_cast<unsigned>(Game::MAP_HEIGHT * Game::TILE_SIZE)
        ),
        "Tower Defense"
    );
    window.setFramerateLimit(60);

    // Game objects
    Enemy enemy;
    Shop shop;
    sf::Clock clock;
    sf::Event event;

    // Main game loop
    while (window.isOpen())
    {
        // Event handling
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            shop.handleEvent(event, window);
        }

        // Update game state
        float deltaTime = clock.restart().asSeconds();
        enemy.update(deltaTime);
        shop.update(window);

        // Render
        window.clear(sf::Color::White);

        // Draw game objects
        enemy.draw(window);
        shop.draw(window);

        window.display();
    }

    return 0;
}