#ifndef PAINTERSERVICE_H
#define PAINTERSERVICE_H

#include "../BaseService.h"
#include "source/BaseService.h"
#include "source/entities/Tile.h"
#include "source/entities/TileMap.h"
#include <QColor>
#include <QGeoCoordinate>
#include <QImage>

#include <QBrush>
#include <QPainter>
#include <QPen>

#include <QDir>
#include <QQuick3DTextureData>

class PainterService : public BaseService {
    Q_OBJECT
public:
    PainterService(QObject *parent = nullptr);

    static PainterService *getInstance();

    void drawPolygonGeo(TileMap &tileMap, const QGeoCoordinate &worldOrigin,
                        const QList<QGeoCoordinate> &geoCoords,
                        uint8_t grayValue);

    void drawLineGeo(TileMap &tileMap, const QGeoCoordinate &worldOrigin,
                     const QGeoCoordinate &coord1, const QGeoCoordinate &coord2,
                     uint8_t grayValue);

    Q_INVOKABLE void rasterizeField(const QGeoCoordinate &worldOrigin,
                                    const std::vector<QGeoCoordinate> &polygon,
                                    TileMap &tilesMap);

private:
    static PainterService *instance;

    QGeoCoordinate worldOrigin =
        QGeoCoordinate(-21.12174192463783, -48.96224030991499);
    int tileResolution = 1000;
    float tileSize = 100;

    QPointF geoToWorldPixel(const QGeoCoordinate &coord,
                            const QGeoCoordinate &worldOrigin) const {
        // Flat-earth approximation
        double metersPerDegLat = 111320.0;
        double metersPerDegLon =
            111320.0 * cos(worldOrigin.latitude() * M_PI / 180.0);

        double dx = (coord.longitude() - worldOrigin.longitude()) * metersPerDegLon;
        double dy = (coord.latitude() - worldOrigin.latitude()) * metersPerDegLat;

        return QPointF(dx, dy);
    }
};

#endif // PAINTERSERVICE_H
