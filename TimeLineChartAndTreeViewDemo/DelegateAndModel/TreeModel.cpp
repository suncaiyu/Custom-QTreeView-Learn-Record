#include "TreeModel.h"
#include <qdebug.h>
#include <QTreeView>
#include "TreeItem.h"
#include "TreeModel.h"
#include <QColor>
#include <QDateTime>
#include <QStringList>
#include <QDebug>
#include <QSize>

TreeModel::TreeModel(QStringList headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    mHeaders = headers;
    mRootItem = new TreeItem;
    treeView = qobject_cast<QTreeView *>(parent);
}

TreeModel::~TreeModel()
{
    delete mRootItem;
}

TreeItem *TreeModel::itemFromIndex(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return nullptr;
    }
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item;
}

QModelIndex TreeModel::indexFromItem(TreeItem *item)
{
    TreeItem *tmp = item;
    QVector<TreeItem *>itemVector;
    while (tmp->parentItem() != nullptr) {
        itemVector.push_back(item->parentItem());
        tmp = tmp->parentItem();
    }
    itemVector.push_back(item);
    QModelIndex tmpIndex;
    itemVector.erase(itemVector.begin());
    for (int i = 0; i < itemVector.size(); ++i) {
        tmpIndex = index(itemVector[i]->row(), 0, tmpIndex);
    }
    return tmpIndex;
}

TreeItem *TreeModel::root()
{
    return mRootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            return mHeaders.at(section);
        }
    }
    return QVariant();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
    if (role == Qt::UserRole) {
        return QVariant::fromValue<TreeItem *>(item);
    } else
        if (Qt::SizeHintRole == role)
        {
            return QSize(0, 30);
        }
        else if (role == Qt::DisplayRole) {
            return item->data(index.column());
        }
    return QVariant();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        item->setData(index.column(), value);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }
    //节点是否允许编辑
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags |= Qt::ItemIsEditable;

    return flags;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }
    TreeItem *parentItem;

    if (!parent.isValid()) {
        parentItem = mRootItem;
    }
    else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }
    TreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }
    else {
        return QModelIndex();
    }
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }
    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == mRootItem) {
        return QModelIndex();
    }
    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0) {
        return 0;
    }
    if (!parent.isValid()) {
        parentItem = mRootItem;
    }
    else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }
    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return mHeaders.size();
}
