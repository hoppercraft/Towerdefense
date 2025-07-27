#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <mysql.h>
#include <Windows.h>
#include "EnvLoader.h"


auto env = loadEnv("Security.env");

const char* HOST = env["HOST"].c_str();
const char* USER = env["USER"].c_str();
const char* PASS = env["PASS"].c_str();
const char* DB = env["DB"].c_str();

enum class AppState {
    Login,
    PlayerUI
};

int main()
{
    MYSQL* conn;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, HOST, USER, PASS, DB, 4134, NULL, 0)) {
        std::cout << "MySQL Connection Error: " << mysql_error(conn) << std::endl;
        return 1;
    }

    unsigned int width = 800;
    unsigned int height = 600;
    sf::RenderWindow window(sf::VideoMode({ width, height }), "TOWER DEFENCE");

    sf::Font font;
    if (!font.openFromFile("C:\\Aayush\\source\\repos\\Towerdefence\\Towerdefense\\arial.ttf")) {
        std::cout << "Failed to load font\n";
        return -1;
    }

    AppState state = AppState::Login;
    std::string username = "";
    std::string password = "";
    unsigned int Highest_score = 0;
    bool typingUsername = true;

    // Background
    sf::RectangleShape background(sf::Vector2f((float)width, (float)height));
    background.setFillColor(sf::Color(10, 25, 50));

    // Welcome Text
    sf::Text welcomeText(font, "WELCOME", 36);
    welcomeText.setFillColor(sf::Color::Red);
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setPosition(sf::Vector2f(300.f, 100.f));

    // Login Panel
    sf::RectangleShape panel(sf::Vector2f(400.f, 300.f));
    panel.setPosition(sf::Vector2f(200.f, 150.f));
    panel.setFillColor(sf::Color(240, 240, 240));
    panel.setOutlineColor(sf::Color(255, 150, 0));
    panel.setOutlineThickness(5.f);

    // Username UI
    sf::RectangleShape usernameBox(sf::Vector2f(255.f, 45.f));
    usernameBox.setPosition(sf::Vector2f(270.f, 180.f));
    usernameBox.setFillColor(sf::Color(220, 220, 220));
    usernameBox.setOutlineColor(sf::Color(180, 180, 180));
    usernameBox.setOutlineThickness(2.f);

    sf::Text usernameLabel(font, "Username", 18);
    usernameLabel.setFillColor(sf::Color::Black);
    usernameLabel.setPosition(sf::Vector2f(280.f, 185.f));

    sf::Text usernameText(font, "", 18);
    usernameText.setFillColor(sf::Color::Black);
    usernameText.setPosition(sf::Vector2f(280.f, 205.f));

    // Password UI
    sf::RectangleShape passwordBox(sf::Vector2f(255.f, 45.f));
    passwordBox.setPosition(sf::Vector2f(270.f, 240.f));
    passwordBox.setFillColor(sf::Color(220, 220, 220));
    passwordBox.setOutlineColor(sf::Color(180, 180, 180));
    passwordBox.setOutlineThickness(2.f);

    sf::Text passwordLabel(font, "Password", 18);
    passwordLabel.setFillColor(sf::Color::Black);
    passwordLabel.setPosition(sf::Vector2f(280.f, 245.f));

    sf::Text passwordText(font, "", 18);
    passwordText.setFillColor(sf::Color::Black);
    passwordText.setPosition(sf::Vector2f(280.f, 265.f));

    // Login & Signup Buttons
    sf::RectangleShape loginButton(sf::Vector2f(100.f, 40.f));
    loginButton.setPosition(sf::Vector2f(400.f, 340.f));
    loginButton.setFillColor(sf::Color(70, 200, 70));

    sf::Text loginText(font, "LOGIN", 18);
    loginText.setFillColor(sf::Color::White);
    loginText.setPosition(sf::Vector2f(425.f, 350.f));

    sf::RectangleShape signupButton(sf::Vector2f(100.f, 40.f));
    signupButton.setPosition(sf::Vector2f(275.f, 340.f));
    signupButton.setFillColor(sf::Color(70, 150, 250));

    sf::Text signupText(font, "SIGN UP", 18);
    signupText.setFillColor(sf::Color::White);
    signupText.setPosition(sf::Vector2f(295.f, 350.f));

    sf::Text errorText(font, "", 16);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(sf::Vector2f(280.f, 390.f));

    // Player UI Elements
    sf::Text playerNameText(font, "", 24);
    playerNameText.setFillColor(sf::Color::White);
    playerNameText.setStyle(sf::Text::Bold);
    playerNameText.setPosition(sf::Vector2f(20.f, 20.f));

    sf::Text highScoreText(font, "", 24);
    highScoreText.setFillColor(sf::Color::Yellow);
    highScoreText.setStyle(sf::Text::Bold | sf::Text::Italic);
    highScoreText.setPosition(sf::Vector2f(600.f, 20.f));

    sf::RectangleShape playButton(sf::Vector2f(160.f, 50.f));
    playButton.setPosition(sf::Vector2f(320.f, 300.f));
    playButton.setFillColor(sf::Color(255, 100, 0));

    sf::Text playLabel(font, "PLAY GAME", 20);
    playLabel.setFillColor(sf::Color::White);
    playLabel.setPosition(sf::Vector2f(340.f, 310.f));

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (state == AppState::Login) {
                if (auto textEvent = event->getIf<sf::Event::TextEntered>()) {
                    char c = static_cast<char>(textEvent->unicode);
                    if (std::isprint(c)) {
                        if (typingUsername && username.size() < 50)
                            username += c;
                        else if (!typingUsername && password.size() < 50)
                            password += c;
                    }
                    if (textEvent->unicode == 8) {
                        if (typingUsername && !username.empty())
                            username.pop_back();
                        else if (!typingUsername && !password.empty())
                            password.pop_back();
                    }
                }

                if (auto key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Enter)
                        typingUsername = !typingUsername;
                }

                if (event->getIf<sf::Event::MouseButtonPressed>()) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    auto mouseVec = static_cast<sf::Vector2f>(mousePos);

                    if (loginButton.getGlobalBounds().contains(mouseVec)) {
                        std::string query = "SELECT user_id FROM players WHERE username='" + username + "' AND password_hash='" + password + "'";
                        if (mysql_query(conn, query.c_str()) == 0) {
                            MYSQL_RES* res = mysql_store_result(conn);
                            if (MYSQL_ROW row = mysql_fetch_row(res)) {
                                int user_id = std::stoi(row[0]);
                                mysql_free_result(res);

                                std::string scoreQuery = "SELECT MAX(score) FROM scores WHERE user_id=" + std::to_string(user_id);
                                if (mysql_query(conn, scoreQuery.c_str()) == 0) {
                                    MYSQL_RES* scoreRes = mysql_store_result(conn);
                                    MYSQL_ROW scoreRow = mysql_fetch_row(scoreRes);
                                    Highest_score = scoreRow[0] ? std::stoi(scoreRow[0]) : 0;
                                    mysql_free_result(scoreRes);

                                    state = AppState::PlayerUI;
                                    playerNameText.setString("Player: " + username);
                                    highScoreText.setString("High Score: " + std::to_string(Highest_score));
                                    errorText.setString("");
                                }
                                else errorText.setString("Error retrieving score.");
                            }
                            else {
                                errorText.setString("Invalid username or password.");
                                mysql_free_result(res);
                            }
                        }
                        else errorText.setString("Database error!");
                    }

                    if (signupButton.getGlobalBounds().contains(mouseVec)) {
                        std::string checkQuery = "SELECT * FROM players WHERE username='" + username + "'";
                        if (mysql_query(conn, checkQuery.c_str()) == 0) {
                            MYSQL_RES* res = mysql_store_result(conn);
                            if (mysql_num_rows(res) > 0) {
                                errorText.setString("Username already exists.");
                            }
                            else {
                                std::string insertQuery = "INSERT INTO players (username, password_hash) VALUES('" +
                                    username + "', '" + password + "')";
                                if (mysql_query(conn, insertQuery.c_str()) == 0) {
                                    state = AppState::PlayerUI;
                                    playerNameText.setString("Player: " + username);
                                    highScoreText.setString("High Score: 0");
                                    errorText.setString("");
                                }
                                else errorText.setString("Insert error.");
                            }
                            mysql_free_result(res);
                        }
                        else errorText.setString("Database error.");
                    }
                }
            }
            else if (state == AppState::PlayerUI) {
                if (event->getIf<sf::Event::MouseButtonPressed>()) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    if (playButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        std::cout << "Game Started!\n";
                        window.close(); // Launch your game scene here
                    }
                }
            }
        }

        window.clear();
        window.draw(background);

        if (state == AppState::Login) {
            window.draw(welcomeText); // <-- NEW LINE
            window.draw(panel);
            window.draw(usernameBox);
            window.draw(usernameLabel);
            usernameText.setString(username);
            window.draw(usernameText);

            window.draw(passwordBox);
            window.draw(passwordLabel);
            passwordText.setString(std::string(password.length(), '*'));
            window.draw(passwordText);

            window.draw(signupButton);
            window.draw(signupText);

            window.draw(loginButton);
            window.draw(loginText);

            window.draw(errorText);
        }
        else if (state == AppState::PlayerUI) {
            window.draw(playerNameText);
            window.draw(highScoreText);
            window.draw(playButton);
            window.draw(playLabel);
        }

        window.display();
    }

    mysql_close(conn);
    return 0;
}
