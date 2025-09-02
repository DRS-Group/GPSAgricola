#include "AddJobView.h"
#include "source/services/JobsService.h"

AddJobView::AddJobView(QObject *parent) : BaseView(parent) {}

bool AddJobView::saveJob(AddJobViewModel *addJobViewModel) {
    qDebug() << addJobViewModel->name();
    qDebug() << addJobViewModel->type();
    qDebug() << addJobViewModel->fieldName();

    FieldService *fieldService = FieldService::getInstance();
    JobsService *jobsService = JobsService::getInstance();

    std::unique_ptr<BaseJob> job = jobsService->createJob(
        static_cast<JobType>(addJobViewModel->type()), addJobViewModel->name(),
        fieldService->getFieldByName(addJobViewModel->fieldName()));

    if (!job)
        return false;
    jobsService->saveJob(job.get());

    return true;
}
