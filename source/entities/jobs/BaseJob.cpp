#include "BaseJob.h"
#include "source/services/PainterService.h"

BaseJob::BaseJob(const QString &name, const Field &field) {
    this->name = name;
    this->field = field;
    this->origin.setLatitude(-21.118922992405288);
    this->origin.setLongitude(-48.96440682249107);

    redrawField();
}

void BaseJob::redrawField() {
    PainterService::getInstance()->rasterizeField(origin, field.polygon, tiles);
}

QQuick3DTextureData *BaseJob::getTileFieldTexture(int tileX, int tileY,
                                                  QQuick3DObject *parent) {
    return tiles.getTileFieldTexture(tileX, tileY, parent);
}
