#ifndef APPLICATION_H
#define APPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>

#include "BaseView.h"
// #include "source/views/AddFieldView.h"
#include "views/FieldView.h"
#include "views/FieldsView.h"
#include "views/HomeView.h"

class Application : public QGuiApplication {
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    ~Application();

    static Application *getInstance();

    void load();
    QQmlApplicationEngine *getEngine() const;

    QString currentPageTitle = "";
    BaseView *currentView = nullptr;

    Q_INVOKABLE void onStackViewItemChanged(QQuickItem *currentItem) {
        if (currentItem) {
            // Get the objectName of the current item
            QString itemName = currentItem->property("title").toString();

            this->currentPageTitle = itemName;

            if (this->currentView) {
                delete this->currentView;
                this->currentView = nullptr;
            }

            if (this->currentPageTitle == "HomeView")
                this->currentView = new HomeView(this);
            else if (this->currentPageTitle == "FieldView")
                this->currentView = new FieldView(this);
            else if (this->currentPageTitle == "FieldsView")
                this->currentView = new FieldsView(this);
            // else if (this->currentPageTitle == "AddFieldView")
            //     this->currentView = new AddFieldView(this);

            if(this->currentView)
                this->currentView->onQmlReady();
        }
    }

private:
    static Application *m_instance;
    QQmlApplicationEngine *engine;

signals:
    // Add your custom signals here
};

#endif // APPLICATION_H
