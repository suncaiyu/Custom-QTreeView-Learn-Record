#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include "TreeItem.h"
#include "TreeModel.h"
#include "ItemDelegate.h"
#include <TreeHead.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    DeletePtr();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    DeletePtr();
    QStringList headList;
    headList << "ID" << "API" << "DUR(us)" << "Button";
    mModel = new TreeModel(headList, ui->treeView);
    TreeItem *root = mModel->root();
    // 1
    for (int i = 0; i < 10; i++) {
        TreeItem *item1 = new TreeItem(root);
        QVector<QVariant> data1;
        data1 << i << "api" + QString::number(i) << "firstapiapiapiapiapiapiapi" + QString::number(i) << "";
        item1->setItemData(data1);
        root->appendChild(item1);

        for (int j = 0; j < 10; j++) {
            TreeItem *item2 = new TreeItem(item1);
            QVector<QVariant> data2;
            data2 << j << "myapi" + QString::number(i + j) << "secondapiapiapiapiapiapiapi" + QString::number(i + j) << "";
            item2->setItemData(data2);
            item1->appendChild(item2);
            if (selected == nullptr && j == 8) {
                selected = item2;
            }
            for (int k = 0; k < 10; k++) {
                TreeItem *item3 = new TreeItem(item2);
                QVector<QVariant> data3;
                data3 << k << "myapi" + QString::number(i + j) << "thirdapiapiapiapiapiapiapi" + QString::number(i + j + k) << "";
                item3->setItemData(data3);
                item2->appendChild(item3);
            }
        }
    }
    mFilterModel = new MyFilterModel(ui->treeView);
    mFilterModel->setSourceModel(mModel);
    mFilterModel->setFilterKeyColumn(1);
    ui->treeView_2->setModel(mFilterModel);
    ui->treeView->setModel(mModel);
    mDelegate = new ItemDelegate(ui->treeView);
    mFilterDelegate = new ItemDelegate(ui->treeView_2);
    ui->treeView->setItemDelegate(mDelegate);
    ui->treeView_2->setItemDelegate(mFilterDelegate);
    ui->treeView_2->setWordWrap(true);
}


void MainWindow::on_pushButton_2_clicked()
{
    QModelIndex idx = mModel->indexFromItem(selected);
    TreeItem *it = idx.data(Qt::UserRole).value<TreeItem *>();
    ui->treeView->setCurrentIndex(idx);
}

void MainWindow::on_lineEdit_returnPressed()
{
    QString text = ui->lineEdit->text();
    mFilterModel->setFilterRegExp(text);
    // 这里因为模拟设了持久化代理显示按钮
    // 如果不进行下面的操作的话，createEditor不更新
    // 给按钮绑定的index就没更新，所以会导致index越界，找的不准等问题
    //reset 会重置所有状态 ，感觉不太好F
//    ui->treeView_2->reset();
    ui->treeView_2->doItemsLayout();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if (mDelegate == nullptr) {
        return;
    }
    mDelegate->setMatchWord(arg1);
    ui->treeView->viewport()->update();
}

void MainWindow::DeletePtr()
{
    if (mModel != nullptr) {
        delete mModel;
        mModel = nullptr;
    }
    if (mDelegate != nullptr) {
        delete mDelegate;
        mDelegate = nullptr;
    }
    if (mFilterDelegate != nullptr) {
        delete mFilterDelegate;
        mFilterDelegate = nullptr;
    }
    if (mFilterModel != nullptr) {
        delete mFilterModel;
        mFilterModel = nullptr;
    }
}
