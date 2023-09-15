#include "TreeHead.h"
#include <QPainter>
TreeHead::TreeHead(Qt::Orientation orientation, QWidget *parent):QHeaderView(orientation,parent)
{
    setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setStretchLastSection(true);
}

TreeHead::~TreeHead()
{

}

void TreeHead::paintEvent(QPaintEvent *e)
{
    QHeaderView::paintEvent(e);
}

void TreeHead::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
//    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->drawText(rect, Qt::AlignCenter, "MyHeaderView");
}

QRect CheckBoxStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    QRect tmp = QProxyStyle::subElementRect(element, option, widget);
       if (element == QStyle::SE_ItemViewItemCheckIndicator/* || element == QStyle::SE_CheckBoxIndicator || QStyle::SE_CheckBoxContents == element || element == QStyle::SE_CheckBoxFocusRect || element == QStyle::SE_CheckBoxClickRect || element == QStyle::SE_CheckBoxLayoutItem*/) {
           tmp.moveCenter(option->rect.center());
       }
       return tmp;
}
