#include "MyTreeView.h"
#include "TreeHead.h"
MyTreeView::MyTreeView(QWidget *parent):QTreeView(parent)
{
    mHeader = std::unique_ptr<TreeHead>(new TreeHead(Qt::Orientation::Horizontal, this));
    setHeader(mHeader.get());
}
