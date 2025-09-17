#ifndef TILEMAP_H
#define TILEMAP_H

#include "Tile.h"
#include <QSize>
#include <cmath>
#include <shared_mutex>
#include <unordered_map>
#include <utility>

struct TileIDHash {
    std::size_t operator()(const std::pair<int, int> &id) const noexcept {
        return std::hash<int>()(id.first) ^ (std::hash<int>()(id.second) << 1);
    }
};

struct TileMap {
    TileMap() {}

    Tile &getTile(int tileX, int tileY) {
        std::lock_guard<std::mutex> lock(mtx); // exclusive lock
        std::pair<int, int> id(tileX, tileY);

        auto it = tiles.find(id);
        if (it == tiles.end()) {
            auto [insertIt, _] = tiles.emplace(id, Tile(tileSize, tileResolution));
            return insertIt->second;
        }
        return it->second;
    }

    QQuick3DTextureData *createTileTexture(const Tile &tile,
                                           QQuick3DObject *parent) {
        QQuick3DTextureData *texData = new QQuick3DTextureData(parent);

        texData->setSize(QSize(tile.resolution, tile.resolution));
        texData->setFormat(QQuick3DTextureData::R8); // single byte per pixel

        // Upload raw pixel data
        QByteArray buffer(
            reinterpret_cast<const char *>(tile.sprayedPixels->data()),
            tile.sprayedPixels->size());
        texData->setTextureData(buffer);

        return texData;
    }

    QQuick3DTextureData *getTileTexture(int tileX, int tileY,
                                        QQuick3DObject *parent) {
        Tile &tile = getTile(tileX, tileY);

        if (!tile.sprayedTexture) {
            tile.sprayedTexture = createTileTexture(tile, parent);
        } else if (tile.dirty) {
            updateTileTexture(tile);
        }

        return tile.sprayedTexture;
    }

    void updateTileTexture(Tile &tile) {
        if (!tile.sprayedTexture)
            return;

        QByteArray buffer(
            reinterpret_cast<const char *>(tile.sprayedPixels->data()),
            tile.sprayedPixels->size());
        tile.sprayedTexture->setTextureData(buffer);

        tile.dirty = false;
    }

    QQuick3DTextureData *createTileFieldTexture(Tile &tile,
                                                QQuick3DObject *parent,
                                                float resolutionScale) {
        if (!tile.fieldPixels)
            return nullptr;

        QQuick3DTextureData *texData = new QQuick3DTextureData(parent);
        int res = tile.resolution;

        int scaledRes = res * resolutionScale;
        texData->setSize(QSize(scaledRes, scaledRes));
        texData->setFormat(QQuick3DTextureData::R8);

        QByteArray buffer(scaledRes * scaledRes, 0);
        uint8_t *ptr = reinterpret_cast<uint8_t *>(buffer.data());

        const std::vector<uint8_t> &field = *tile.fieldPixels;

        for (int y = 0; y < scaledRes; ++y) {
            for (int x = 0; x < scaledRes; ++x) {
                // map scaled pixel to original resolution
                int origX = (x * res) / scaledRes;
                int origY = (y * res) / scaledRes;
                int origIndex = origY * res + origX;

                int byteIndex = origIndex >> 3;
                int bitIndex  = origIndex & 7;
                ptr[y * scaledRes + x] = ((field[byteIndex] >> bitIndex) & 1) ? 255 : 0;
            }
        }

        texData->setTextureData(buffer);
        tile.fieldResolutionScale = resolutionScale;

        return texData;
    }

    QQuick3DTextureData *getTileFieldTexture(int tileX, int tileY,
                                             QQuick3DObject *parent, float resolutionScale) {
        Tile &tile = getTile(tileX, tileY);

        if (!tile.fieldTexture || tile.fieldResolutionScale != resolutionScale) {
            tile.fieldTexture = createTileFieldTexture(tile, parent, resolutionScale);
        }
        return tile.fieldTexture;
    }

    void serialize(QDataStream &out) const {
        out << static_cast<quint32>(tiles.size());

        for (auto &[coord, tile] : tiles) {
            out << coord.first << coord.second;
            out.writeRawData(
                reinterpret_cast<const char *>(tile.sprayedPixels->data()),
                tile.sprayedPixels->size());
            out.writeRawData(reinterpret_cast<const char *>(tile.fieldPixels->data()),
                             tile.fieldPixels->size());
            out.writeRawData(
                reinterpret_cast<const char *>(tile.plantedPixels->data()),
                tile.plantedPixels->size());
        }

        // TileMap-level metadata
        out << tileSize;
        out << tileResolution;
    }

    void deserialize(QDataStream &in) {
        quint32 count;
        in >> count;

        tiles.clear();

        for (quint32 i = 0; i < count; ++i) {
            int x, y;
            in >> x >> y;
            Tile tile(tileSize, tileResolution);
            in.readRawData(reinterpret_cast<char *>(tile.sprayedPixels->data()),
                           tile.sprayedPixels->size());
            in.readRawData(reinterpret_cast<char *>(tile.fieldPixels->data()),
                           tile.fieldPixels->size());
            in.readRawData(reinterpret_cast<char *>(tile.plantedPixels->data()),
                           tile.plantedPixels->size());
            tiles[{x, y}] = std::move(tile);
        }

        in >> tileSize;
        in >> tileResolution;
    }

    std::unordered_map<std::pair<int, int>, Tile, TileIDHash> tiles;
    mutable std::mutex mtx;
    float tileSize = 50;       // tileSize in meters
    int tileResolution = 5000; // how many pixels are there in each row
};

#endif // TILEMAP_H
