#ifndef INFOWIDGET_H
#define INFOWIDGET_H
#include <QWidget>
#include <QStandardItemModel>
#include "ChartData.h"
#include "./ui_InfoWidget.h"

class InfoWidget : public QWidget, private Ui::InfoWidget
{
    Q_OBJECT
public:
    InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

    void SetInfoData(TimeStamp tm);

private:
    void SetInfo(QString agr1, QString arg2, QStandardItemModel *model);

    QStandardItemModel *detailsModel = nullptr;
    QStandardItemModel *argsModel = nullptr;
};

#endif // INFOWIDGET_H
