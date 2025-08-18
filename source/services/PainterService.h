#ifndef PAINTERSERVICE_H
#define PAINTERSERVICE_H

#include "source/BaseService.h"
#include "../BaseService.h"
#include "source/types/Tile.h"
#include <QGeoCoordinate>
#include <unordered_map>
#include <utility>
#include <QImage>
#include <QColor>

#include <QQuick3DTextureData>

class PainterService : public BaseService
{
    Q_OBJECT
public:
    PainterService(QObject *parent = nullptr);

    Tile& getTile(int tileX, int tileY) {
        TileID id(tileX, tileY);
        auto it = tiles.find(id);
        if (it == tiles.end()) {
            auto [insertIt, _] = tiles.emplace(id, Tile(tileWidth, tileHeight));
            return insertIt->second;
        }
        return it->second;
    }

    Tile* getTileIfExists(int tileX, int tileY) {
        TileID id(tileX, tileY);
        auto it = tiles.find(id);
        if (it != tiles.end())
            return &it->second;
        return nullptr;
    }

    void markSprayed(int tileX, int tileY, int pixelX, int pixelY) {
        Tile& tile = getTile(tileX, tileY);
        int idx = pixelY * tileWidth + pixelX;
        if (idx >= 0 && idx < (int)tile.pixels.size()) {
            tile.pixels[idx] = 1;
            tile.dirty = true;
        }
    }

    void markSprayed(int x, int y) {
        // Calculate tile coordinates
        int tileX = x / tileWidth;
        int tileY = y / tileHeight;

        // Calculate pixel coordinates inside the tile
        int pixelX = x % tileWidth;
        int pixelY = y % tileHeight;

        // Call the existing markSprayed
        markSprayed(tileX, tileY, pixelX, pixelY);
    }

    QQuick3DTextureData* createTileTexture(const Tile &tile, QQuick3DObject* parent) {

        // Create a QQuick3DTextureData object
        QQuick3DTextureData* texData = new QQuick3DTextureData(parent);

        // Set size
        texData->setSize(QSize(tileWidth, tileHeight));

        // Each pixel is RGBA, 4 bytes
        texData->setFormat(QQuick3DTextureData::RGBA8);

        // Allocate buffer
        QByteArray buffer(tileWidth * tileHeight * 4, 0); // 4 bytes per pixel

        // Fill buffer from tile.pixels
        for (int y = 0; y < tileHeight; ++y) {
            for (int x = 0; x < tileWidth; ++x) {
                uint8_t val = tile.pixels[y * tileWidth + x];
                int idx = (y * tileWidth + x) * 4;
                if (val == 1) {
                    buffer[idx + 0] = 0;   // R
                    buffer[idx + 1] = -1; // G
                    buffer[idx + 2] = 0;   // B
                    buffer[idx + 3] = -1; // A
                } else {
                    buffer[idx + 0] = 0;
                    buffer[idx + 1] = 0;
                    buffer[idx + 2] = 0;
                    buffer[idx + 3] = 0;   // transparent
                }
            }
        }

        texData->setTextureData(buffer);

        return texData;
    }

   Q_INVOKABLE QQuick3DTextureData* getTileTexture(int tileX, int tileY, QQuick3DObject* parent){
       qDebug() << "getting tile texture";
        Tile* tile = getTileIfExists(tileX, tileY);
        if (!tile) return nullptr;
        return createTileTexture(*tile, parent);
    }


private:
    using TileID = std::pair<int, int>; // tile grid coordinates

    struct TileIDHash {
        std::size_t operator()(const TileID& id) const noexcept {
            return std::hash<int>()(id.first) ^ (std::hash<int>()(id.second) << 1);
        }
    };

    QGeoCoordinate baseCoordinate = QGeoCoordinate(-21.1308, -49.0019);
    double pixelScale = 1; // 1 pixel is 10cm.
    int tileWidth = 1024;
    int tileHeight = 1024;

    std::unordered_map<TileID, Tile, TileIDHash> tiles;
};

#endif // PAINTERSERVICE_H
