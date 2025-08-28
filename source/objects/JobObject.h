#ifndef JOBOBJECT_H
#define JOBOBJECT_H

#include "source/entities/jobs/BaseJob.h"
#include "source/entities/jobs/SprayJob.h"
#include <QGeoCoordinate>
#include <QObject>

class JobObject : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QGeoCoordinate origin READ origin NOTIFY originChanged)
    Q_PROPERTY(QDateTime creationDateTime READ creationDateTime NOTIFY
                   creationDateTimeChanged)
    Q_PROPERTY(int type READ type NOTIFY typeChanged) // expose JobType as int
    Q_PROPERTY(QString fieldName READ fieldName NOTIFY fieldNameChanged)

public:
    explicit JobObject(BaseJob *job, QObject *parent = nullptr)
        : QObject(parent), m_job(job) {}

    JobObject(const JobType &jobType, QObject *parent = nullptr)
        : QObject(parent) {
        if (jobType == JobType::Spray) {
            m_job = new SprayJob();
        }
    }

    // Accessors
    QString id() const { return m_job->id; }

    QString name() const { return m_job->name; }
    void setName(const QString &n) {
        if (m_job->name != n) {
            m_job->name = n;
            emit nameChanged();
        }
    }

    QGeoCoordinate origin() const { return m_job->origin; }
    QDateTime creationDateTime() const { return m_job->creationDateTime; }
    int type() const { return static_cast<int>(m_job->type()); }

    QString fieldName() const { return m_job->field.name; }

    // Expose backend functions if needed
    Q_INVOKABLE void update(const QGeoCoordinate &pos) { m_job->update(pos); }

    BaseJob *getJob() const { return m_job; }

signals:
    void idChanged();
    void nameChanged();
    void originChanged();
    void creationDateTimeChanged();
    void typeChanged();
    void fieldNameChanged();

private:
    BaseJob *m_job = nullptr; // not owned (depends on ownership design)
};

#endif // JOBOBJECT_H
