#include "FieldsListModel.h"
#include <QDebug>

FieldsListModel::FieldsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int FieldsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_fields.size());
}

QVariant FieldsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_fields.size()))
        return QVariant();

    const Field &field = m_fields[index.row()];

    switch (role) {
    case NameRole:
        return field.name;
    case AreaRole:
        return field.area;
    case OriginLatRole:
        return field.origin.latitude();
    case OriginLonRole:
        return field.origin.longitude();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FieldsListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[AreaRole] = "area";
    roles[OriginLatRole] = "originLat";
    roles[OriginLonRole] = "originLon";
    return roles;
}

void FieldsListModel::setFields(const std::vector<Field> &fields)
{
    beginResetModel();
    m_fields = fields;
    endResetModel();
}

void FieldsListModel::addField(const Field &field)
{
    beginInsertRows(QModelIndex(), m_fields.size(), m_fields.size());
    m_fields.push_back(field);
    endInsertRows();
}

Field FieldsListModel::getField(int row) const
{
    if (row < 0 || row >= static_cast<int>(m_fields.size()))
        return Field();
    return m_fields[row];
}
