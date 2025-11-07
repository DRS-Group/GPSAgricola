#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include "../BaseView.h"
#include "QQuick3DTextureData"
#include "source/SprayGeometry.h"
#include <QVector2D>
#include <QGeoCoordinate>

class FieldView : public BaseView {
    Q_OBJECT
    Q_PROPERTY(QVector2D fieldOrigin READ fieldOrigin NOTIFY fieldOriginChanged)
    Q_PROPERTY(SprayGeometry* sprayGeometry READ sprayGeometry WRITE setSprayGeometry NOTIFY sprayGeometryChanged)
public:
    explicit FieldView(QObject *parent = nullptr);
    ~FieldView();

    void onQmlReady() override {}

    Q_INVOKABLE QVector2D coordinateInCentimeters();

    Q_INVOKABLE void setSpeed(float speed);
    Q_INVOKABLE void setRotationSpeed(float rotationSpeed);

    Q_INVOKABLE QVector2D geoToCentimeters(const QGeoCoordinate &coord) const;

    QVector2D fieldOrigin() const { return m_fieldOrigin; }
    void setFieldOrigin(const QVector2D &origin) {
        if (m_fieldOrigin != origin) {
            m_fieldOrigin = origin;
            emit fieldOriginChanged();
        }
    }

    SprayGeometry *sprayGeometry() const { return m_sprayGeometry; }
    void setSprayGeometry(SprayGeometry *geometry) {
        if (m_sprayGeometry == geometry)
            return;
        m_sprayGeometry = geometry;
        emit sprayGeometryChanged();
    }

signals:
    void fieldOriginChanged();
    void sprayGeometryChanged();

private:
    QVector2D m_fieldOrigin = QVector2D(0, 0);
        SprayGeometry *m_sprayGeometry = new SprayGeometry(); // default instance
};

#endif // FIELDVIEW_H
