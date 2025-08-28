#ifndef FIELDSELECTORMODAL_H
#define FIELDSELECTORMODAL_H

#include "source/objects/FieldObject.h"
#include <QObject>
class FieldSelector : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> fields READ getFields NOTIFY fieldsChanged)
public:
    FieldSelector(QObject *parent = nullptr);

    QList<QObject *> getFields() const {
        QList<QObject *> list;
        for (auto *field : fields)
            list.append(field);
        return list;
    }

    Q_INVOKABLE QUrl renderFieldAsBase64(const QString &fieldName, int width, int height, int border=0);

signals:
    void fieldsChanged();

private:
    QList<FieldObject *> fields;
};

#endif // FIELDSELECTORMODAL_H
