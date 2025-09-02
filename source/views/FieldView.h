#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include "../BaseView.h"
#include "QQuick3DTextureData"
#include "source/services/JobsService.h"
#include <QVector2D>
#include <QGeoCoordinate>

class FieldView : public BaseView {
    Q_OBJECT
    Q_PROPERTY(QVector2D fieldOrigin READ fieldOrigin NOTIFY fieldOriginChanged)
    Q_PROPERTY(int tileSize READ tileSize CONSTANT)
public:
    explicit FieldView(QObject *parent = nullptr);
    ~FieldView();

    void onQmlReady() override {}

    Q_INVOKABLE QQuick3DTextureData *getTileFieldTexture(int tileX, int tileY,
                                                         QQuick3DObject *parent);

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

    int tileSize() const {
        const JobsService* jobService = JobsService::getInstance();
        const BaseJob* currentJob = jobService->getCurrentJob();

        if(!currentJob) return 0;

        return currentJob->tiles.tileSize;
    }

signals:
    void fieldOriginChanged();

private:
    QVector2D m_fieldOrigin = QVector2D(0, 0);
};

#endif // FIELDVIEW_H
