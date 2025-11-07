#pragma once
#include <QVector2D>
#include <QVector>
#include <QtQuick3D/QQuick3DGeometry>

class SprayGeometry : public QQuick3DGeometry {
    Q_OBJECT
    Q_PROPERTY(float offset READ offset WRITE setOffset NOTIFY offsetChanged FINAL)
public:
    SprayGeometry();

    bool x1 = false;

    float offset() { return this->m_offset; };
    void setOffset(float offset) { this->m_offset = offset; };
    QList<QVector2D> getPath() const { return m_path; }
    void setPath(const QList<QVector2D> &path);

    Q_INVOKABLE void addPathPoint(float x, float y) {
        QVector2D newPoint(-x, y);

        if (!m_path.isEmpty()) {
            const QVector2D &last = m_path.last();
            float dist = (newPoint - last).length();
            if (dist < 10.0f)
                return; // too close, skip
        }

        m_path.append(newPoint);
        updateData(generateTriangleStrip(100));
    }

signals:
    void pathChanged();
    void offsetChanged();

private:
    void updateData(const QList<QVector2D> &strip);
    QList<QVector2D> generateTriangleStrip(float offset);
    QList<QVector2D> generateShiftedPath(float offset) const;
    QList<QVector2D> m_path;
    float m_offset = 0;
};
