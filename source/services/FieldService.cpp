#include "FieldService.h"
#include <QBuffer>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <qstandardpaths.h>

FieldService *FieldService::instance = nullptr;

FieldService::FieldService(QObject *parent) : BaseService(parent) {
    // std::vector<QGeoCoordinate> polygon =
    //     loadFromGeoJSON("/home/gustavodbp/casa.json");
    // Field field = Field("Talhão 3", polygon);

    // saveField(field);
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

std::vector<std::vector<QGeoCoordinate>>
FieldService::loadManyFromGeoJSON(const QString &filePath) const {
    std::vector<std::vector<QGeoCoordinate>> polygons;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open GeoJSON file:" << filePath;
        return polygons;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject())
        return polygons;

    QJsonObject root = doc.object();
    if (!root.contains("type") || root["type"].toString() != "FeatureCollection")
        return polygons;

    QJsonArray features = root["features"].toArray();
    if (features.isEmpty())
        return polygons;

    for (const auto &featureVal : features) {
        QJsonObject feature = featureVal.toObject();
        if (!feature.contains("geometry"))
            continue;

        QJsonObject geometry = feature["geometry"].toObject();
        QString type = geometry["type"].toString();

        if (type == "Polygon") {
            // GeoJSON polygon: coordinates = array of linear rings
            QJsonArray rings = geometry["coordinates"].toArray();
            for (const auto &ringVal : rings) {
                std::vector<QGeoCoordinate> ring;
                QJsonArray ringCoords = ringVal.toArray();
                for (const auto &ptVal : ringCoords) {
                    QJsonArray pt = ptVal.toArray();
                    if (pt.size() >= 2) {
                        double lon = pt[0].toDouble();
                        double lat = pt[1].toDouble();
                        ring.emplace_back(lat, lon);
                    }
                }
                if (!ring.empty())
                    polygons.push_back(std::move(ring));
            }
        } else if (type == "MultiPolygon") {
            // GeoJSON multipolygon: array of polygons, each = array of rings
            QJsonArray multi = geometry["coordinates"].toArray();
            for (const auto &polyVal : multi) {
                QJsonArray rings = polyVal.toArray();
                for (const auto &ringVal : rings) {
                    std::vector<QGeoCoordinate> ring;
                    QJsonArray ringCoords = ringVal.toArray();
                    for (const auto &ptVal : ringCoords) {
                        QJsonArray pt = ptVal.toArray();
                        if (pt.size() >= 2) {
                            double lon = pt[0].toDouble();
                            double lat = pt[1].toDouble();
                            ring.emplace_back(lat, lon);
                        }
                    }
                    if (!ring.empty())
                        polygons.push_back(std::move(ring));
                }
            }
        }
    }

    return polygons;
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

Field FieldService::getFieldByName(const QString &name) const {
    auto fields = getAllFields();

    for (const auto &field : fields) {
        if (field.name == name) {
            return field;
        }
    };

    return Field();
}

bool saveImage(const QImage &image, const QString &filePath) {
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

QImage FieldService::renderFieldPolygon(const Field &field, int width,
                                        int height, int border) const {
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    if (field.polygon.size() < 3) // Not a polygon
        return image;

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor(0, 100, 0, 255), border, Qt::SolidLine, Qt::RoundCap);
    QBrush brush(QColor(0, 200, 0, 255)); // semi-transparent fill
    painter.setPen(pen);
    painter.setBrush(brush);

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

    double usableWidth = width - border;
    double usableHeight = height - border;

    // Compute scale to preserve aspect ratio
    double lonRange = maxLon - minLon;
    double latRange = maxLat - minLat;
    double scaleX = usableWidth / lonRange;
    double scaleY = usableHeight / latRange;
    double scale = qMin(scaleX, scaleY);

    // Center offsets
    double xOffset = (usableWidth - lonRange * scale) / 2.0 + border / 2.0;
    double yOffset = (usableHeight - latRange * scale) / 2.0 + border / 2.0;

    // Map coordinates to image
    auto mapToImage = [&](const QGeoCoordinate &c) {
        double x = (c.longitude() - minLon) * scale + xOffset;
        double y = (maxLat - c.latitude()) * scale + yOffset; // flip Y
        return QPointF(x, y);
    };

    QPolygonF poly;
    for (const auto &c : field.polygon)
        poly << mapToImage(c);

    painter.drawPolygon(poly);

    // Optional: save for debugging
    saveImage(image, "/home/gustavodbp/field.png");

    return image;
}

QImage FieldService::renderSpots(const std::vector<std::vector<QGeoCoordinate>> &spots,
                                 int width, int height, int border) const {
    QImage image(width, height, QImage::Format_ARGB32);
    image.fill(Qt::transparent);

    if (spots.empty())
        return image;

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::red, border > 0 ? border : 2, Qt::SolidLine, Qt::RoundCap);
    QBrush brush(QColor(255, 0, 0, 128)); // semi-transparent red
    painter.setPen(pen);
    painter.setBrush(brush);

    // compute bounding box across all spots
    double minLat = spots[0][0].latitude();
    double maxLat = spots[0][0].latitude();
    double minLon = spots[0][0].longitude();
    double maxLon = spots[0][0].longitude();

    for (const auto &ring : spots) {
        for (const auto &p : ring) {
            minLat = qMin(minLat, p.latitude());
            maxLat = qMax(maxLat, p.latitude());
            minLon = qMin(minLon, p.longitude());
            maxLon = qMax(maxLon, p.longitude());
        }
    }

    double usableWidth = width - border;
    double usableHeight = height - border;
    double lonRange = maxLon - minLon;
    double latRange = maxLat - minLat;
    double scaleX = usableWidth / lonRange;
    double scaleY = usableHeight / latRange;
    double scale = qMin(scaleX, scaleY);

    double xOffset = (usableWidth - lonRange * scale) / 2.0 + border / 2.0;
    double yOffset = (usableHeight - latRange * scale) / 2.0 + border / 2.0;

    auto mapToImage = [&](const QGeoCoordinate &c) {
        double x = (c.longitude() - minLon) * scale + xOffset;
        double y = (maxLat - c.latitude()) * scale + yOffset;
        return QPointF(x, y);
    };

    for (const auto &ring : spots) {
        if (ring.size() < 3) continue;
        QPolygonF poly;
        for (const auto &c : ring)
            poly << mapToImage(c);
        painter.drawPolygon(poly);
    }

    return image;
}

QImage FieldService::renderFieldWithSpots(const Field &field,
                                          const std::vector<std::vector<QGeoCoordinate>> &spots,
                                          int width, int height, int border) const {
    // First draw the field
    QImage image = renderFieldPolygon(field, width, height, border);

    if (spots.empty())
        return image;

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap);
    QBrush brush(QColor(255, 0, 0, 128));
    painter.setPen(pen);
    painter.setBrush(brush);

    // compute bounding box of the field to reuse mapping
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

    double usableWidth = width - border;
    double usableHeight = height - border;
    double lonRange = maxLon - minLon;
    double latRange = maxLat - minLat;
    double scaleX = usableWidth / lonRange;
    double scaleY = usableHeight / latRange;
    double scale = qMin(scaleX, scaleY);

    double xOffset = (usableWidth - lonRange * scale) / 2.0 + border / 2.0;
    double yOffset = (usableHeight - latRange * scale) / 2.0 + border / 2.0;

    auto mapToImage = [&](const QGeoCoordinate &c) {
        double x = (c.longitude() - minLon) * scale + xOffset;
        double y = (maxLat - c.latitude()) * scale + yOffset;
        return QPointF(x, y);
    };

    // Draw each spot polygon
    for (const auto &ring : spots) {
        if (ring.size() < 3) continue;
        QPolygonF poly;
        for (const auto &c : ring)
            poly << mapToImage(c);
        painter.drawPolygon(poly);
    }

    return image;
}


QUrl FieldService::renderFieldAsUrl(const Field &field, int width, int height,
                                    int border) const {
    QImage img = renderFieldPolygon(field, width, height, border);

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "png");
    QString base64 = QString::fromUtf8(byteArray.toBase64());
    return QString("data:image/png;base64,") + base64;

    return QString();
}

QUrl FieldService::renderFieldWithSpotsAsUrl(const Field& field,
                                             const std::vector<std::vector<QGeoCoordinate>>& spots,
                                             int width,
                                             int height,
                                             int border) const
{
    // Render combined field + spots
    QImage img = renderFieldWithSpots(field, spots, width, height, border);

    // Save into a temporary writable location
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if (tempDir.isEmpty()) {
        tempDir = QDir::tempPath();
    }

    QDir dir(tempDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString tempFilePath = dir.filePath("field_with_spots.png");
    img.save(tempFilePath, "PNG");

    return QUrl::fromLocalFile(tempFilePath);
}
