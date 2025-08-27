#include <QQmlApplicationEngine>
#include <QFont>
#include <QQmlContext>
#include "source/application.h"
#include "source/objects/FieldObject.h"
#include "source/views/AddFieldView.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<FieldObject>("GPSAgricola", 1, 0, "FieldObject");
    qmlRegisterType<AddFieldView>("GPSAgricola", 1, 0, "AddFieldView");

    Application app(argc, argv);

    app.load();

    return app.exec();
}
