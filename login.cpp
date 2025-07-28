#include "login.hpp"
#include <map>

namespace CS {

class TextBox {
public:
    sf::RectangleShape box;
    sf::Text text;
    bool active = false;
    bool isPassword = false;
    std::string input;

    TextBox(const sf::Font& font, sf::Vector2f pos, sf::Vector2f size) {
        box.setPosition(pos);
        box.setSize(size);
        box.setFillColor(sf::Color(230, 230, 230));
        box.setOutlineColor(sf::Color::Blue);
        box.setOutlineThickness(2.f);

        text.setFont(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::Black);
        text.setPosition(pos + sf::Vector2f(5.f, 5.f));
    }

    void draw(sf::RenderWindow& win) {
        win.draw(box);
        win.draw(text);
    }

    void handleClick(sf::Vector2f pt) {
        active = box.getGlobalBounds().contains(pt);
        box.setOutlineColor(active ? sf::Color::Red : sf::Color::Blue);
    }

    void handleText(sf::Uint32 ch) {
        if (!active) return;
        if (ch == '\b') {
            if (!input.empty()) input.pop_back();
        } else if (ch < 128 && isprint(ch)) {
            if(input.size() < 20)
                input += static_cast<char>(ch);
        }
        std::string displayText = isPassword ? std::string(input.length(), '*') : input;
        text.setString(displayText);
    }
};

bool runLoginWindow(const sf::Font& font, std::string& username_out) {
    sf::RenderWindow window(sf::VideoMode(500, 350), "Game Login");

    std::map<std::string, std::string> credentials = {
        {"Sudha","2525"},
        {"player", "1234"},
        {"admin", "adminpass"},
        {"guest", "guest123"}
    };

    TextBox username(font, {100, 80}, {300, 40});
    TextBox password(font, {100, 150}, {300, 40});
    password.isPassword = true;

    sf::Text labelU("Username:", font, 20);
    labelU.setPosition(100, 55);
    sf::Text labelP("Password:", font, 20);
    labelP.setPosition(100, 125);

    sf::RectangleShape loginBtn({150, 40});
    loginBtn.setPosition(175, 230);
    loginBtn.setFillColor(sf::Color(70, 160, 70));
    sf::Text loginTxt("Login", font, 22);
    loginTxt.setFillColor(sf::Color::White);
    loginTxt.setPosition(220, 235);

    sf::Text statusMsg("", font, 20);
    statusMsg.setPosition(100, 280);
    statusMsg.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
                return false;
            } 
            else if (ev.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);
                username.handleClick(m);
                password.handleClick(m);

                if (loginBtn.getGlobalBounds().contains(m)) {
                    auto it = credentials.find(username.input);
                    if (it != credentials.end() && it->second == password.input) {
                        username_out = username.input;
                        window.close();
                        return true;
                    } else {
                        statusMsg.setString("Incorrect login credentials.");
                        password.input.clear();
                        password.text.setString("");
                    }
                }
            }
            else if (ev.type == sf::Event::TextEntered) {
                username.handleText(ev.text.unicode);
                password.handleText(ev.text.unicode);
            }
            else if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) {
                auto it = credentials.find(username.input);
                if (it != credentials.end() && it->second == password.input) {
                    username_out = username.input;
                    window.close();
                    return true;
                } else {
                    statusMsg.setString("Incorrect login credentials.");
                    password.input.clear();
                    password.text.setString("");
                }
            }
        }

        window.clear(sf::Color(200, 200, 200));
        window.draw(labelU);
        window.draw(labelP);
        username.draw(window);
        password.draw(window);
        window.draw(loginBtn);
        window.draw(loginTxt);
        window.draw(statusMsg);
        window.display();
    }

    return false;
}

} // namespace CS
