#ifndef SPRAYJOB_H
#define SPRAYJOB_H

#include "BaseJob.h"

class SprayJob : public BaseJob {
public:
    SprayJob(const QString& name, const Field& field);

    JobType type() const override { return JobType::Spray; }
    void serialize(QDataStream &out) const override;
    void deserialize(QDataStream &in) override;
    void update(const QGeoCoordinate &position) override;


};

#endif // SPRAYJOB_H
