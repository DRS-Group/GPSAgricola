#include "FieldSelectorModal.h"
#include <QBuffer>

FieldSelector::FieldSelector(QObject *parent) : QObject(parent){
    FieldService* fieldService = FieldService::getInstance();

    for (Field field : fieldService->getAllFields()){
        FieldObject *fieldObject = new FieldObject();
        fieldObject->setField(field);
        fields.append(fieldObject);
    }
}

QUrl FieldSelector::renderFieldAsBase64(const QString &fieldName, int width, int height, int border){
    FieldService* fieldService = FieldService::getInstance();
    auto fields = fieldService->getAllFields();

    auto it = std::find_if(fields.begin(), fields.end(),
                           [&](const Field &f){ return f.name == fieldName; });
    if (it != fields.end()) {
        QImage img = fieldService->renderFieldPolygon(*it, width, height, border);
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "png");
        QString base64 = QString::fromUtf8(byteArray.toBase64());
        return QString("data:image/png;base64,") + base64;
    }

    return QString();
}
