#include "Shop.h"
#include "GameConstants.h"

Shop::Shop()
{
    // Setup the sidebar
    bar.setPosition(sf::Vector2f(
        static_cast<float>(Game::MAP_WIDTH * Game::TILE_SIZE),
        0.f
    ));
    bar.setSize(sf::Vector2f(
        static_cast<float>(Game::TILE_SIZE * 3),
        static_cast<float>(Game::TILE_SIZE * Game::MAP_HEIGHT)
    ));
    bar.setFillColor(sf::Color(0x7C5123FF));
    bar.setOutlineThickness(2.f);
    bar.setOutlineColor(sf::Color(0xA0ACAAFF));

    // Create tower frames and towers
    for (int i = 0; i < 3; ++i)
    {
        // Create frame for tower
        sf::RectangleShape frame;
        frame.setSize(sf::Vector2f(
            static_cast<float>(Game::TILE_SIZE) * 1.75f,
            static_cast<float>(Game::TILE_SIZE) * 1.75f
        ));
        frame.setOrigin(frame.getSize().x / 2.f, frame.getSize().y / 2.f);
        frame.setPosition(sf::Vector2f(
            static_cast<float>(Game::MAP_WIDTH * Game::TILE_SIZE) + bar.getSize().x / 2.f,
            static_cast<float>(Game::TILE_SIZE) * (1.25f + i * 2.f)
        ));
        frame.setFillColor(sf::Color(0xC4B093FF));
        frames.push_back(frame);

        // Create tower
        sf::CircleShape tower(7.f);
        tower.setFillColor(sf::Color(0x0000FFFF));
        tower.setOrigin(tower.getRadius(), tower.getRadius());
        tower.setPosition(frame.getPosition());
        towers.push_back(tower);
    }
}

void Shop::draw(sf::RenderWindow& window) const
{
    window.draw(bar);

    for (const auto& frame : frames)
        window.draw(frame);

    for (const auto& tower : towers)
        window.draw(tower);

    if (dragging)
        window.draw(draggedTower);

    for (const auto& tower : deployedTowers)
        window.draw(tower);
}

void Shop::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            for (size_t i = 0; i < towers.size(); ++i)
            {
                if (towers[i].getGlobalBounds().contains(mousePos))
                {
                    dragging = true;
                    draggedTower = towers[i];
                    draggedTower.setFillColor(sf::Color(0x0000FF55));
                    draggedTower.setPosition(mousePos);
                    break;
                }
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left && dragging)
        {
            dragging = false;
            auto worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            int tileX = static_cast<int>(worldPos.x) / Game::TILE_SIZE;
            int tileY = static_cast<int>(worldPos.y) / Game::TILE_SIZE;

            if (tileX >= 0 && tileX < Game::MAP_WIDTH &&
                tileY >= 0 && tileY < Game::MAP_HEIGHT &&
                Game::gameMap[tileY][tileX] == Game::Grass)
            {
                draggedTower.setFillColor(sf::Color(0x0000FFFF));
                draggedTower.setPosition(sf::Vector2f(
                    static_cast<float>(tileX * Game::TILE_SIZE) + Game::TILE_SIZE / 2.f,
                    static_cast<float>(tileY * Game::TILE_SIZE) + Game::TILE_SIZE / 2.f
                ));
                deployedTowers.push_back(draggedTower);
            }
        }
    }
}

void Shop::update(const sf::RenderWindow& window)
{
    if (dragging)
    {
        auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        draggedTower.setPosition(mousePos);
    }
}

const std::vector<sf::CircleShape>& Shop::getDeployedTowers() const
{
    return deployedTowers;
}