#include "FieldsView.h"
#include "../application.h"
#include "source/ServicesManager.h"

#include <QQmlContext>
#include <QBuffer>

FieldsView::FieldsView(QObject *parent) : BaseView(parent) {
    Application::getInstance()->getEngine()->rootContext()->setContextProperty("fieldsViewCpp", this);
}

FieldsView::~FieldsView(){
    Application::getInstance()->getEngine()->rootContext()->setContextProperty("fieldsViewCpp", nullptr);
    Application::getInstance()->getEngine()->rootContext()->setContextProperty("fieldModel", nullptr);

    delete fieldModel;
    fieldModel= nullptr;
}

void FieldsView::onQmlReady(){
    fieldService = ServicesManager::getInstance()->fieldService;

    fieldModel = new FieldsListModel();
    fieldModel->setFields(fieldService->getAllFields());

    Application::getInstance()->getEngine()->rootContext()->setContextProperty("fieldModel", fieldModel);
}

QUrl imageToUrl(const QImage& image)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "png");
    QString base64 = QString::fromUtf8(byteArray.toBase64());
    return QString("data:image/png;base64,") + base64;
}

QUrl FieldsView::renderFieldAsBase64(const QString &fieldName, int width, int height, int border) {
    FieldService* fieldService = ServicesManager::getInstance()->fieldService;
    auto fields = fieldService->getAllFields();

    auto it = std::find_if(fields.begin(), fields.end(),
                           [&](const Field &f){ return f.name == fieldName; });
    if (it != fields.end()) {
        QImage img = fieldService->renderFieldPolygon(*it, width, height, border);
        return imageToUrl(img);
    }

    return QString();
}
