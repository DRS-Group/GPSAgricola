#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include "../BaseView.h"
#include <QDebug>

class FieldView : public BaseView
{
    Q_OBJECT
public:
    explicit FieldView(QObject *parent = nullptr);

    void onQmlReady() override{
        qDebug() << "FieldView qml loaded";
    }
};

#endif // FIELDVIEW_H
