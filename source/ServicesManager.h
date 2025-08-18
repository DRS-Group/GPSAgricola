#ifndef SERVICESMANAGER_H
#define SERVICESMANAGER_H

#include "source/services/PainterService.h"
class ServicesManager
{
public:
    ServicesManager();

    static ServicesManager* getInstance();
    PainterService* painterService = new PainterService();

private:
    static ServicesManager* instance;
};

#endif // SERVICESMANAGER_H
