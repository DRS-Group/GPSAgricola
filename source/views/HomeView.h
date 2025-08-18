#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include "../BaseView.h"
#include <QDebug>

class HomeView : public BaseView
{
public:
    explicit HomeView(QObject *parent = nullptr);

    void onQmlReady() override{
        qDebug() << "FieldView qml loaded";
    }
};

#endif // HOMEVIEW_H
