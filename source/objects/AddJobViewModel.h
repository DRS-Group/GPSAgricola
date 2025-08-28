#ifndef ADDJOBVIEWMODEL_H
#define ADDJOBVIEWMODEL_H

#include "source/services/FieldService.h"
#include <QObject>
#include <QString>

class AddJobViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY
                   typeChanged) // expose JobType as int
    Q_PROPERTY(QString fieldName READ fieldName WRITE setFieldName NOTIFY
                   fieldNameChanged)

public:
    explicit AddJobViewModel(QObject *parent = nullptr) : QObject(parent) {}

    // Accessors
    QString name() const { return m_name; }
    void setName(const QString &n) {
        if (m_name != n) {
            m_name = n;
            emit nameChanged();
        }
    }

    int type() const { return m_type; }
    void setType(int t) {
        if (m_type != t) {
            m_type = t;
            emit typeChanged();
        }
    }

    QString fieldName() const { return m_fieldName; }
    void setFieldName(const QString &f) {
        if (m_fieldName != f) {
            m_fieldName = f;
            emit fieldNameChanged();
        }
    }

    // To be implemented later
    Q_INVOKABLE QString renderFieldAsUrl(int width, int height, int border = 0) {
        FieldService *service = FieldService::getInstance();
        Field field = service->getFieldByName(m_fieldName);
        return service->renderFieldAsUrl(field, width, height, border).toString();
    }

signals:
    void nameChanged();
    void typeChanged();
    void fieldNameChanged();

private:
    QString m_name;
    int m_type = 0; // default: JobType::Spray (or whatever you want)
    QString m_fieldName;
};

#endif // ADDJOBVIEWMODEL_H
