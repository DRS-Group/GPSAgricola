#ifndef APPLICATION_H
#define APPLICATION_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>

#include "BaseView.h"
#include "views/HomeView.h"
#include "views/FieldView.h"

class Application : public QGuiApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    ~Application();

    static Application* getInstance();

    void load();
    QQmlApplicationEngine* getEngine() const;

    QString currentPageTitle = "";
    BaseView* currentView = nullptr;

    Q_INVOKABLE void onStackViewItemChanged(QQuickItem* currentItem)
    {
        if (currentItem) {
            // Get the objectName of the current item
            QString itemName = currentItem->property("title").toString();
            qDebug() << "StackView item has changed. New item name:" << itemName;

            this->currentPageTitle = itemName;

            if(currentView) free(currentView);

            if (this->currentPageTitle == "HomeView")
                this->currentView = new HomeView(this);
            else if (this->currentPageTitle == "FieldView")
                this->currentView = new FieldView(this);
        }
    }

private:
    static Application* m_instance;
    QQmlApplicationEngine* engine;

signals:
    // Add your custom signals here
};

#endif // APPLICATION_H
