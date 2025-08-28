#include "JobsService.h"
#include "source/entities/jobs/SprayJob.h"
#include "source/services/FieldService.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

JobsService *JobsService::instance = nullptr;

JobsService *JobsService::getInstance() {
    if (!instance) {
        instance = new JobsService();
    }
    return instance;
}

JobsService::JobsService(QObject *parent) : BaseService{parent} {
    // FieldService* fieldService = FieldService::getInstance();

    // Field field;
    // field.name = "Talhão 1";
    // field.polygon =
    // fieldService->loadFromGeoJSON("/home/gustavodbp/casa.json");

    // currentJob = createJob(JobType::Spray, "Trabalho 1", field);
}

std::unique_ptr<BaseJob> JobsService::createJob(JobType type, const QString &name, const Field &field) {
    switch (type) {
    case JobType::Spray: {
        std::unique_ptr<SprayJob> sprayJob =
            std::make_unique<SprayJob>(name, field);
        sprayJob->id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        return sprayJob;
    }
    // case JobType::Planter:
    //     return std::make_unique<PlanterJob>(name, field);
    // case JobType::SpotSpray:
    //     return std::make_unique<SpotSprayJob>(name, field);
    default:
        return nullptr;
    }
}

bool JobsService::saveJob(BaseJob *job) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << "Failed to create folder:" << folderPath;
            return false;
        }
    }

    QString filePath = folderPath + "/" + job->id + ".drs";

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_0);

    out << job->id;
    out << static_cast<int>(job->type());
    out << job->name;
    out << job->field.name;

    job->serialize(out);

    file.close();
    return true;
}

BaseJob *JobsService::loadJob(const QString &fileName) {
    QFile file(folderPath + "/" + fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << fileName;
        return nullptr;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_0);

    // Determine job type first
    QString id;
    int jobTypeInt;
    QString name;
    QDateTime creationDate;
    QString fieldName;

    in >> id >> jobTypeInt >> name >> creationDate >> fieldName;

    BaseJob *job = nullptr;
    JobType jobType = static_cast<JobType>(jobTypeInt);

    Field field = FieldService::getInstance()->getFieldByName(fieldName);

    if (jobType == JobType::Spray) {
        job = new SprayJob(name, field); // create field with name for now
    } else {
        // Handle other job types
    }

    if (job) {
        job->id = id;
        job->creationDateTime = creationDate;
        job->tiles.deserialize(in);
    }

    return job;
}

QStringList JobsService::getAllJobIds() const {
    QStringList jobIds;

    QDir dir(folderPath);
    if (!dir.exists()) {
        qWarning() << "Jobs folder does not exist:" << folderPath;
        return jobIds;
    }

    dir.setNameFilters(QStringList() << "*.drs");
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();

    for (const QFileInfo &fileInfo : fileList) {
        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open file:" << fileInfo.fileName();
            continue;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);

        QString savedId;
        in >> savedId; // first field is the job id

        // Check if filename (without extension) matches the id
        QString fileNameWithoutExt = fileInfo.completeBaseName();
        if (fileNameWithoutExt == savedId) {
            jobIds.append(savedId);
        } else {
            qWarning() << "Filename does not match saved job id:"
                       << fileInfo.fileName();
        }
    }

    return jobIds;
}

QList<BaseJob *> JobsService::getAllJobs() const {
    QList<BaseJob *> jobs;

    QDir dir(folderPath);
    if (!dir.exists()) {
        qWarning() << "Jobs folder does not exist:" << folderPath;
        return jobs;
    }

    dir.setNameFilters(QStringList() << "*.drs");
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();

    for (const QFileInfo &fileInfo : fileList) {
        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open file:" << fileInfo.fileName();
            continue;
        }

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);

        // Read job metadata first
        QString id;
        int jobTypeInt;
        QString name;
        QString fieldName;

        in >> id;
        in >> jobTypeInt;
        in >> name;
        in >> fieldName;

        // Check if file name matches job id
        QString fileBaseName =
            fileInfo.completeBaseName(); // filename without extension
        if (fileBaseName != id) {
            qWarning() << "File name does not match job id, skipping:"
                       << fileInfo.fileName();
            continue;
        }

        Field field = FieldService::getInstance()->getFieldByName(fieldName);
        SprayJob *job = new SprayJob(name, field);
        job->id = id;

        job->deserialize(in); // deserialize the rest
        jobs.append(job);
    }

    return jobs;
}
