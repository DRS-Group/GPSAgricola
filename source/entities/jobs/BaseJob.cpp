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
}

QQuick3DTextureData *BaseJob::getTileFieldTexture(int tileX, int tileY,
                                                  QQuick3DObject *parent) {
    return tiles.getTileFieldTexture(tileX, tileY, parent);
}
