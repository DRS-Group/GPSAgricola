#include "application.h"
#include "ServicesManager.h"
#include <QFont>
#include <QQmlContext>

Application *Application::m_instance = nullptr;

Application::Application(int &argc, char **argv) : QGuiApplication(argc, argv) {
    if (m_instance != nullptr) {
        qWarning("An instance of Application already exists.");
        // You might want to handle this more gracefully, e.g., by exiting.
    }
    m_instance = this;

    QFont font("Arial");
    this->setFont(font);

    this->engine = new QQmlApplicationEngine(this);
}

Application::~Application() {
    m_instance = nullptr; // Clear the instance pointer on destruction
}

Application *Application::getInstance() { return m_instance; }

void Application::load() {
    QObject::connect(
        engine, &QQmlApplicationEngine::objectCreationFailed, this,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    ServicesManager *servicesManager = ServicesManager::getInstance();
    PainterService *painterService = servicesManager->painterService;
    GeolocationService *geolocationService = servicesManager->geolocationService;
    JobsService *jobsService = servicesManager->jobsService;

    engine->rootContext()->setContextProperty("application", this);
    engine->rootContext()->setContextProperty("painterService", painterService);
    engine->rootContext()->setContextProperty("geolocationService",
                                              geolocationService);
    engine->rootContext()->setContextProperty("jobsService", jobsService);
    engine->loadFromModule("GPSAgricola", "Main");
}

QQmlApplicationEngine *Application::getEngine() const { return this->engine; }
