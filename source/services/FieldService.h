#ifndef FIELDSERVICE_H
#define FIELDSERVICE_H

#include "source/BaseService.h"
#include "source/entities/Field.h"

#include <QObject>
#include <QGeoCoordinate>
#include <vector>
#include <QImage>

class FieldService : public BaseService
{
    Q_OBJECT
public:
    explicit FieldService(QObject *parent = nullptr);

    static FieldService* getInstance();

    std::vector<QGeoCoordinate> loadFromGeoJSON(const QString &filePath) const;

    // Check if point is inside a given polygon
    static bool contains(const std::vector<QGeoCoordinate>& polygon,
                         double latitude, double longitude);


    bool saveField(const Field& field);
    std::vector<Field> getAllFields() const;
    QImage renderFieldPolygon(const Field &field, int width, int height, int border = 0) const;
    QUrl renderFieldAsUrl(const Field &field, int width, int height,
                          int border) const;

private:
    static FieldService* instance;

    QString folderPath = "/home/gustavodbp/fields";
};

#endif // FIELDSERVICE_H
