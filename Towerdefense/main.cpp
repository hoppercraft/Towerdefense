#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Window.hpp>
int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "My window");
    window.setFramerateLimit(60);
    sf::ConvexShape convex;

    convex.setPointCount(6);
    convex.setPoint(0, { 13.0f,17.0f });
    convex.setPoint(1, { 3.5f,1.6f });
    convex.setPoint(2, { 0.25f,-12.0f });
    convex.setPoint(3, { -12.0f,-7.3f });
    convex.setPoint(4, { -12.5f,-1.6f });
    convex.setPoint(5, { -5.0f,7.5f });
    convex.setOrigin(convex.getGeometricCenter());
    convex.setFillColor(sf::Color(0x3F00FFFF));
    convex.setOutlineThickness(2.4f);
    convex.setOutlineColor(sf::Color(0xFF8888FF));
    convex.setPosition({ 800 / 2.0f, 600 / 2.0f });
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
        //render
        window.clear();
        window.draw(convex);
        //drawing
        window.display();
    }

}