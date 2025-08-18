#include <QQmlApplicationEngine>
#include <QFont>
#include <QQmlContext>
#include "source/application.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    app.load();

    return app.exec();
}
