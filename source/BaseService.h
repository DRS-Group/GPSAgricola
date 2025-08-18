#ifndef BASESERVICE_H
#define BASESERVICE_H

#include <QQuickItem>

class BaseService: public QObject
{
    Q_OBJECT
public:
    BaseService(QObject *parent = nullptr);
};

#endif // BASESERVICE_H
