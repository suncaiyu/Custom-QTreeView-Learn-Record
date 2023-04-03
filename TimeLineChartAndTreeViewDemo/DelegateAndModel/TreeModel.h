#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "TreeItem.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QSortFilterProxyModel>
#include <QSize>
class QTreeView;
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QStringList headers, QObject *parent = 0);
    ~TreeModel();
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public:
    TreeItem *itemFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromItem(TreeItem *item);
    TreeItem *root();

private:
    QStringList mHeaders;   // 表头内容
    TreeItem *mRootItem;    // 根节点
    QTreeView *treeView = nullptr;
};
#endif
