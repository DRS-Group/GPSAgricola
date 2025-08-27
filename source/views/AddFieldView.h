#ifndef ADDFIELDVIEW_H
#define ADDFIELDVIEW_H

#include "source/BaseView.h"
#include "source/objects/FieldObject.h"
#include "source/services/FieldService.h"

class AddFieldView : public BaseView {
    Q_OBJECT
    Q_PROPERTY(FieldObject *field READ getField NOTIFY fieldChanged)
public:
    explicit AddFieldView(QObject *parent = nullptr);
    ~AddFieldView();

    void onQmlReady() override;

    Q_INVOKABLE void loadFromFile(QUrl filePath);
    Q_INVOKABLE bool saveField();

    FieldObject *getField() const { return field; }

signals:
    void fieldChanged();

private:
    FieldService *fieldService;
    FieldObject *field = nullptr;
};

#endif // ADDFIELDVIEW_H
