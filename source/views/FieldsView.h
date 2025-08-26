#ifndef FIELDSVIEW_H
#define FIELDSVIEW_H

#include "source/BaseView.h"
#include "source/models/FieldsListModel.h"
#include "source/services/FieldService.h"
#include <QImage>

class FieldsView : public BaseView
{
    Q_OBJECT
public:
    explicit FieldsView(QObject *parent = nullptr);
    ~FieldsView();

    void onQmlReady() override;

    Q_INVOKABLE QUrl renderFieldAsBase64(const QString &fieldName, int width, int height, int border=0);

private:
    FieldsListModel *fieldModel;
    FieldService* fieldService;
};

#endif // FIELDSVIEW_H
