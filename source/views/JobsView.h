#ifndef JOBSVIEW_H
#define JOBSVIEW_H

#include "source/BaseView.h"
#include "source/objects/JobObject.h"
#include "source/services/JobsService.h"

class JobsView : public BaseView {
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> jobs READ getJobs NOTIFY jobsChanged)
public:
    explicit JobsView(QObject *parent = nullptr);
    void onQmlReady() override {};

    QList<QObject *> getJobs() const {
        QList<QObject *> list;
        for (auto *job : jobs)
            list.append(job);
        return list;
    }

    Q_INVOKABLE QUrl renderFieldAsBase64(const QString &fieldName, int width, int height, int border=0);

signals:
    void jobsChanged();

private:
    QList<JobObject *> jobs;
    JobsService *jobsService;
};

#endif // JOBSVIEW_H
