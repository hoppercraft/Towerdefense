#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <SFML/Graphics.hpp>
#include <string>

namespace CS {
    // Runs the login window, returns true if success.
    // username_out will contain logged in username.
    bool runLoginWindow(const sf::Font& font, std::string& username_out);
}

#endif // LOGIN_HPP
