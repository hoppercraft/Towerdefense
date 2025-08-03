#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <mysql.h>
#include <Windows.h>
#include <chrono>
#include "EnvLoader.h"
#include "login.h"
#include<regex>

auto env = loadEnv("Security.env");

const char* HOST = env["HOST"].c_str();
const char* USER = env["USER"].c_str();
const char* PASS = env["PASS"].c_str();
const char* DB = env["DB"].c_str();

// Global variables that Game.cpp will use
MYSQL* globalConnection = nullptr;
std::string loggedInUsername = "";
bool isNewGame = true; // Flag to indicate if starting a new game or continuing

enum class AppState {
    Login,
    PlayerUI
};

bool isValidPassword(const std::string& password)
{
    std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d]).{8,}$");
    return std::regex_match(password, pattern);
}

bool hasSavedGame(MYSQL* conn, const std::string& username) {
    std::string query = "SELECT COUNT(*) FROM game_saves WHERE username='" + username + "'";
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (MYSQL_ROW row = mysql_fetch_row(res)) {
            int count = std::stoi(row[0]);
            mysql_free_result(res);
            return count > 0;
        }
        mysql_free_result(res);
    }
    return false;
}

bool login()
{
    bool loginSuccessful = false;
    MYSQL* conn;
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, HOST, USER, PASS, DB, 4134, NULL, 0)) {
        std::cout << "MySQL Connection Error: " << mysql_error(conn) << std::endl;
        return false;
    }

    unsigned int width = 800;
    unsigned int height = 600;
    sf::RenderWindow window(sf::VideoMode({ width, height }), "TOWER DEFENCE");

    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cout << "Failed to load font\n";
        return false;
    }

    AppState state = AppState::Login;
    std::string username = "";
    std::string password = "";
    unsigned int Highest_score = 0;
    bool typingUsername = true;
    bool playerHasSavedGame = false;

    // Timer for blinking caret
    auto lastBlink = std::chrono::steady_clock::now();
    bool showCaret = true;

    // Timer for error message display
    std::chrono::steady_clock::time_point errorStartTime;
    bool errorVisible = false;
    bool errorVisible1 = false;

    // Background
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("background.png"))
        std::cout << "Failed to load background image\n";
    sf::Sprite bgSprite(bgTexture);

    // Welcome Text
    sf::Text welcomeText(font, "WELCOME", 36);
    welcomeText.setFillColor(sf::Color::Red);
    welcomeText.setStyle(sf::Text::Bold);
    welcomeText.setPosition(sf::Vector2f(300.f, 100.f));

    // Login Panel
    sf::RectangleShape panel(sf::Vector2f(400.f, 300.f));
    panel.setPosition(sf::Vector2f(200.f, 150.f));
    panel.setFillColor(sf::Color(25, 25, 112));
    panel.setOutlineColor(sf::Color(0, 0, 0));
    panel.setOutlineThickness(5.f);

    // Username UI
    sf::RectangleShape usernameBox(sf::Vector2f(255.f, 45.f));
    usernameBox.setPosition(sf::Vector2f(270.f, 180.f));
    usernameBox.setFillColor(sf::Color(220, 220, 220));
    usernameBox.setOutlineColor(sf::Color(180, 180, 180));
    usernameBox.setOutlineThickness(2.f);

    sf::Text usernameText(font, "", 18);
    usernameText.setFillColor(sf::Color::Black);
    usernameText.setPosition(sf::Vector2f(310.f, 193.f));

    // Password UI
    sf::RectangleShape passwordBox(sf::Vector2f(255.f, 45.f));
    passwordBox.setPosition(sf::Vector2f(270.f, 240.f));
    passwordBox.setFillColor(sf::Color(220, 220, 220));
    passwordBox.setOutlineColor(sf::Color(180, 180, 180));
    passwordBox.setOutlineThickness(2.f);

    sf::Text passwordText(font, "", 18);
    passwordText.setFillColor(sf::Color::Black);
    passwordText.setPosition(sf::Vector2f(310.f, 250.f));

    // Login & Signup Buttons
    sf::RectangleShape loginButton(sf::Vector2f(100.f, 40.f));
    loginButton.setPosition(sf::Vector2f(400.f, 340.f));
    loginButton.setFillColor(sf::Color(200, 255, 200));

    sf::Text loginText(font, "LOGIN", 18);
    loginText.setFillColor(sf::Color::Black);
    loginText.setPosition(sf::Vector2f(425.f, 350.f));

    sf::RectangleShape signupButton(sf::Vector2f(100.f, 40.f));
    signupButton.setPosition(sf::Vector2f(275.f, 340.f));
    signupButton.setFillColor(sf::Color(200, 255, 200));

    sf::Text signupText(font, "SIGN UP", 18);
    signupText.setFillColor(sf::Color::Black);
    signupText.setPosition(sf::Vector2f(295.f, 350.f));

    sf::Text errorText(font, "", 16);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(sf::Vector2f(280.f, 390.f));

    sf::Text errorText1(font, "", 16);
    errorText1.setFillColor(sf::Color::Red);
    errorText1.setPosition(sf::Vector2f(203.f, 390.f));

    // Player UI Elements
    sf::Text playerNameText(font, "", 24);
    playerNameText.setFillColor(sf::Color::White);
    playerNameText.setStyle(sf::Text::Bold);
    playerNameText.setPosition(sf::Vector2f(20.f, 20.f));

    sf::Text highScoreText(font, "", 24);
    highScoreText.setFillColor(sf::Color::Yellow);
    highScoreText.setStyle(sf::Text::Bold | sf::Text::Italic);
    highScoreText.setPosition(sf::Vector2f(600.f, 20.f));

    // New Game Button
    sf::RectangleShape NewButton(sf::Vector2f(160.f, 50.f));
    NewButton.setPosition(sf::Vector2f(250.f, 300.f));
    NewButton.setFillColor(sf::Color(200, 255, 200));

    sf::Text NewLabel(font, "NEW GAME", 20);
    NewLabel.setFillColor(sf::Color::Black);
    NewLabel.setPosition(sf::Vector2f(270.f, 310.f));

    // Continue Button
    sf::RectangleShape ContinueButton(sf::Vector2f(160.f, 50.f));
    ContinueButton.setPosition(sf::Vector2f(420.f, 300.f));
    ContinueButton.setFillColor(sf::Color(200, 255, 200));

    sf::Text ContinueLabel(font, "CONTINUE", 20);
    ContinueLabel.setFillColor(sf::Color::Black);
    ContinueLabel.setPosition(sf::Vector2f(440.f, 310.f));

    // Placeholder texts
    sf::Text usernamePlaceholder(font, "Username", 18);
    usernamePlaceholder.setFillColor(sf::Color(150, 150, 150));
    usernamePlaceholder.setPosition(usernameText.getPosition());

    sf::Text passwordPlaceholder(font, "Password", 18);
    passwordPlaceholder.setFillColor(sf::Color(150, 150, 150));
    passwordPlaceholder.setPosition(passwordText.getPosition());

    sf::Texture userIconTex;
    if (!userIconTex.loadFromFile("user.png"))
        return false;
    sf::Sprite userIcon(userIconTex);
    userIcon.setScale(sf::Vector2f(0.05f, 0.05f));
    userIcon.setPosition(sf::Vector2f(280.f, 191.f));

    sf::Texture PassIconTex;
    if (!PassIconTex.loadFromFile("password.png"))
        return false;
    sf::Sprite PassIcon(PassIconTex);
    PassIcon.setScale(sf::Vector2f(0.05f, 0.05f));
    PassIcon.setPosition(sf::Vector2f(280.f, 248.f));

    // Colors for hover and pressed states
    sf::Color loginNormalColor = sf::Color(200, 255, 200);
    sf::Color loginHoverColor = sf::Color(150, 230, 150);
    sf::Color loginPressedColor = sf::Color(100, 180, 100);

    sf::Color signupNormalColor = sf::Color(200, 255, 200);
    sf::Color signupHoverColor = sf::Color(150, 230, 150);
    sf::Color signupPressedColor = sf::Color(100, 180, 100);

    sf::Color NewNormalColor = sf::Color(200, 255, 200);
    sf::Color NewHoverColor = sf::Color(150, 230, 150);
    sf::Color NewPressedColor = sf::Color(100, 180, 100);

    sf::Color ContinueNormalColor = sf::Color(200, 255, 200);
    sf::Color ContinueHoverColor = sf::Color(150, 230, 150);
    sf::Color ContinuePressedColor = sf::Color(100, 180, 100);

    // Color for disabled continue button
    sf::Color ContinueDisabledColor = sf::Color(150, 150, 150);

    bool loginPressed = false;
    bool signupPressed = false;
    bool NewPressed = false;
    bool ContinuePressed = false;

    // Game loop
    while (window.isOpen()) {
        // Manage caret blinking
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastBlink).count() > 500) {
            showCaret = !showCaret;
            lastBlink = now;
        }

        // Poll events
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (auto resizeEvent = event->getIf<sf::Event::Resized>()) {
                sf::Vector2u newSize = window.getSize();
                sf::FloatRect visibleArea(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(width, height));
                window.setView(sf::View(visibleArea));
            }
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
                    else if (textEvent->unicode == 8) { // Backspace
                        if (typingUsername && !username.empty())
                            username.pop_back();
                        else if (!typingUsername && !password.empty())
                            password.pop_back();
                    }
                }
                if (auto keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                    if (keyEvent->code == sf::Keyboard::Key::Tab) {
                        typingUsername = !typingUsername;
                    }
                    else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                        if (!username.empty() && !password.empty()) {
                            loginPressed = true;
                        }
                        else {
                            typingUsername = !typingUsername;
                        }
                    }
                }

                if (event->getIf<sf::Event::MouseMoved>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mouseVec = window.mapPixelToCoords(mousePos);

                    // Login Button Hover
                    if (loginButton.getGlobalBounds().contains(mouseVec)) {
                        if (!loginPressed)
                            loginButton.setFillColor(loginHoverColor);
                    }
                    else if (!loginPressed) {
                        loginButton.setFillColor(loginNormalColor);
                    }

                    // Signup Button Hover
                    if (signupButton.getGlobalBounds().contains(mouseVec)) {
                        if (!signupPressed)
                            signupButton.setFillColor(signupHoverColor);
                    }
                    else if (!signupPressed) {
                        signupButton.setFillColor(signupNormalColor);
                    }
                }

                if (event->getIf<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mouseVec = window.mapPixelToCoords(mousePos);

                    // Login Button Press
                    if (loginButton.getGlobalBounds().contains(mouseVec)) {
                        loginPressed = true;
                        loginButton.setFillColor(loginPressedColor);
                    }

                    // Signup Button Press
                    if (signupButton.getGlobalBounds().contains(mouseVec)) {
                        signupPressed = true;
                        signupButton.setFillColor(signupPressedColor);
                    }
                }

                if (event->getIf<sf::Event::MouseButtonReleased>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mouseVec = window.mapPixelToCoords(mousePos);

                    // Login Button Release
                    if (loginPressed) {
                        loginPressed = false;
                        loginButton.setFillColor(loginNormalColor);

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

                                        // Check if player has saved game
                                        playerHasSavedGame = hasSavedGame(conn, username);

                                        // SET GLOBAL VARIABLES HERE
                                        globalConnection = conn;
                                        loggedInUsername = username;

                                        state = AppState::PlayerUI;
                                        playerNameText.setString("Player: " + username);
                                        highScoreText.setString("High Score: " + std::to_string(Highest_score));
                                        errorText.setString("");
                                        errorVisible = false;
                                    }
                                    else {
                                        errorText.setString("Error retrieving score.");
                                        errorStartTime = std::chrono::steady_clock::now();
                                        errorVisible = true;
                                    }
                                }
                                else {
                                    errorText.setString("Invalid username or password.");
                                    mysql_free_result(res);
                                    errorStartTime = std::chrono::steady_clock::now();
                                    errorVisible = true;
                                }
                            }
                            else {
                                errorText.setString("Database error!");
                                errorStartTime = std::chrono::steady_clock::now();
                                errorVisible = true;
                            }
                        }
                    }

                    // Signup Button Release
                    if (signupPressed) {
                        signupPressed = false;
                        signupButton.setFillColor(signupNormalColor);

                        if (!isValidPassword(password)) {
                            errorVisible1 = true;
                            errorStartTime = std::chrono::steady_clock::now();
                            errorText1.setString("Password must be 8 + chars, upper, lower, digit, special.");
                        }
                        else {
                            if (signupButton.getGlobalBounds().contains(mouseVec)) {
                                std::string checkQuery = "SELECT * FROM players WHERE username='" + username + "'";
                                if (mysql_query(conn, checkQuery.c_str()) == 0) {
                                    MYSQL_RES* res = mysql_store_result(conn);
                                    if (mysql_num_rows(res) > 0) {
                                        errorText.setString("Username already exists.");
                                        errorStartTime = std::chrono::steady_clock::now();
                                        errorVisible = true;
                                    }
                                    else {
                                        std::string insertQuery = "INSERT INTO players (username, password_hash) VALUES('" +
                                            username + "', '" + password + "')";
                                        if (mysql_query(conn, insertQuery.c_str()) == 0) {
                                            // New users don't have saved games
                                            playerHasSavedGame = false;

                                            // SET GLOBAL VARIABLES HERE TOO
                                            globalConnection = conn;
                                            loggedInUsername = username;

                                            state = AppState::PlayerUI;
                                            playerNameText.setString("Player: " + username);

                                            highScoreText.setString("High Score: " );
                                            errorText.setString("");
                                            errorVisible = false;
                                        }
                                        else {
                                            errorText.setString("Insert error.");
                                            errorStartTime = std::chrono::steady_clock::now();
                                            errorVisible = true;
                                        }
                                    }
                                    mysql_free_result(res);
                                }
                                else {
                                    errorText.setString("Database error.");
                                    errorStartTime = std::chrono::steady_clock::now();
                                    errorVisible = true;
                                }
                            }
                        }
                    }
                }
            }
            else if (state == AppState::PlayerUI) {
                if (event->getIf<sf::Event::MouseMoved>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mouseVec = window.mapPixelToCoords(mousePos);

                    // New Game Button Hover
                    if (NewButton.getGlobalBounds().contains(mouseVec)) {
                        if (!NewPressed)
                            NewButton.setFillColor(NewHoverColor);
                    }
                    else if (!NewPressed) {
                        NewButton.setFillColor(NewNormalColor);
                    }

                    // Continue Button Hover (only if player has saved game)
                    if (hasSavedGame && ContinueButton.getGlobalBounds().contains(mouseVec)) {
                        if (!ContinuePressed)
                            ContinueButton.setFillColor(ContinueHoverColor);
                    }
                    else if (!ContinuePressed && playerHasSavedGame) {
                        ContinueButton.setFillColor(ContinueNormalColor);
                    }
                }

                if (event->getIf<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mouseVec = window.mapPixelToCoords(mousePos);

                    // New Game Button Press
                    if (NewButton.getGlobalBounds().contains(mouseVec)) {
                        NewPressed = true;
                        NewButton.setFillColor(NewPressedColor);
                    }

                    // Continue Button Press (only if player has saved game)
                    if (hasSavedGame && ContinueButton.getGlobalBounds().contains(mouseVec)) {
                        ContinuePressed = true;
                        ContinueButton.setFillColor(ContinuePressedColor);
                    }
                }

                if (event->getIf<sf::Event::MouseButtonReleased>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mouseVec = window.mapPixelToCoords(mousePos);

                    // New Game Button Release
                    if (NewPressed) {
                        NewPressed = false;
                        NewButton.setFillColor(NewNormalColor);
                        if (NewButton.getGlobalBounds().contains(mouseVec)) {
                            std::cout << "New Game Started!\n";
                            isNewGame = true; // Set flag for new game
                            loginSuccessful = true;
                            window.close();
                        }
                    }

                    // Continue Button Release
                    if (ContinuePressed) {
                        ContinuePressed = false;
                        ContinueButton.setFillColor(ContinueNormalColor);
                        if (ContinueButton.getGlobalBounds().contains(mouseVec)) {
                            std::cout << "Continuing Previous Game!\n";
                            isNewGame = false; // Set flag for continuing game
                            loginSuccessful = true;
                            window.close();
                        }
                    }
                }
            }
        }

        // Clear window and draw everything
        window.clear();
        window.draw(bgSprite);

        if (state == AppState::Login) {
            // Highlight input boxes when typing
            if (typingUsername) {
                usernameBox.setOutlineColor(sf::Color::Yellow);
                passwordBox.setOutlineColor(sf::Color(180, 180, 180));
            }
            else {
                usernameBox.setOutlineColor(sf::Color(180, 180, 180));
                passwordBox.setOutlineColor(sf::Color::Yellow);
            }

            // Draw UI
            window.draw(welcomeText);
            window.draw(panel);
            window.draw(usernameBox);

            // Show username text with caret if typing
            std::string usernameDisplay = username;
            if (typingUsername && showCaret) usernameDisplay += '|';
            usernameText.setString(usernameDisplay);

            if (username.empty()) {
                window.draw(usernamePlaceholder);
            }
            else {
                window.draw(usernameText);
            }

            window.draw(passwordBox);
            window.draw(userIcon);
            window.draw(PassIcon);

            // Show password masked and caret if typing
            std::string passwordDisplay(password.size(), '*');
            if (!typingUsername && showCaret) passwordDisplay += '|';
            passwordText.setString(passwordDisplay);
            if (password.empty()) {
                window.draw(passwordPlaceholder);
            }
            else {
                window.draw(passwordText);
            }

            window.draw(signupButton);
            window.draw(signupText);

            window.draw(loginButton);
            window.draw(loginText);

            if (errorVisible1) {
                auto elapsed = std::chrono::steady_clock::now() - errorStartTime;
                if (std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() > 3) {
                    errorVisible1 = false;
                    errorText1.setString("");
                }
                window.draw(errorText1);
            }

            if (errorVisible) {
                auto elapsed = std::chrono::steady_clock::now() - errorStartTime;
                if (std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() > 3) {
                    errorVisible = false;
                    errorText.setString("");
                }
                else {
                    window.draw(errorText);
                }
            }
        }
        else if (state == AppState::PlayerUI) {
            window.draw(playerNameText);
            window.draw(highScoreText);

            // Always draw New Game button
            window.draw(NewButton);
            window.draw(NewLabel);

            // Draw Continue button - disabled if no saved game
            if (playerHasSavedGame) {
                window.draw(ContinueButton);
                window.draw(ContinueLabel);
            }
            else {
                // Draw disabled continue button
                sf::RectangleShape disabledContinueButton = ContinueButton;
                disabledContinueButton.setFillColor(ContinueDisabledColor);
                window.draw(disabledContinueButton);

                sf::Text disabledContinueLabel = ContinueLabel;
                disabledContinueLabel.setFillColor(sf::Color(100, 100, 100));
                window.draw(disabledContinueLabel);
            }
        }

        window.display();
    }

    // Only close connection if login was not successful
    // If successful, Game.cpp will use the connection
    if (!loginSuccessful) {
        mysql_close(conn);
        globalConnection = nullptr;
        loggedInUsername = "";
    }

    return loginSuccessful;
}