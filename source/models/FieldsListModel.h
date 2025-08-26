#ifndef FIELDSLISTMODEL_H
#define FIELDSLISTMODEL_H

#include <QAbstractListModel>
#include "source/entities/Field.h"
#include <vector>

class FieldsListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum FieldRoles {
        NameRole = Qt::UserRole + 1,
        AreaRole,
        OriginLatRole,
        OriginLonRole
        // You could add more roles, e.g., PolygonRole, ImageRole
    };

    explicit FieldsListModel(QObject *parent = nullptr);

    // Basic functionality
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Populate model
    void setFields(const std::vector<Field> &fields);
    void addField(const Field &field);
    Field getField(int row) const;

private:
    std::vector<Field> m_fields;
};

#endif // FIELDSLISTMODEL_H
