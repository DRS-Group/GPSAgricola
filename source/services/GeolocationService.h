#ifndef GEOLOCATIONSERVICE_H
#define GEOLOCATIONSERVICE_H

#include "../types/VehicleData.h"
#include "source/BaseService.h"

class GeolocationService : public BaseService
{
public:
    GeolocationService();

    VehicleData vehicleData;

    void SetSpeed(const double newSpeed);
    void SetCoordinate(const QGeoCoordinate& newCoordinate);
    void SetRotation(const QVector3D& newRotation);
};

#endif // GEOLOCATIONSERVICE_H
