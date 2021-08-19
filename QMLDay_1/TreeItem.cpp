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
    if (item->mLevel == 1) {
        item->mTab = item->ItemData()[1].toString();
    } else {
        item->mTab = this->mTab;
    }
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
    //if (mLevel == 1)
    //{
    //    //一级节点，班级
    //    if (column == 0)
    //    {
    //        CLASS* c = (CLASS*)mPtr;
    //        return c->name;
    //    }
    //}
    //else if (mLevel == 2)
    //{
    //    //二级节点学生信息
    //    STUDENT* s = (STUDENT*)mPtr;
    //    switch (column)
    //    {
    //    case 0: return s->name;
    //    case 1: return QString::number(s->score1);
    //    case 2: return QString::number(s->score2);
    //    case 3: return QString::number(s->score3);
    //    case 4: return QString::number(s->score1 + s->score2 + s->score3);
    //    case 5: return QString::number((s->score1 + s->score2 + s->score3) / 3);
    //    case 6:
    //    {
    //        if (s->score1 < 60 || s->score2 < 60 || s->score3 < 60)
    //        {
    //            //任意一门课不合格则不合格
    //            return QStringLiteral("不合格");
    //        }
    //        else
    //        {
    //            return QStringLiteral("合格");
    //        }
    //    }
    //    case 7:
    //    {
    //        if (s->score1 >= 90 && s->score2 >= 90 && s->score3 >= 90)
    //        {
    //            //每门课都达到90分以上评优
    //            return QStringLiteral("优秀");
    //        }
    //        else
    //        {
    //            return QStringLiteral("-");
    //        }
    //    }
    //    default:
    //        return QVariant();
    //    }
    //}
    return QVariant();
}

void TreeItem::setData(int column, QVariant value)
{
    if (column >= mItemData.size()) {
        return;
    }
    mItemData[column] = value;
}




