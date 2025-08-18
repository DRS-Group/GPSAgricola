#ifndef BASEVIEW_H
#define BASEVIEW_H

#include <QObject>

class BaseView : public QObject
{
    Q_OBJECT
public:
    explicit BaseView(QObject *parent = nullptr);

    virtual void onQmlReady() = 0;
signals:
};

#endif // BASEVIEW_H
