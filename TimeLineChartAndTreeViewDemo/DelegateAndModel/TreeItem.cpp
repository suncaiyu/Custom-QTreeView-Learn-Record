#include "TreeItem.h"

#include <QStringList>
#include <QDateTime>
#include "TreeItem.h"

TreeItem::TreeItem(TreeItem *parent)
{
    mParentItem = parent;
    mLevel = 0;
    mRow = 0;
}

TreeItem::~TreeItem()
{
    removeChilds();
}

void TreeItem::appendChild(TreeItem *item)
{
    item->setRow(mChildItems.size());   //item存自己是第几个，可以优化效率
    mChildItems.append(item);
    item->setLevel(mLevel + 1);
}

void TreeItem::removeChilds()
{
    qDeleteAll(mChildItems);
    mChildItems.clear();
}

TreeItem *TreeItem::child(int row)
{
    return mChildItems.value(row);
}

TreeItem *TreeItem::parentItem()
{
    return mParentItem;
}

int TreeItem::childCount() const
{
    return mChildItems.count();
}

int TreeItem::row() const
{
    return mRow;
}

QVariant TreeItem::data(int column) const
{
    if (column < mItemData.size()) {
        return mItemData[column];
    }
    return QVariant();
}

void TreeItem::setData(int column, QVariant value)
{
    if (column >= mItemData.size()) {
        return;
    }
    mItemData[column] = value;
}
