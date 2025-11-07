#include "qml/views/addJob/components/FieldSelectorModal.h"
#include "source/application.h"
#include "source/objects/AddJobViewModel.h"
#include "source/objects/FieldObject.h"
#include "source/views/AddFieldView.h"
#include "source/views/AddJobView.h"
#include "source/views/JobsView.h"
#include "source/SprayGeometry.h"
#include <QFont>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuick3DGeometry>

int main(int argc, char *argv[]) {
    qmlRegisterType<SprayGeometry>("GPSAgricola", 1, 0, "SprayGeometry");

    qmlRegisterType<FieldObject>("GPSAgricola", 1, 0, "FieldObject");
    qmlRegisterType<AddFieldView>("GPSAgricola", 1, 0, "AddFieldView");
    qmlRegisterType<JobsView>("GPSAgricola", 1, 0, "JobsView");
    qmlRegisterType<AddJobView>("GPSAgricola", 1, 0, "AddJobView");
    qmlRegisterType<FieldView>("GPSAgricola", 1, 0, "FieldView");

    qmlRegisterType<AddJobViewModel>("GpsAgricola", 1, 0, "AddJobViewModel");

    qmlRegisterType<FieldSelector>("GpsAgricola", 1, 0, "FieldSelectorCpp");

    Application app(argc, argv);

    app.load();

    return app.exec();
}
