#ifndef HOMEVIEW_H
#define HOMEVIEW_H

#include "../BaseView.h"

class HomeView : public BaseView
{
public:
    explicit HomeView(QObject *parent = nullptr);

    void onQmlReady() override{

    }
};

#endif // HOMEVIEW_H
