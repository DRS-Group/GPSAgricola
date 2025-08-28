#include "SprayJob.h"

SprayJob::SprayJob(const QString &name, const Field &field)
    : BaseJob(name, field) {}

SprayJob::SprayJob()
    : BaseJob() {}

void SprayJob::serialize(QDataStream &out) const { tiles.serialize(out); }

void SprayJob::deserialize(QDataStream &in) {
    tiles.deserialize(in); // 6. tiles (you need a deserialize method in TileMap)
}

void SprayJob::update(const QGeoCoordinate &position) {}
