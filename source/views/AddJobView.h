#ifndef ADDJOBVIEW_H
#define ADDJOBVIEW_H

#include "source/BaseView.h"
#include "source/objects/AddJobViewModel.h"

class AddJobView : public BaseView {
    Q_OBJECT
public:
    explicit AddJobView(QObject *parent = nullptr);

    void onQmlReady() override {};

    Q_INVOKABLE void loadSpotFromFile(QUrl filePath);
    Q_INVOKABLE bool saveJob(AddJobViewModel* addJobViewModel);

private:
    FieldService *fieldService;
};

#endif // ADDJOBVIEW_H
