// GeolocationService.h
#ifndef GEOLOCATIONSERVICE_H
#define GEOLOCATIONSERVICE_H

#include "../entities/VehicleData.h"
#include "source/BaseService.h"

#include <QGeoCoordinate>
#include <QVector3D>
#include <QTimer>

class GeolocationService : public BaseService
{
    Q_OBJECT

public:
    GeolocationService();

    Q_INVOKABLE QVector2D geoToCentimeters(const QGeoCoordinate &coord) const;
    Q_INVOKABLE QGeoCoordinate centimetersToGeo(const QVector2D &vec) const;

    void SetSpeed(double newSpeed);
    void SetCoordinate(const QGeoCoordinate& newCoordinate);
    void SetRotation(const QVector3D& newRotation);
    void SetRotationSpeed(double newRotationSpeed);
    Q_INVOKABLE VehicleData getVehicleData() const;
    Q_INVOKABLE QVector2D coordinateInCentimeters() const;
    Q_INVOKABLE float eulerRotation() const;

private slots:
    void UpdatePosition();

private:
    VehicleData vehicleData;
    double rotationSpeed = 0.0; // degrees per second
    QTimer updateTimer;
};

#endif // GEOLOCATIONSERVICE_H
