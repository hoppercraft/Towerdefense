#pragma once
#include <SFML/System/Vector2.hpp>

namespace Game {
    // Map dimensions
    constexpr int TILE_SIZE = 20;
    constexpr int MAP_WIDTH = 13;
    constexpr int MAP_HEIGHT = 8;

    // Tile types
    enum TileType { Grass = 0, Water = 1, Path = 2, Tree = 3 };

    // Game map declaration
    extern int Map1[MAP_HEIGHT][MAP_WIDTH];
}