#ifndef PAINTERSERVICE_H
#define PAINTERSERVICE_H

#include "../BaseService.h"
#include "source/BaseService.h"
#include "source/ServicesManager.h"
#include "source/entities/TileMap.h"
#include "source/services/GeolocationService.h"
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

    void rasterizeFieldOptimized(const QGeoCoordinate &worldOrigin,
                                 const std::vector<QGeoCoordinate> &geoCoords,
                                 TileMap &tileMap);

void rasterizeSpotsOptimized(
    const QGeoCoordinate &worldOrigin,
    const std::vector<std::vector<QGeoCoordinate>> &spots,
        TileMap &tileMap);

private:
    static PainterService *instance;

    QPointF geoToWorldPixel(const QGeoCoordinate &coord,
                            const QGeoCoordinate &worldOrigin) const {

        GeolocationService *geolocationService = ServicesManager::getInstance()->geolocationService;

        // Get position relative to world origin in centimeters
        QVector2D relCm = geolocationService->geoToCentimeters(coord) -
                          geolocationService->geoToCentimeters(worldOrigin);

        // Convert to meters if needed
        double dx = relCm.x() / 100.0;
        double dy = relCm.y() / 100.0;

        return QPointF(dx, dy);
    }
};

#endif // PAINTERSERVICE_H
