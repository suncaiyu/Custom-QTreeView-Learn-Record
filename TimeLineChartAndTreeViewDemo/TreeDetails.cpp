#include "TreeDetails.h"
#include <QTableWidget>
#include "DelegateAndModel/TreeModel.h"
#include "DelegateAndModel/TreeDelegate.h"
#include "DelegateAndModel/MyHeaderView.h"
TreeDetails::TreeDetails(WebSocketClient &client, QWidget *parent) :
    QWidget(parent), mClient(&client)
{
    setupUi(this);
    treeView->setAlternatingRowColors(true);
    treeView->horizontalScrollBar();
    treeView->setHeader(new MyHeaderView(Qt::Orientation::Horizontal));
    QStringList headList;
    headList << "Name" << "Value" << "Details";
    mModel = new TreeModel(headList, treeView);
    mParser = new JsonParser;
    mParser->LoadFileForTree(":/b", mModel->root());
    fliterModel = new TreeModel(headList, treeView);
    for (int i = 0; i < mModel->root()->child(0)->childCount(); i++) {
        TreeItem *tmp = mModel->root()->child(0)->child(i);
        for (int j = 0; j < tmp->childCount(); j++) {
            TreeItem *tmp1 = tmp->child(j);
            TreeItem *app= new TreeItem(fliterModel->root());
            QVector<QVariant> data1;
            data1 << tmp1->data(0) << tmp1->child(0)->data(1);
            app->setItemData(data1);
            fliterModel->root()->appendChild(app);
//            qDebug() << tmp1->data(0);
//            qDebug() << tmp1->child(0)->data(0);
        }
    }
    treeView->setModel(fliterModel);
//    qDebug() << mModel->root()->child(0)->childCount();

//    treeView->setModel(mModel);
    mDelegate = new TreeDelegate(treeView, mParser->maxValue);
    treeView->setItemDelegate(mDelegate);
    treeView->expandAll();
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(mClient, &WebSocketClient::TreeModelDataSignal, [this](const QString &msg){
        mParser->LoadDataForTree(msg, mModel->root());
        treeView->setModel(mModel);
        mDelegate = new TreeDelegate(treeView, mParser->maxValue);
        treeView->setItemDelegate(mDelegate);
        treeView->expandAll();
    });
}

TreeDetails::~TreeDetails()
{
    delete mModel;
    delete mDelegate;
    delete treeView;
    delete verticalLayout;
    delete mParser;
}

void TreeDetails::ChangeModel()
{
    if (detailsModel) {
        treeView->setModel(fliterModel);
    } else {
        treeView->setModel(mModel);
    }
    detailsModel = !detailsModel;
}
