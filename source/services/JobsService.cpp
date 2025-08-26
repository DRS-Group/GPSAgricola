#include "JobsService.h"
#include "source/entities/jobs/SprayJob.h"
#include "source/services/FieldService.h"

JobsService *JobsService::instance = nullptr;

JobsService *JobsService::getInstance() {
    if (!instance) {
        instance = new JobsService();
    }
    return instance;
}

JobsService::JobsService(QObject *parent)
    : BaseService{parent}
{
    // FieldService* fieldService = FieldService::getInstance();

    // Field field;
    // field.name = "Talhão 1";
    // field.polygon = fieldService->loadFromGeoJSON("/home/gustavodbp/casa.json");

    // currentJob = createJob(JobType::Spray, "Trabalho 1", field);
}

std::unique_ptr<BaseJob> JobsService::createJob(JobType type, const QString& name, const Field& field) {
    switch(type) {
    case JobType::Spray:
        return std::make_unique<SprayJob>(name, field);
    // case JobType::Planter:
    //     return std::make_unique<PlanterJob>(name, field);
    // case JobType::SpotSpray:
    //     return std::make_unique<SpotSprayJob>(name, field);
    default:
        return nullptr;
    }
}
