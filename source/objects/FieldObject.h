#ifndef FIELDOBJECT_H
#define FIELDOBJECT_H

#include "source/entities/Field.h"
#include "source/services/FieldService.h"
#include <QObject>
#include <QGeoCoordinate>

class FieldObject : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(float area READ area NOTIFY areaChanged)
    Q_PROPERTY(QGeoCoordinate origin READ origin NOTIFY originChanged)
    Q_PROPERTY(QVector<QGeoCoordinate> polygon READ polygon WRITE setPolygon NOTIFY polygonChanged)

public:
    explicit FieldObject(QObject* parent = nullptr) : QObject(parent) {}

    // Set the internal Field struct
    void setField(const Field& f) {
        m_field = f;
        emit nameChanged();
        emit areaChanged();
        emit originChanged();
    }

    // Q_PROPERTY accessors
    QString name() const { return m_field.name; }
    void setName(const QString& n) {
        if (m_field.name != n) {
            m_field.name = n;
            emit nameChanged();
        }
    }

    float area() const { return m_field.area; }
    QGeoCoordinate origin() const { return m_field.origin; }

    QVector<QGeoCoordinate> polygon() const {
        return QVector<QGeoCoordinate>(m_field.polygon.begin(), m_field.polygon.end());
    }

    void setPolygon(const QVector<QGeoCoordinate>& poly) {
        std::vector<QGeoCoordinate> stdPoly(poly.begin(), poly.end());
        if (stdPoly != m_field.polygon) {
            m_field.polygon = stdPoly;
            recalcField();
            emit polygonChanged();
        }
    }

    // Expose FieldService rendering
    Q_INVOKABLE QUrl renderFieldAsUrl(int width, int height, int border = 0) const {
        FieldService service;
        return service.renderFieldAsUrl(m_field, width, height, border).toString();
    }

    Field& getField(){
        return m_field;
    };

signals:
    void nameChanged();
    void areaChanged();
    void originChanged();
    void polygonChanged();

private:
    Field m_field;  // your original struct

    void recalcField() {
        m_field.area = m_field.calculateArea();
        m_field.origin = m_field.calculateOrigin();
        emit areaChanged();
        emit originChanged();
    }

};

#endif // FIELDOBJECT_H
