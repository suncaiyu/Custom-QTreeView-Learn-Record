#include "createmodel.h"
#include <QDebug>
CreateModel::CreateModel()
{
    initModel();
}

void CreateModel::initModel()
{
    QStringList headList;
    headList << "ID" << "API" << "DUR(us)" << "Button";
    mModel = new TreeModel(headList);
    TreeItem *root = mModel->root();
    for (int i = 0; i < 10; i++) {
        TreeItem *item1 = new TreeItem(root);
        QVector<QVariant> data1;
        data1 << i << "api"<< "first" + QString::number(i) << "";
        item1->setItemData(data1);
        root->appendChild(item1);

        for (int j = 0; j < 10; j++) {
            TreeItem *item2 = new TreeItem(item1);
            QVector<QVariant> data2;
            data2 << j << "myapi" + QString::number(i + j) << "second" + QString::number(i + j) << "";
            item2->setItemData(data2);
            item1->appendChild(item2);
            for (int k = 0; k < 10; k++) {
                TreeItem *item3 = new TreeItem(item2);
                QVector<QVariant> data3;
                data3 << k << "myapi" + QString::number(i + j) << "third" + QString::number(i + j + k) << "";
                item3->setItemData(data3);
                item2->appendChild(item3);
            }
        }
    }
    qDebug() << "done";
}

QString CreateModel::getHeaderString(int column)
{
    return mModel->headerData(column, Qt::Horizontal).toString();
}
