#include "PainterService.h"

#include "source/services/FieldService.h"
#include <QElapsedTimer>
#include <QImage>
#include <QList>
#include <QPainter>
#include <QPair>
#include <QPolygonF>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentRun>
#include <cmath>

PainterService *PainterService::instance = nullptr;

PainterService *PainterService::getInstance() {
    if (!instance) {
        instance = new PainterService();
    }
    return instance;
}

PainterService::PainterService(QObject *parent) : BaseService(parent) {}

void PainterService::drawPolygonGeo(TileMap &tileMap,
                                    const QGeoCoordinate &worldOrigin,
                                    const QList<QGeoCoordinate> &geoCoords,
                                    uint8_t grayValue) {
    if (geoCoords.size() < 3)
        return; // need at least a triangle

    // Polygon in world space (meters, relative to worldOrigin)
    QPolygonF worldPolygon;
    for (const QGeoCoordinate &coord : geoCoords) {
        QPointF pt = geoToWorldPixel(coord, worldOrigin);
        worldPolygon << pt;
    }

    // Bounding box in world space
    QRectF bbox = worldPolygon.boundingRect();
    int minTileX = static_cast<int>(std::floor(bbox.left() / tileMap.tileSize));
    int maxTileX = static_cast<int>(std::floor(bbox.right() / tileMap.tileSize));
    int minTileY = static_cast<int>(std::floor(bbox.top() / tileMap.tileSize));
    int maxTileY = static_cast<int>(std::floor(bbox.bottom() / tileMap.tileSize));

    for (int tx = minTileX; tx <= maxTileX; ++tx) {
        for (int ty = minTileY; ty <= maxTileY; ++ty) {
            Tile &tile = tileMap.getTile(tx, ty);

            // Transform polygon into tile-local coordinates (pixels)
            QPolygonF localPolygon;
            for (const QPointF &wp : worldPolygon) {
                double localX = (wp.x() - tx * tileMap.tileSize) / tileMap.tileSize *
                                tile.resolution;
                double localY = (wp.y() - ty * tileMap.tileSize) / tileMap.tileSize *
                                tile.resolution;
                localPolygon << QPointF(localX, localY);
            }

            // Draw
            tile.ensureSprayedPixels(); // make sure sprayedPixels exists
            QImage img(tile.sprayedPixels->data(), tile.resolution, tile.resolution,
                       tile.resolution, QImage::Format_Grayscale8);

            QPainter painter(&img);
            painter.setRenderHint(QPainter::Antialiasing, false);

            QColor color(grayValue, grayValue, grayValue);
            painter.setPen(QPen(color, 1));
            painter.setBrush(QBrush(color));

            painter.drawPolygon(localPolygon);
            painter.end();

            tile.dirty = true;
        }
    }
}

void PainterService::drawLineGeo(TileMap &tileMap,
                                 const QGeoCoordinate &worldOrigin,
                                 const QGeoCoordinate &coord1,
                                 const QGeoCoordinate &coord2,
                                 uint8_t grayValue) {
    // Convert to world space (meters, relative to worldOrigin)
    QPointF wp1 = geoToWorldPixel(coord1, worldOrigin);
    QPointF wp2 = geoToWorldPixel(coord2, worldOrigin);

    // Bounding box of the line in world space
    QRectF bbox = QRectF(wp1, wp2).normalized();
    int minTileX = static_cast<int>(std::floor(bbox.left() / tileMap.tileSize));
    int maxTileX = static_cast<int>(std::floor(bbox.right() / tileMap.tileSize));
    int minTileY = static_cast<int>(std::floor(bbox.top() / tileMap.tileSize));
    int maxTileY = static_cast<int>(std::floor(bbox.bottom() / tileMap.tileSize));

    for (int tx = minTileX; tx <= maxTileX; ++tx) {
        for (int ty = minTileY; ty <= maxTileY; ++ty) {
            Tile &tile = tileMap.getTile(tx, ty);
            tile.ensureSprayedPixels(); // ensure sprayedPixels exists

            // Transform world coords into tile-local pixel coordinates
            QPointF localP1((wp1.x() - tx * tileMap.tileSize) / tileMap.tileSize *
                                tile.resolution,
                            (wp1.y() - ty * tileMap.tileSize) / tileMap.tileSize *
                                tile.resolution);
            QPointF localP2((wp2.x() - tx * tileMap.tileSize) / tileMap.tileSize *
                                tile.resolution,
                            (wp2.y() - ty * tileMap.tileSize) / tileMap.tileSize *
                                tile.resolution);

            // Draw
            QImage img(tile.sprayedPixels->data(), tile.resolution, tile.resolution,
                       tile.resolution, QImage::Format_Grayscale8);

            QPainter painter(&img);
            painter.setRenderHint(QPainter::Antialiasing, false);

            QColor color(grayValue, grayValue, grayValue);
            painter.setPen(QPen(color, 1, Qt::SolidLine, Qt::FlatCap));

            painter.drawLine(localP1, localP2);
            painter.end();

            tile.dirty = true;
        }
    }
}

void PainterService::rasterizeField(const QGeoCoordinate &worldOrigin,
                                    const std::vector<QGeoCoordinate> &polygon,
                                    TileMap &tilesMap) {
    if (polygon.size() < 3)
        return;

    // Bounding box in world coords
    QPolygonF worldPolygon;
    for (const QGeoCoordinate &coord : polygon) {
        QPointF pt = geoToWorldPixel(coord, worldOrigin);
        worldPolygon << pt;
    }
    QRectF bbox = worldPolygon.boundingRect();

    int minTileX = static_cast<int>(std::floor(bbox.left() / tilesMap.tileSize));
    int maxTileX = static_cast<int>(std::floor(bbox.right() / tilesMap.tileSize));
    int minTileY = static_cast<int>(std::floor(bbox.top() / tilesMap.tileSize));
    int maxTileY =
        static_cast<int>(std::floor(bbox.bottom() / tilesMap.tileSize));

    const FieldService *fieldService = FieldService::getInstance();

    for (int tx = minTileX; tx <= maxTileX; ++tx) {
        for (int ty = minTileY; ty <= maxTileY; ++ty) {
            Tile &tile = tilesMap.getTile(tx, ty); // <-- use getTile

            for (int py = 0; py < tile.resolution; ++py) {
                for (int px = 0; px < tile.resolution; ++px) {
                    double worldX = tx * tilesMap.tileSize +
                                    (px + 0.5) * (tilesMap.tileSize / tile.resolution);
                    double worldY = ty * tilesMap.tileSize +
                                    (py + 0.5) * (tilesMap.tileSize / tile.resolution);

                    double metersPerDegLat = 111320.0;
                    double metersPerDegLon =
                        111320.0 * cos(worldOrigin.latitude() * M_PI / 180.0);
                    double lon = worldOrigin.longitude() + worldX / metersPerDegLon;
                    double lat = worldOrigin.latitude() + worldY / metersPerDegLat;

                    if (fieldService->contains(polygon, lat, lon)) {
                        setFieldPixel(tile, px, py, true);
                    }
                }
            }
        }
    }
}

void PainterService::rasterizeFieldOptimized(
    const QGeoCoordinate &worldOrigin,
    const std::vector<QGeoCoordinate> &geoCoords,
    TileMap &tileMap)
{
    if (geoCoords.size() < 3) return;

    // 1️⃣ Convert polygon to world coordinates
    QPolygonF worldPolygon;
    for (const auto &coord : geoCoords)
        worldPolygon << geoToWorldPixel(coord, worldOrigin);

    // 2️⃣ Determine bounding tiles
    QRectF bbox = worldPolygon.boundingRect();
    int minTileX = static_cast<int>(std::floor(bbox.left() / tileMap.tileSize));
    int maxTileX = static_cast<int>(std::floor(bbox.right() / tileMap.tileSize));
    int minTileY = static_cast<int>(std::floor(bbox.top() / tileMap.tileSize));
    int maxTileY = static_cast<int>(std::floor(bbox.bottom() / tileMap.tileSize));

    // 3️⃣ Rasterize per tile
    for (int tx = minTileX; tx <= maxTileX; ++tx) {
        for (int ty = minTileY; ty <= maxTileY; ++ty) {
            QRectF tileRect(tx * tileMap.tileSize,
                            ty * tileMap.tileSize,
                            tileMap.tileSize,
                            tileMap.tileSize);
            if (!tileRect.intersects(bbox))
                continue;

            Tile &tile = tileMap.getTile(tx, ty);
            tile.ensureFieldPixels();

            // 3a️⃣ Convert world polygon to tile-local pixel coordinates (integer)
            QList<QPointF> localPolygon;
            for (const QPointF &wp : worldPolygon) {
                double fx = (wp.x() - tx * tileMap.tileSize) / tileMap.tileSize;
                double fy = (wp.y() - ty * tileMap.tileSize) / tileMap.tileSize;

                int lx = static_cast<int>(fx * tile.resolution + 0.5);
                int ly = static_cast<int>(fy * tile.resolution + 0.5);
                localPolygon << QPointF(lx, ly);
            }
            QPolygonF polygon(localPolygon);

            // 3b️⃣ Compute tile-local bounding box in pixels
            int minX = tile.resolution - 1, maxX = 0, minY = tile.resolution - 1, maxY = 0;
            for (const QPointF &p : localPolygon) {
                minX = std::min(minX, static_cast<int>(p.x()));
                maxX = std::max(maxX, static_cast<int>(p.x()));
                minY = std::min(minY, static_cast<int>(p.y()));
                maxY = std::max(maxY, static_cast<int>(p.y()));
            }
            minX = std::max(0, minX);
            maxX = std::min(tile.resolution - 1, maxX);
            minY = std::max(0, minY);
            maxY = std::min(tile.resolution - 1, maxY);

            // 3c️⃣ Rasterize polygon directly into tile bitmask
            for (int y = minY; y <= maxY; ++y) {
                for (int x = minX; x <= maxX; ++x) {
                    if (polygon.containsPoint(QPointF(x, y), Qt::OddEvenFill)) {
                        setFieldPixel(tile, x, y, true);
                    }
                }
            }

            tile.fieldResolutionScale = 0;
        }
    }
}


