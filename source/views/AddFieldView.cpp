#include "AddFieldView.h"
#include "../application.h"
#include "source/ServicesManager.h"

#include <QBuffer>
#include <QQmlContext>
#include <QGeoCoordinate>
#include <QVector>

AddFieldView::AddFieldView(QObject *parent) : BaseView(parent) {
    fieldService = ServicesManager::getInstance()->fieldService;

    field = new FieldObject(this);
    field->setName("Campo 1");
}

AddFieldView::~AddFieldView() {
    Application::getInstance()->getEngine()->rootContext()->setContextProperty(
        "addFieldViewCpp", nullptr);

    delete field;
    field = nullptr;
}

void AddFieldView::onQmlReady() {}

void AddFieldView::loadFromFile(QUrl filePath){
    std::vector<QGeoCoordinate> polygon = fieldService->loadFromGeoJSON(filePath.toLocalFile());
    field->setPolygon(QVector<QGeoCoordinate>(polygon.begin(), polygon.end()));
}

bool AddFieldView::saveField(){
    if(field->polygon().size() < 3){
        qWarning() << "Field polygon is not defined";
        return false;
    }

    if(field->name() == ""){
        qWarning() << "Field name is not defined";
        return false;
    }

    return fieldService->saveField(field->getField());
}
