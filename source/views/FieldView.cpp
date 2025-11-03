#include "FieldView.h"
#include "source/ServicesManager.h"

#include <QQuickItem>
#include <QTimer>
#include <QtConcurrent/QtConcurrentRun>
#include <QQmlContext>
#include <QtConcurrent>

FieldView::FieldView(QObject *parent) : BaseView(parent) {
    JobsService *jobsService = JobsService::getInstance();
    GeolocationService* geolocationService = ServicesManager::getInstance()->geolocationService;
    setFieldOrigin(geolocationService->geoToCentimeters(jobsService->getCurrentJob()->field.origin));

    // Application::getInstance()->getEngine()->rootContext()->setContextProperty("fieldViewCpp", this);

    // ServicesManager::getInstance()->geolocationService->SetSpeed(50);
    // ServicesManager::getInstance()->geolocationService->SetRotationSpeed(1);



    auto job = jobsService->getCurrentJob();

    QFuture<void> future = QtConcurrent::run([job]() {
        job->redrawField();
    });

    // connect to callback when finished
    auto watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished, this, [this]() {
        emit fieldReady();   // custom signal to QML
    });
    watcher->setFuture(future);
}

FieldView::~FieldView(){
    // Application::getInstance()->getEngine()->rootContext()->setContextProperty("fieldViewCpp", nullptr);
}

QQuick3DTextureData *FieldView::getTileFieldTexture(int tileX, int tileY,
                                                    QQuick3DObject *parent, float resolutionScale) {
    JobsService *jobsService = JobsService::getInstance();
    BaseJob *currentJob = jobsService->getCurrentJob();
    return currentJob->getTileFieldTexture(tileX, tileY, parent, resolutionScale);
}

QQuick3DTextureData* FieldView::getTileSpotsTexture(int tileX, int tileY,
                                                    QQuick3DObject* parent, float resolutionScale) {
    JobsService* jobsService = JobsService::getInstance();
    BaseJob* currentJob = jobsService->getCurrentJob();
    return currentJob->getTileSpotsTexture(tileX, tileY, parent, resolutionScale);
}


QVector2D FieldView::coordinateInCentimeters(){
    GeolocationService* geolocationService = ServicesManager::getInstance()->geolocationService;
    return geolocationService->coordinateInCentimeters();
}

void FieldView::setSpeed(float speed) {
    auto geo = ServicesManager::getInstance()->geolocationService;
    if (geo)
        geo->SetSpeed(speed);
}

void FieldView::setRotationSpeed(float rotationSpeed) {
    auto geo = ServicesManager::getInstance()->geolocationService;
    if (geo)
        geo->SetRotationSpeed(rotationSpeed);
}


QVector2D FieldView::geoToCentimeters(const QGeoCoordinate &coord) const {
    GeolocationService* geo = ServicesManager::getInstance()->geolocationService;
    if (geo) {
        return geo->geoToCentimeters(coord);
    }
    return QVector2D(0, 0); // fallback if geolocation service is null
}
