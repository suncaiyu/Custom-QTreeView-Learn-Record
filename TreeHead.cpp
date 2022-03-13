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
