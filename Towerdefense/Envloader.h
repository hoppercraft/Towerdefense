#pragma once
#ifndef ENVLOADER_H
#define ENVLOADER_H

#include <unordered_map>
#include <string>

std::unordered_map<std::string, std::string> loadEnv(const std::string& Security);

#endif
