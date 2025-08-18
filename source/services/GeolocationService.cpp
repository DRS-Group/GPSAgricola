#include "GeolocationService.h"

GeolocationService::GeolocationService() {}

void GeolocationService::SetSpeed(const double newSpeed){
    this->vehicleData.speed = newSpeed;
}

void GeolocationService::SetCoordinate(const QGeoCoordinate& newCoordinate){
    this->vehicleData.coordinate.setCoordinate(newCoordinate);
}

void GeolocationService::SetRotation(const QVector3D& newRotation){
    this->vehicleData.rotation = newRotation;
}
