#ifndef TILE_H
#define TILE_H

#include <cstdint>
#include <vector>

struct Tile{
    Tile(int width = 512, int height = 512)
            : pixels(width * height, 0) {}

    std::vector<uint8_t> pixels;
    bool dirty;
};

#endif // TILE_H
