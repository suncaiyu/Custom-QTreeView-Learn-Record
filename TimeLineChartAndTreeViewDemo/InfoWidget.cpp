#include "InfoWidget.h"
#include <QPainter>
InfoWidget::InfoWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    treeView->setHeaderHidden(true);
    treeView_2->setHeaderHidden(true);
}

InfoWidget::~InfoWidget()
{
    if (detailsModel != nullptr) {
        delete detailsModel;
    }
    if (argsModel != nullptr) {
        delete argsModel;
    }
}

void InfoWidget::SetInfoData(TimeStamp tm)
{
    if (detailsModel != nullptr) {
        delete detailsModel;
        detailsModel = nullptr;
    }
    if (argsModel != nullptr) {
        delete argsModel;
        argsModel = nullptr;
    }
    detailsModel = new QStandardItemModel;
    treeView->setModel(detailsModel);
    argsModel = new QStandardItemModel;
    treeView_2->setModel(argsModel);

    SetInfo("Name", tm.name, detailsModel);
    SetInfo("Start Time", QString::number(tm.start), detailsModel);
    SetInfo("Thread Name", tm.threadName, detailsModel);
    SetInfo("Process Name", tm.processName, detailsModel);
    SetInfo("Duration", QString::number(tm.end - tm.start), detailsModel);

    SetInfo("start_time", QString::number(tm.start), argsModel);
    SetInfo("end_time", QString::number(tm.end), argsModel);
}

void InfoWidget::SetInfo(QString agr1, QString arg2, QStandardItemModel *model)
{
    QStandardItem *root = model->invisibleRootItem();
    QList<QStandardItem *> topList;
    QStandardItem *name = new QStandardItem(agr1);
    QStandardItem *nameValue = new QStandardItem(arg2);
    topList.append(name);
    topList.append(nameValue);
    root->appendRow(topList);
}

//void InfoWidget::paintEvent(QPaintEvent *e)
//{
//    QPainter p(this);
//    p.fillRect(rect(), Qt::green);
//}
