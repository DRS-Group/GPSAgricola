#ifndef ADDJOBVIEW_H
#define ADDJOBVIEW_H

#include "source/BaseView.h"
#include "source/objects/AddJobViewModel.h"

class AddJobView : public BaseView {
    Q_OBJECT
public:
    explicit AddJobView(QObject *parent = nullptr);

    void onQmlReady() override {};

    Q_INVOKABLE bool saveJob(AddJobViewModel* addJobViewModel);
};

#endif // ADDJOBVIEW_H
