#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H

#include <QTreeView>
#include <QHeaderView>
#include "TreeHead.h"

class MyTreeView : public QTreeView
{
    Q_OBJECT
public:
    MyTreeView(QWidget *parent = nullptr);
    virtual ~MyTreeView() {};

private:
    std::unique_ptr<TreeHead> mHeader;
};

#endif // MYTREEVIEW_H
