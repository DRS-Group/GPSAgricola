#ifndef ADDJOBVIEW_H
#define ADDJOBVIEW_H

#include "source/BaseView.h"

class AddJobView : public BaseView {
    Q_OBJECT
public:
    explicit AddJobView(QObject *parent = nullptr);

    void onQmlReady() override {};
};

#endif // ADDJOBVIEW_H
