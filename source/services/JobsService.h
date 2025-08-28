#ifndef JOBSSERVICE_H
#define JOBSSERVICE_H

#include "source/BaseService.h"
#include "source/entities/jobs/BaseJob.h"

class JobsService : public BaseService {
    Q_OBJECT
public:
    explicit JobsService(QObject *parent = nullptr);

    static JobsService *getInstance();

    std::unique_ptr<BaseJob> createJob(JobType type, const QString &name,
                                       const Field &field);

    BaseJob *getCurrentJob() const { return currentJob.get(); }

    bool saveJob(BaseJob *job);
    BaseJob *loadJob(const QString &fileName);
    QStringList getAllJobIds() const;
    QList<BaseJob *> getAllJobs() const;

private:
    static JobsService *instance;

    QString folderPath = "/home/gustavodbp/jobs";

    std::unique_ptr<BaseJob> currentJob;
};

#endif // JOBSSERVICE_H
