#ifndef CREATEMODEL_H
#define CREATEMODEL_H
#include <QObject>
#include "TreeModel.h"
#include "ItemDelegate.h"
#include <QDebug>
class MyFunction : public QObject {
    Q_OBJECT
public:
    MyFunction(){};
public slots:
    void callbackFunc() {
        qDebug() << "from c++";
    }
};
class CreateModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int num READ num WRITE setNum)
    Q_PROPERTY(TreeModel* model READ model WRITE setModel NOTIFY modelChanged)
public:
    CreateModel();
    void initModel();
    Q_INVOKABLE QString getHeaderString(int column);
    Q_INVOKABLE int getHeaderSize() { return mModel->headerSize();}
    void setNum(int n) {
        i = n;
    }

    int num(){return i;}
    TreeModel *model() {
        return this->mModel;
    }
    void setModel(TreeModel *m) {
        mModel = m;
    }

    TreeModel *mModel;
    int i = 66;

signals:
    void modelChanged();
};
#endif // CREATEMODEL_H
