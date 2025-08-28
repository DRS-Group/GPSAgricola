#ifndef BASEJOB_H
#define BASEJOB_H

#include "../Field.h"
#include "source/entities/JobTypes.h"
#include "source/entities/TileMap.h"
#include <QGeoCoordinate>
#include <QString>

class BaseJob {
public:
    BaseJob(const QString &name, const Field &field);
    BaseJob();
    virtual ~BaseJob() = default;

    virtual JobType type() const = 0;
    virtual void serialize(QDataStream &out) const = 0;
    virtual void deserialize(QDataStream &in) = 0;
    virtual void update(const QGeoCoordinate &position) = 0;

    QString id;
    QString name;
    QGeoCoordinate origin;
    Field field;
    QDateTime creationDateTime;

    void redrawField();
    QQuick3DTextureData *getTileFieldTexture(int tileX, int tileY,
                                             QQuick3DObject *parent);

    TileMap tiles;
};

#endif // BASEJOB_H
