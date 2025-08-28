#include "JobsView.h"
#include "source/entities/jobs/SprayJob.h"
#include "source/services/FieldService.h"

#include <QBuffer>

JobsView::JobsView(QObject *parent) : BaseView(parent) {
    jobsService = JobsService::getInstance();
    // FieldService *fieldService = FieldService::getInstance();
    // Field field = fieldService->getFieldByName("Campo 1");

    // std::unique_ptr<BaseJob> job = jobsService->createJob(JobType::Spray, "Trablaho 1", field);

    // jobs.append(jobObject);

    // jobsService->saveJob(job.get());

    // BaseJob* job = jobsService->loadJob("teste.drs");
    // JobObject* jobObject = new JobObject(job);
    // jobs.append(jobObject);

    QList<BaseJob *> jobsList = jobsService->getAllJobs();
    for (BaseJob *job : jobsList) {
        JobObject *jobObject = new JobObject(job);
        jobs.append(jobObject);
    }
}

QUrl JobsView::renderFieldAsBase64(const QString &fieldName, int width,
                                   int height, int border) {
    FieldService *fieldService = FieldService::getInstance();
    auto fields = fieldService->getAllFields();

    auto it = std::find_if(fields.begin(), fields.end(),
                           [&](const Field &f) { return f.name == fieldName; });
    if (it != fields.end()) {
        QImage img = fieldService->renderFieldPolygon(*it, width, height, border);
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer, "png");
        QString base64 = QString::fromUtf8(byteArray.toBase64());
        return QString("data:image/png;base64,") + base64;
    }

    return QString();
}
