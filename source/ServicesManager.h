#ifndef SERVICESMANAGER_H
#define SERVICESMANAGER_H

#include "source/services/FieldService.h"
#include "source/services/GeolocationService.h"
#include "source/services/JobsService.h"
#include "source/services/PainterService.h"

class ServicesManager
{
public:
    ServicesManager();

    static ServicesManager* getInstance();
    GeolocationService* geolocationService = new GeolocationService();
    FieldService* fieldService = FieldService::getInstance();
    PainterService* painterService = PainterService::getInstance();
    JobsService* jobsService = JobsService::getInstance();
private:
    static ServicesManager* instance;
};

#endif // SERVICESMANAGER_H
