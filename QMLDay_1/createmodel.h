#ifndef CREATEMODEL_H
#define CREATEMODEL_H
#include <QObject>
#include "TreeModel.h"
#include "ItemDelegate.h"
class CreateModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int num READ num WRITE setNum)
    Q_PROPERTY(TreeModel* model READ model WRITE setModel)
public:
    CreateModel();
    Q_INVOKABLE void InitModel();

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
};
#endif // CREATEMODEL_H
