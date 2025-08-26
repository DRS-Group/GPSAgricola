#ifndef TILE_H
#define TILE_H

#include <QQuick3DTextureData>

struct Tile {
    Tile(float size = 512, int resolution = 512)
        : size(size),
          resolution(resolution),
          fieldPixels(((resolution * resolution) + 7) / 8, 0), // bit-packed
          dirty(false) {}

    float size;
    int resolution;

    // --- Always present ---
    std::vector<uint8_t> fieldPixels;  // 1 bit per pixel (inside field = 1, outside = 0)
    QQuick3DTextureData* fieldTexture = nullptr;

    // --- Optional job-specific layers ---
    std::unique_ptr<std::vector<uint8_t>> sprayedPixels; // 1 byte per pixel
    QQuick3DTextureData* sprayedTexture = nullptr;

    std::unique_ptr<std::vector<uint8_t>> plantedPixels; // 0 or 1 per pixel
    QQuick3DTextureData* plantedTexture = nullptr;

    bool dirty;

    // --- Lazy allocators ---
    void ensureSprayedPixels() {
        if (!sprayedPixels) {
            sprayedPixels = std::make_unique<std::vector<uint8_t>>(resolution * resolution, 0);
        }
    }

    void ensurePlantedPixels() {
        if (!plantedPixels) {
            plantedPixels = std::make_unique<std::vector<uint8_t>>(resolution * resolution, 0);
        }
    }
};

// --- Bit access helpers for field mask ---
inline bool getFieldPixel(const Tile& tile, int x, int y) {
    int index = y * tile.resolution + x;
    return (tile.fieldPixels[index >> 3] >> (index & 7)) & 1;
}

inline void setFieldPixel(Tile& tile, int x, int y, bool value) {
    int index = y * tile.resolution + x;
    if (value)
        tile.fieldPixels[index >> 3] |= (1 << (index & 7));
    else
        tile.fieldPixels[index >> 3] &= ~(1 << (index & 7));
}

#endif // TILE_H
