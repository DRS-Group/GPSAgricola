#ifndef VEHICLEDATA_H
#define VEHICLEDATA_H

#include <QGeoLocation>
#include <QVector3D>

struct VehicleData{
    QGeoLocation coordinate;
    QVector3D rotation;
    double speed;

};

#endif // VEHICLEDATA_H
