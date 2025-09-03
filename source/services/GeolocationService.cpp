// GeolocationService.cpp
#include "GeolocationService.h"
#include <QtMath>

constexpr double EARTH_RADIUS_CM =
    6371000.0 * 100; // Earth's radius in centimeters

QVector2D
GeolocationService::geoToCentimeters(const QGeoCoordinate &coord) const {
    double latRad = qDegreesToRadians(coord.latitude());
    double lonRad = qDegreesToRadians(coord.longitude());

    double northCm = latRad * EARTH_RADIUS_CM;
    double eastCm = lonRad * EARTH_RADIUS_CM * qCos(latRad);

    return QVector2D(eastCm, northCm);
}

QGeoCoordinate
GeolocationService::centimetersToGeo(const QVector2D &vec) const {
    double northCm = vec.y();
    double eastCm = vec.x();

    double latRad = northCm / EARTH_RADIUS_CM;
    double latitude = qRadiansToDegrees(latRad);

    double lonRad = eastCm / (EARTH_RADIUS_CM * qCos(latRad));
    double longitude = qRadiansToDegrees(lonRad);

    return QGeoCoordinate(latitude, longitude);
}

GeolocationService::GeolocationService() {
    // vehicleData.coordinate = QGeoCoordinate(-21.1245, -48.9917);
    vehicleData.coordinate = QGeoCoordinate( -21.124493305309855,
                                            -48.991681397538315);
    vehicleData.rotation = QVector3D(0.0, 0.0, 0.0); // yaw = Z axis
    vehicleData.speed = 0;
    rotationSpeed = 0;

    // Timer for updating position every 200ms
    connect(&updateTimer, &QTimer::timeout, this,
            &GeolocationService::UpdatePosition);
    updateTimer.start(200);
}

void GeolocationService::SetSpeed(double newSpeed) {
    vehicleData.speed = newSpeed;
}

void GeolocationService::SetCoordinate(const QGeoCoordinate &newCoordinate) {
    vehicleData.coordinate = newCoordinate;
}

void GeolocationService::SetRotation(const QVector3D &newRotation) {
    vehicleData.rotation = newRotation;
}

void GeolocationService::SetRotationSpeed(double newRotationSpeed) {
    rotationSpeed = newRotationSpeed;
}

VehicleData GeolocationService::getVehicleData() const { return vehicleData; }

QVector2D GeolocationService::coordinateInCentimeters() const {
    QVector2D location = geoToCentimeters(vehicleData.coordinate);

    location.setX(-location.x());
    location.setY(-location.y());

    return location;
}

float GeolocationService::eulerRotation() const {
    return vehicleData.rotation.z();
}

void GeolocationService::UpdatePosition() {
    // Update rotation from rotationSpeed
    double deltaTimeSec = 0.2;                           // 200ms
    double deltaRotation = rotationSpeed * deltaTimeSec; // degrees

    double newZ = vehicleData.rotation.z() + deltaRotation;
    // Wrap between 0 and 360 degrees
    newZ = fmod(newZ, 360.0);
    if (newZ < 0.0)
        newZ += 360.0;

    vehicleData.rotation.setZ(newZ);

    if (vehicleData.speed != 0.0) {
        QVector2D posCm = geoToCentimeters(vehicleData.coordinate);

        double distanceCm = vehicleData.speed * deltaTimeSec * 100.0; // speed m/s → cm

        double bearingRad = qDegreesToRadians(vehicleData.rotation.z());

        posCm.setX(posCm.x() + distanceCm * qSin(bearingRad)); // east
        posCm.setY(posCm.y() + distanceCm * qCos(bearingRad)); // north

        vehicleData.coordinate = centimetersToGeo(posCm);
    }

    // qDebug() << "lon: " << vehicleData.coordinate.longitude() << " | lat: " << vehicleData.coordinate.latitude();
}
