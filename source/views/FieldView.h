#ifndef FIELDVIEW_H
#define FIELDVIEW_H

#include "../BaseView.h"
#include "QQuick3DTextureData"

class FieldView : public BaseView {
    Q_OBJECT
public:
    explicit FieldView(QObject *parent = nullptr);
    ~FieldView();

    void onQmlReady() override {}

    Q_INVOKABLE QQuick3DTextureData *getTileFieldTexture(int tileX, int tileY,
                                                         QQuick3DObject *parent);
};

#endif // FIELDVIEW_H
