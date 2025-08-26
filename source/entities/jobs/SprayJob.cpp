#include "SprayJob.h"

SprayJob::SprayJob(const QString &name, const Field &field) : BaseJob(name, field) {

}

void SprayJob::serialize(QDataStream &out) const{

}
void SprayJob::deserialize(QDataStream &in) {

}

void SprayJob::update(const QGeoCoordinate &position) {

}
