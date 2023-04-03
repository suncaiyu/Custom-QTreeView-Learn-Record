#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QColor>

// �ڵ��Ƿ������༭
#define TREE_ITEM_CAN_MODIFY    1


class TreeItem
{
public:
    explicit TreeItem(TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);
    void removeChilds();

    TreeItem *child(int row);
    TreeItem *parentItem();
    int childCount() const;
    int row() const;

    QVariant data(int column) const;


    int level() { return mLevel; }


    void setItemData(QVector<QVariant> data) { mItemData = data; };
    QVector<QVariant> ItemData() { return mItemData; };


    void setRow(int row) {
        mRow = row;
    }

    void setData(int column, QVariant value);

private:
    void setLevel(int level) { mLevel = level; }
    QList<TreeItem*> mChildItems;
    TreeItem *mParentItem;
    int mLevel;
    int mRow;
    QVector<QVariant> mItemData;
};
Q_DECLARE_METATYPE(TreeItem *)
#endif
