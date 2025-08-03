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

enum class AppState {
    Login,
    PlayerUI
};

bool isValidPassword(const std::string& password)
{
    std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d]).{8,}$");
    return std::regex_match(password, pattern);
}

bool login()
{
    bool loginSuccessful = false;
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
    if (!font.openFromFile("arial.ttf")) {
        std::cout << "Failed to load font\n";
        return -1;
    }

    AppState state = AppState::Login;
    std::string username = "";
    std::string password = "";
    unsigned int Highest_score = 0;
    bool typingUsername = true;

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

    sf::Text errorText1 (font, "", 16);
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


    sf::RectangleShape playButton(sf::Vector2f(160.f, 50.f));
    playButton.setPosition(sf::Vector2f(320.f, 300.f));
    playButton.setFillColor(sf::Color(200, 255, 200));

    sf::Text playLabel(font, "PLAY GAME", 20);
    playLabel.setFillColor(sf::Color::Black);
    playLabel.setPosition(sf::Vector2f(340.f, 310.f));

    // Placeholder texts
    sf::Text usernamePlaceholder(font, "Username", 18);
    usernamePlaceholder.setFillColor(sf::Color(150, 150, 150));  // Light gray color
    usernamePlaceholder.setPosition(usernameText.getPosition());

    sf::Text passwordPlaceholder(font, "Password", 18);
    passwordPlaceholder.setFillColor(sf::Color(150, 150, 150));  // Light gray color
    passwordPlaceholder.setPosition(passwordText.getPosition());

    sf::Texture userIconTex;
    if (!userIconTex.loadFromFile("user.png"))
        return -1;
    sf::Sprite userIcon(userIconTex);
    userIcon.setScale(sf::Vector2f(0.05f, 0.05f));
    userIcon.setPosition(sf::Vector2f(280.f, 191.f));

    sf::Texture PassIconTex;
    if (!PassIconTex.loadFromFile("password.png"))
        return -1;
    sf::Sprite PassIcon(PassIconTex);
    PassIcon.setScale(sf::Vector2f(0.05f, 0.05f));
    PassIcon.setPosition(sf::Vector2f(280.f, 248.f));

    

    // Colors for hover and pressed states (just a bit darker/lighter)
    sf::Color loginNormalColor = sf::Color(200, 255, 200);
    sf::Color loginHoverColor = sf::Color(150, 230, 150);
    sf::Color loginPressedColor = sf::Color(100, 180, 100);

    sf::Color signupNormalColor = sf::Color(200, 255, 200);
    sf::Color signupHoverColor = sf::Color(150, 230, 150);
    sf::Color signupPressedColor = sf::Color(100, 180, 100);

    sf::Color playNormalColor = sf::Color(200, 255, 200);
    sf::Color playHoverColor = sf::Color(150, 230, 150);
    sf::Color playPressedColor = sf::Color(100, 180, 100);

    bool loginPressed = false;
    bool signupPressed = false;
    bool playPressed = false;

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
                sf::FloatRect visibleArea(sf::Vector2f(0.0f,0.0f),sf::Vector2f(width,height));
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
                        typingUsername = !typingUsername;  // Switch input field on Tab
                    }
                    else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                        if (!username.empty() && !password.empty()) {
                            // Trigger login button click simulation
                            loginPressed = true;
                        }
                        else {
                            typingUsername = !typingUsername;  // Switch input on Enter if empty
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

                    // Play Button Hover (only in PlayerUI)
                    if (state == AppState::PlayerUI) {
                        if (playButton.getGlobalBounds().contains(mouseVec)) {
                            if (!playPressed)
                                playButton.setFillColor(playHoverColor);
                        }
                        else if (!playPressed) {
                            playButton.setFillColor(playNormalColor);
                        }
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

                    // Play Button Press
                    if (state == AppState::PlayerUI && playButton.getGlobalBounds().contains(mouseVec)) {
                        playPressed = true;
                        playButton.setFillColor(playPressedColor);
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
                                            state = AppState::PlayerUI;
                                            playerNameText.setString("Player: " + username);
                                            highScoreText.setString("High Score: 0");
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

                    // Play Button Release
                    if (playPressed) {
                        playPressed = false;
                        playButton.setFillColor(playNormalColor);

                        if (state == AppState::PlayerUI && playButton.getGlobalBounds().contains(mouseVec)) {
                            std::cout << "Game Started!\n";
                            loginSuccessful = true;
                            window.close();
                        }
                    }
                }
            }
            else if (state == AppState::PlayerUI) {
                if (event->getIf<sf::Event::MouseMoved>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mouseVec = window.mapPixelToCoords(mousePos);

                    if (playButton.getGlobalBounds().contains(mouseVec)) {
                        if (!playPressed)
                            playButton.setFillColor(playHoverColor);
                    }
                    else if (!playPressed) {
                        playButton.setFillColor(playNormalColor);
                    }
                }

                if (event->getIf<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mouseVec = window.mapPixelToCoords(mousePos);

                    if (playButton.getGlobalBounds().contains(mouseVec)) {
                        playPressed = true;
                        playButton.setFillColor(playPressedColor);
                    }
                }

                if (event->getIf<sf::Event::MouseButtonReleased>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f mouseVec = window.mapPixelToCoords(mousePos);
                    if (playPressed) {
                        playPressed = false;
                        playButton.setFillColor(playNormalColor);
                        if (playButton.getGlobalBounds().contains(mouseVec)) {
                            std::cout << "Game Started!\n";
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
                /*passwordText.setString(std::string(password.length(), '*'));*/
                window.draw(passwordText);
            }

            window.draw(signupButton);
            window.draw(signupText);
            
            window.draw(loginButton);
            window.draw(loginText);

            if (errorVisible1)
            {
                auto elapsed = std::chrono::steady_clock::now() - errorStartTime;
                if (std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() > 3) {
                    errorVisible1 = false;
                    errorText1.setString("");
                }
                window.draw(errorText1);
            }
            // Show error text for 3 seconds

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
            window.draw(playButton);
            window.draw(playLabel);
        }

        window.display();
    }
    mysql_close(conn);
    return loginSuccessful;
}
