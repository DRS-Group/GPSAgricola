#include "fieldview.h"
#include "../application.h"

#include <QTimer>
#include <QQuickItem>
#include <QtConcurrent/QtConcurrentRun>


FieldView::FieldView(QObject *parent)
    : BaseView(parent)
{
    QObject *rootObject = Application::getInstance()->getEngine()->rootObjects().first();
    if (rootObject) {
        QObject *fieldViewQml = rootObject->findChild<QObject*>("FieldView");
        if (fieldViewQml) {
            QMetaObject::invokeMethod(fieldViewQml, "myQmlFunction",
                                      Qt::QueuedConnection, // Use queued connection for thread safety
                                      Q_ARG(QVariant, "Hello from C++!"));
        }
    }
}
