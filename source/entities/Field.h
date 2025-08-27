#ifndef FIELD_H
#define FIELD_H

#include <QGeoCoordinate>
#include <QString>
#include <vector>

struct Field {
    Field() {};
    Field(QString name, std::vector<QGeoCoordinate> polygon)
        : name(name), polygon(polygon) {
        origin = calculateOrigin();
        area = calculateArea();
    }

    QString name = "";
    std::vector<QGeoCoordinate> polygon = {};
    QGeoCoordinate origin = QGeoCoordinate(0, 0);
    float area = 0;

public:
    float calculateArea() {
        if (polygon.size() < 3)
            return 0.0f; // Not a polygon

        // Convert lat/lon to meters relative to first point
        std::vector<std::pair<double, double>> pointsMeters;
        double refLat = polygon[0].latitude();
        double refLon = polygon[0].longitude();

        for (const auto &p : polygon) {
            double latMid = (refLat + p.latitude()) / 2.0;
            double metersPerDegLat = 111132.92 -
                                     559.82 * cos(2 * latMid * M_PI / 180.0) +
                                     1.175 * cos(4 * latMid * M_PI / 180.0);
            double metersPerDegLon = 111412.84 * cos(latMid * M_PI / 180.0) -
                                     93.5 * cos(3 * latMid * M_PI / 180.0);

            double x = (p.longitude() - refLon) * metersPerDegLon;
            double y = (p.latitude() - refLat) * metersPerDegLat;
            pointsMeters.emplace_back(x, y);
        }

        // Shoelace formula
        double area = 0.0;
        int n = pointsMeters.size();
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            area += pointsMeters[i].first * pointsMeters[j].second -
                    pointsMeters[j].first * pointsMeters[i].second;
        }

        return std::abs(area) / 2.0; // area in m²
    }

    QGeoCoordinate calculateOrigin() {
        if (polygon.empty())
            return QGeoCoordinate();

        double minLat = polygon[0].latitude();
        double maxLat = polygon[0].latitude();
        double minLon = polygon[0].longitude();
        double maxLon = polygon[0].longitude();

        for (const auto &point : polygon) {
            minLat = qMin(minLat, point.latitude());
            maxLat = qMax(maxLat, point.latitude());
            minLon = qMin(minLon, point.longitude());
            maxLon = qMax(maxLon, point.longitude());
        }

        double originLat = (minLat + maxLat) / 2.0;
        double originLon = (minLon + maxLon) / 2.0;

        return QGeoCoordinate(originLat, originLon);
    }
};

#endif // FIELD_H
