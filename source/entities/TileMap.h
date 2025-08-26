#ifndef TILEMAP_H
#define TILEMAP_H

#include "Tile.h"
#include <QSize>
#include <cmath>
#include <unordered_map>
#include <utility>

struct TileIDHash {
    std::size_t operator()(const std::pair<int, int> &id) const noexcept {
        return std::hash<int>()(id.first) ^ (std::hash<int>()(id.second) << 1);
    }
};

struct TileMap {
  TileMap(float tileSize = 51.2f, int tileResolution = 1024)
        : tileSize(tileSize), tileResolution(tileResolution) {}

  Tile &getTile(int tileX, int tileY) {
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
                                              QQuick3DObject *parent) {
      QQuick3DTextureData *texData = new QQuick3DTextureData(parent);

      texData->setSize(QSize(tile.resolution, tile.resolution));
      texData->setFormat(QQuick3DTextureData::R8);

      // Convert bit-packed fieldPixels into bytes for texture
      QByteArray buffer(tile.resolution * tile.resolution, 0);

      for (int y = 0; y < tile.resolution; ++y) {
          for (int x = 0; x < tile.resolution; ++x) {
              bool isField = getFieldPixel(tile, x, y); // implement bit access
              buffer[y * tile.resolution + x] = isField ? -1 : 0;
          }
      }

      texData->setTextureData(buffer);

      return texData;
  }

  QQuick3DTextureData *getTileFieldTexture(int tileX, int tileY,
                                           QQuick3DObject *parent) {
      Tile &tile = getTile(tileX, tileY);

      if (!tile.fieldTexture) {
          tile.fieldTexture = createTileFieldTexture(tile, parent);
      }
      return tile.fieldTexture;
  }

  std::unordered_map<std::pair<int, int>, Tile, TileIDHash> tiles;
  float tileSize;
  int tileResolution;
};

#endif // TILEMAP_H
