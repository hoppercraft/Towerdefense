#include <SFML/Graphics.hpp>
#include <iostream>

enum TileType {
    Grass = 0,
    Water = 1,
    Path = 2,
    Tree = 3
};

const int TILE_SIZE = 20;
const int MAP_WIDTH = 13;
const int MAP_HEIGHT = 8;

int map[MAP_HEIGHT][MAP_WIDTH] = {
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 3, 3, 0, 0, 0, 0, 1, 1, 0, 0, 0},
    {2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 3, 0, 0, 2, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 3, 3, 0, 2, 2, 2, 2, 2, 2},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

class Enemy {
public:
    Enemy(int startX, int startY) {
        shape.setRadius(7);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition({ static_cast<float>(startX * TILE_SIZE - 10), static_cast<float>(startY * TILE_SIZE + 10) });
        shape.setOrigin(shape.getGeometricCenter());
        for (int y = 0; y < MAP_HEIGHT; ++y)
            for (int x = 0; x < MAP_WIDTH; ++x)
                visited[y][x] = false;

        findPath(startX, startY);
    }

    void update(float speed) {
        if (currentStep < path.size()) {
            sf::Vector2f target(static_cast<float>(path[currentStep].x * TILE_SIZE + 10), static_cast<float>(path[currentStep].y * TILE_SIZE + 10));
            sf::Vector2f current = shape.getPosition();
            sf::Vector2f delta = target - current;

            float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);
            if (dist < speed) {
                shape.setPosition(target);
                currentStep++;
            }
            else {
                delta /= dist;
                shape.move(delta * speed);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

private:
    sf::CircleShape shape;
    std::vector<sf::Vector2i> path;
    bool visited[MAP_HEIGHT][MAP_WIDTH];
    size_t currentStep = 0;

    const std::vector<sf::Vector2i> directions = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}
    };
        
    void findPath(int x, int y) {
        if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT)
            return;
        if (map[y][x] != TileType::Path || visited[y][x])
            return;

        visited[y][x] = true;
        path.push_back({ x, y });

        for (auto dir : directions)
            findPath(x + dir.x, y + dir.y);
        if (path.size() == 1 || x != path[0].x || y != path[0].y)
            return;

        sf::Vector2i last = path.back();
        path.push_back({ last.x+1, last.y });
    }

};
int main() {
    sf::RenderWindow window(sf::VideoMode({ MAP_WIDTH * TILE_SIZE, MAP_HEIGHT * TILE_SIZE }), "Tower Defense Map");
    window.setFramerateLimit(60);
    Enemy enemy(0, 3);

    const float speed = 25.0f;

    sf::Clock deltaClock;

    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("Sprites/tile.png")) {
        std::cerr << "Failed to load tile texture.\n";
        return -1;
    }

    sf::IntRect tileRects[4];
    for (int i = 0; i < 4; ++i) {
        tileRects[i] = sf::IntRect({ 20 * i + 1,0 }, { 20,20 });
    }

    sf::Sprite tile(tileTexture);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            }
        }
        float deltaTime = deltaClock.restart().asSeconds();

        enemy.update(speed * deltaTime);
        window.clear();

        for (int row = 0; row < MAP_HEIGHT; ++row) {
            for (int col = 0; col < MAP_WIDTH; ++col) {
                int tileType = map[row][col];
                tile.setTextureRect(tileRects[tileType]);
                tile.setPosition({ static_cast<float>(col * TILE_SIZE), static_cast<float>(row * TILE_SIZE) });
                window.draw(tile);
            }
        }

        enemy.draw(window);
        window.display();
    }

    return 0;
}
