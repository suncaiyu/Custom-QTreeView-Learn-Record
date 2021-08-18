#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>

// 节点是否允许编辑
#define TREE_ITEM_CAN_MODIFY    1


class TreeItem
{
public:
    explicit TreeItem(TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);      // 在本节点下增加子节点
    void removeChilds();                    // 清空所有节点

    TreeItem *child(int row);               // 获取第row个子节点指针
    TreeItem *parentItem();                 // 获取父节点指针
    int childCount() const;                 // 子节点计数
    int row() const;                        // 获取该节点是父节点的第几个子节点

    //核心函数：获取节点第column列的数据
    QVariant data(int column) const;

    //设置、获取节点是几级节点（就是树的层级）
    int level() { return mLevel; }

    //设置、获取节点存的数据指针
    void setItemData(QVector<QVariant> data) { mItemData = data; };
    QVector<QVariant> ItemData() { return mItemData; };

    //保存该节点是其父节点的第几个子节点，查询优化所用
    void setRow(int row) {
        mRow = row;
    }

    //修改节点内容(只对有数据的节点可修改，比如姓名、三门课，其他是根据这些内容自动求解的，会自动刷新，但是这里没法改)
    void setData(int column, QVariant value);

    QString mTab; // 记录第一层信息，搜索用

private:
    void setLevel(int level) { mLevel = level; }
    QList<TreeItem*> mChildItems;   //子节点
    TreeItem *mParentItem;          //父节点
    int mLevel;     //该节点是第几级节点
    int mRow;       //记录该item是第几个，可优化查询效率
    QVector<QVariant> mItemData;
};
Q_DECLARE_METATYPE(TreeItem *)
#endif
