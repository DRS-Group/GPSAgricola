// Copyright (C) 2020 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "SprayGeometry.h"
#include <QRandomGenerator>
#include <QVector3D>

SprayGeometry::SprayGeometry() {

    // updateData();
}

void SprayGeometry::setPath(const QList<QVector2D> &path) {
    if (path == m_path)
        return;
    m_path = path;
    emit pathChanged();
    // updateData();
}

void SprayGeometry::updateData(const QList<QVector2D> &strip) {
    if (strip.size() < 3)
        return; // Not enough vertices for a triangle strip

    clear();

    int stride = 3 * sizeof(float);
    int vertexCount = strip.size();
    QByteArray vertexData(vertexCount * stride,
                          Qt::Initialization::Uninitialized);
    float *p = reinterpret_cast<float *>(vertexData.data());

    for (const QVector2D &pt : strip) {
        *p++ = pt.x();
        *p++ = pt.y();
        *p++ = 0.0f;
    }

    setVertexData(vertexData);
    setStride(stride);

    // Compute bounds
    QVector2D minPt = strip.first();
    QVector2D maxPt = strip.first();
    for (const QVector2D &pt : strip) {
        minPt.setX(std::min(minPt.x(), pt.x()));
        minPt.setY(std::min(minPt.y(), pt.y()));
        maxPt.setX(std::max(maxPt.x(), pt.x()));
        maxPt.setY(std::max(maxPt.y(), pt.y()));
    }

    setBounds(QVector3D(minPt.x(), minPt.y(), 0.0f),
              QVector3D(maxPt.x(), maxPt.y(), 0.0f));
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::TriangleStrip);

    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic, 0,
                 QQuick3DGeometry::Attribute::F32Type);

    update();
}

QList<QVector2D> SprayGeometry::generateTriangleStrip(float width) {
    QList<QVector2D> path = generateShiftedPath(this->offset());

    QList<QVector2D> strip;
    if (path.size() < 2)
        return strip;

    for (int i = 0; i < path.size(); ++i) {
        QVector2D dir;

        if (i == 0)
            dir = (path[i + 1] - path[i]).normalized();
        else if (i == path.size() - 1)
            dir = (path[i] - path[i - 1]).normalized();
        else
            dir = ((path[i + 1] - path[i - 1]) * 0.5f).normalized();

        QVector2D right(-dir.y(), dir.x());
        QVector2D leftPos = path[i] - right * width;
        QVector2D rightPos = path[i] + right * width;

        // Order is important for triangle strip pattern
        strip.append(leftPos);
        strip.append(rightPos);
    }

    return strip;
}

QList<QVector2D> SprayGeometry::generateShiftedPath(float offset) const {
    QList<QVector2D> shifted;
    if (m_path.size() < 2)
        return shifted;

    for (int i = 0; i < m_path.size(); ++i) {
        QVector2D dir;

        if (i == 0)
            dir = (m_path[i + 1] - m_path[i]).normalized();
        else if (i == m_path.size() - 1)
            dir = (m_path[i] - m_path[i - 1]).normalized();
        else
            dir = ((m_path[i + 1] - m_path[i - 1]) * 0.5f).normalized();

        QVector2D right(-dir.y(), dir.x());
        shifted.append(m_path[i] + right * offset);
    }

    return shifted;
}
