#include "MyHeaderView.h"
#include <QPainter>
#include <QDebug>
#include <QFontDatabase>
MyHeaderView::MyHeaderView(Qt::Orientation orientation, QWidget *parent) : QHeaderView(orientation, parent)
{
    setStretchLastSection(true);
}

MyHeaderView::~MyHeaderView()
{

}

void MyHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    QLinearGradient linearGradient;
    linearGradient.setStart(0, 0);
    linearGradient.setFinalStop(0, rect.height());
    linearGradient.setColorAt(1.0, QColor(237, 237, 237));
    linearGradient.setColorAt(0.0, Qt::white);
    painter->fillRect(rect, linearGradient);
    QString title = this->model()->headerData(logicalIndex, Qt::Orientation::Horizontal).toString();
    QFont font = painter->font();
    font.setFamily(QStringLiteral( "Terminal"));
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, title);
    QPen pen = painter->pen();
    pen.setColor(QColor(184, 184, 184));
    painter->setPen(pen);
//    qDebug() << painter->font();
    if (logicalIndex == 0) {
        painter->drawRect(rect.adjusted(-1, -1, 0, -1));
    } else {
        painter->drawRect(rect.adjusted(0, -1, 0, -1));
    }
}
