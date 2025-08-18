#include "ServicesManager.h"

ServicesManager* ServicesManager::instance = nullptr;

ServicesManager::ServicesManager() {}

ServicesManager* ServicesManager::getInstance(){
    if(!instance){
        instance = new ServicesManager();
    }
    return instance;
}
