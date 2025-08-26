#ifndef VEHICLEDATA_H
#define VEHICLEDATA_H

#include <QGeoCoordinate>
#include <QVector3D>

struct VehicleData{
    QGeoCoordinate coordinate;
    QVector3D rotation;
    double speed;
};

#endif // VEHICLEDATA_H
