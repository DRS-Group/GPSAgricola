#include "FieldView.h"
#include "../application.h"
#include "source/ServicesManager.h"

#include <QQuickItem>
#include <QTimer>
#include <QtConcurrent/QtConcurrentRun>
#include <qqmlcontext.h>

FieldView::FieldView(QObject *parent) : BaseView(parent) {
    Application::getInstance()->getEngine()->rootContext()->setContextProperty("fieldViewCpp", this);

    ServicesManager::getInstance()->geolocationService->SetSpeed(10);
    ServicesManager::getInstance()->geolocationService->SetRotationSpeed(10);

    QObject *rootObject =
        Application::getInstance()->getEngine()->rootObjects().first();
    if (rootObject) {
        QObject *fieldViewQml = rootObject->findChild<QObject *>("FieldView");
        if (fieldViewQml) {
            QMetaObject::invokeMethod(fieldViewQml, "createTiles");
        }
    }
}

FieldView::~FieldView(){
    Application::getInstance()->getEngine()->rootContext()->setContextProperty("fieldViewCpp", nullptr);
}

QQuick3DTextureData *FieldView::getTileFieldTexture(int tileX, int tileY,
                                                    QQuick3DObject *parent) {
    JobsService *jobsService = JobsService::getInstance();
    BaseJob *currentJob = jobsService->getCurrentJob();
    return currentJob->getTileFieldTexture(tileX, tileY, parent);
}
