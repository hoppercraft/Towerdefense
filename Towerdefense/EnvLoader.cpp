#include "EnvLoader.h"
#include <fstream>
#include <iostream>

std::unordered_map<std::string, std::string> loadEnv(const std::string& Security) {
    std::unordered_map<std::string, std::string> env;
    std::ifstream file(Security);
    if (!file.is_open()) {
        std::cerr << "Failed to open .env file\n";
        return env;
    }

    std::string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t equals = line.find('=');
        if (equals != std::string::npos) {
            std::string key = line.substr(0, equals);
            std::string value = line.substr(equals + 1);
            env[key] = value;
        }
    }

    file.close();
    return env;
}
