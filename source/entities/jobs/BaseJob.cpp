#include "BaseJob.h"
#include "source/services/PainterService.h"

BaseJob::BaseJob(const QString &name, const Field &field) {
    this->name = name;
    this->field = field;
}

BaseJob::BaseJob(){
    this->name = "";
    this->field = Field();
}

void BaseJob::redrawField() {
    PainterService::getInstance()->rasterizeFieldOptimized(field.origin, field.polygon, tiles);

    std::vector<std::vector<QGeoCoordinate>> polygons = FieldService::getInstance()->loadManyFromGeoJSON("/home/gustavodbp/spots/spots1.geojson");

    PainterService::getInstance()->rasterizeSpotsOptimized(field.origin, polygons, tiles);
}

QQuick3DTextureData *BaseJob::getTileFieldTexture(int tileX, int tileY,
                                                  QQuick3DObject *parent, float resolutionScale) {
    return tiles.getTileFieldTexture(tileX, tileY, parent, resolutionScale);
}

QQuick3DTextureData* BaseJob::getTileSpotsTexture(int tileX, int tileY,
                                                  QQuick3DObject* parent, float resolutionScale) {
    return tiles.getTileSpotsTexture(tileX, tileY, parent, resolutionScale);
}
