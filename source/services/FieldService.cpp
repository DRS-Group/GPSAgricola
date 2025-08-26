#include "FieldService.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QPainter>

FieldService *FieldService::instance = nullptr;

FieldService::FieldService(QObject *parent) : BaseService(parent) {
    std::vector<QGeoCoordinate> polygon = loadFromGeoJSON("/home/gustavodbp/casa.json");
    Field field = Field("Talhão 3", polygon);

    saveField(field);
}

FieldService *FieldService::getInstance() {
    if (!instance) {
        instance = new FieldService();
    }
    return instance;
}

std::vector<QGeoCoordinate>
FieldService::loadFromGeoJSON(const QString &filePath) const {
    std::vector<QGeoCoordinate> polygon;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open GeoJSON file:" << filePath;
        return polygon;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject())
        return polygon;

    QJsonObject root = doc.object();
    if (!root.contains("type") || root["type"].toString() != "FeatureCollection")
        return polygon;

    QJsonArray features = root["features"].toArray();
    if (features.isEmpty())
        return polygon;

    // Take first polygon only
    QJsonObject geometry = features[0].toObject()["geometry"].toObject();
    if (geometry["type"].toString() != "Polygon")
        return polygon;

    QJsonArray coords = geometry["coordinates"].toArray()[0].toArray();

    for (const auto &pointValue : std::as_const(coords)) {
        QJsonArray point = pointValue.toArray();
        if (point.size() >= 2) {
            double lon = point[0].toDouble();
            double lat = point[1].toDouble();
            polygon.emplace_back(lat, lon);
        }
    }

    return polygon;
}

// Ray-casting algorithm for point-in-polygon
bool FieldService::contains(const std::vector<QGeoCoordinate> &polygon,
                            double latitude, double longitude) {
    bool inside = false;
    int n = polygon.size();
    if (n < 3)
        return false;

    for (int i = 0, j = n - 1; i < n; j = i++) {
        double xi = polygon[i].longitude();
        double yi = polygon[i].latitude();
        double xj = polygon[j].longitude();
        double yj = polygon[j].latitude();

        bool intersect = ((yi > latitude) != (yj > latitude)) &&
                         (longitude < (xj - xi) * (latitude - yi) / (yj - yi) + xi);
        if (intersect)
            inside = !inside;
    }
    return inside;
}

bool FieldService::saveField(const Field &field) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << "Failed to create folder:" << folderPath;
            return false; // can't save without a folder
        }
    }

    QString filePath = folderPath + "/" + field.name + ".json";

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QJsonObject json;
    json["name"] = field.name;

    QJsonArray coords;
    for (const auto &c : field.polygon) {
        QJsonObject point;
        point["lat"] = c.latitude();
        point["lon"] = c.longitude();
        coords.append(point);
    }
    json["polygon"] = coords;

    QJsonObject origin;
    origin["lat"] = field.origin.latitude();
    origin["lon"] = field.origin.longitude();
    json["origin"] = origin;

    json["area"] = field.area;

    QJsonDocument doc(json);
    file.write(doc.toJson());
    return true;
};


std::vector<Field> FieldService::getAllFields() const {
    std::vector<Field> fields;

    QDir dir(folderPath);
    if (!dir.exists()) {
        qWarning() << "Field folder does not exist:" << folderPath;
        return fields;
    }

    dir.setNameFilters(QStringList() << "*.json");
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();

    for (const QFileInfo &fileInfo : fileList) {
        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open file:" << fileInfo.fileName();
            continue;
        }

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isObject()) {
            qWarning() << "Invalid JSON in file:" << fileInfo.fileName();
            continue;
        }

        QJsonObject obj = doc.object();
        QString name = obj["name"].toString();
        QJsonArray coordsArray = obj["polygon"].toArray();
        std::vector<QGeoCoordinate> polygon;

        for (const auto &v : coordsArray) {
            QJsonObject point = v.toObject();
            double lat = point["lat"].toDouble();
            double lon = point["lon"].toDouble();
            polygon.emplace_back(lat, lon);
        }

        Field field(name, polygon);
        // Optionally, calculate area here
        fields.push_back(field);
    }

    return fields;
}


bool saveImage(const QImage &image, const QString &filePath)
{
    if (image.isNull()) {
        qWarning() << "Cannot save an empty image!";
        return false;
    }

    // The format is inferred from the file extension (e.g., ".png", ".jpg")
    bool success = image.save(filePath);
    if (!success)
        qWarning() << "Failed to save image to" << filePath;

    return success;
}

QImage FieldService::renderFieldPolygon(const Field &field, int width, int height, int border) const
{
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    // Style: border is the border
    QPen pen(QColor(0, 100, 0, 255), border, Qt::SolidLine, Qt::RoundCap);
    QBrush brush(QColor(0, 200, 0, 255)); // semi-transparent fill
    painter.setPen(pen);
    painter.setBrush(brush);

    if (field.polygon.empty())
        return image;

    // Determine polygon bounding box
    double minLat = field.polygon[0].latitude();
    double maxLat = field.polygon[0].latitude();
    double minLon = field.polygon[0].longitude();
    double maxLon = field.polygon[0].longitude();

    for (const auto &p : field.polygon) {
        minLat = qMin(minLat, p.latitude());
        maxLat = qMax(maxLat, p.latitude());
        minLon = qMin(minLon, p.longitude());
        maxLon = qMax(maxLon, p.longitude());
    }

    // Offset to ensure the pen stays inside the image
    double penOffset = border / 2.0;
    double usableWidth = width - 2.0 * penOffset;
    double usableHeight = height - 2.0 * penOffset;

    // Map lat/lon to pixel coordinates considering the pen offset
    auto mapToImage = [&](const QGeoCoordinate &c) {
        double x = (c.longitude() - minLon) / (maxLon - minLon) * usableWidth + penOffset;
        double y = (1.0 - (c.latitude() - minLat) / (maxLat - minLat)) * usableHeight + penOffset;
        return QPointF(x, y);
    };

    QPolygonF poly;
    for (const auto &c : field.polygon)
        poly << mapToImage(c);

    painter.drawPolygon(poly);

    // Optional: save the image for debugging
    saveImage(image, "/home/gustavodbp/field.png");

    return image;
}
