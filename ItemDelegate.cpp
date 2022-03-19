#include "ItemDelegate.h"
#include <QHeaderView>
#include "TreeItem.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <TreeModel.h>
#include <QDebug>
#include <QTextDocument>
#include <QLineEdit>

namespace {
    constexpr int IDENT = 20;
    const QColor COLOR_HOVER(250, 138, 56, 255);
    const QColor COLOR_SELECTED(248, 192, 80, 255);
    constexpr int LINE_HEIGHT = 30;
    constexpr qreal TRIANGLE_WIDTH = IDENT / 4.0;
    constexpr qreal PADDING = IDENT / 8.0;
    const QColor HIGHTLIGHT_TEXT_COLOR(255, 0, 0);
}
ItemDelegate::ItemDelegate(QTreeView *view, QObject * parent)
    : m_View(view), QStyledItemDelegate(parent)
{
    setQss();
    m_View->setIndentation(0);
//    m_View->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_View->setUniformRowHeights(false);
    connect(m_View->header(), &QHeaderView::sectionResized, this, [](int logicalIndex, int oldSize, int newSize){
        if (logicalIndex == 2) {
            qDebug() << newSize;
        }
    });
}

void ItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    const_cast<QAbstractItemModel*>(index.model())->setData(index, option.rect.width(), Qt::UserRole+1);
    QStyleOptionViewItem op = option;
    TreeItem *item = index.data(Qt::UserRole).value<TreeItem *>();
    if (index.column() == 1) {
        int level = item->level();
        int ident = level * IDENT;
        op.rect = op.rect.adjusted(ident, 0, 0, 0);
        if (op.state.testFlag(QStyle::State_MouseOver)) {
            painter->fillRect(option.rect, COLOR_HOVER);
        }
        if (op.state.testFlag(QStyle::State_Selected)) {
            painter->fillRect(option.rect, COLOR_SELECTED);
        }
        if (index.model()->rowCount(index.siblingAtColumn(0)) >  0) {
            DrawTriangle(painter, op, index);
        }
        DrawKeyword(painter, op, index);
        //QStyledItemDelegate::paint(painter, op, index);
    } else {
        //        if (index.column() == 2) {
        //            QTextDocument doc;
        //            painter->save();
        //            doc.setHtml(item->ItemData()[index.column()].toString());
        //            doc.setTextWidth(option.rect.width());
        //            painter->translate(option.rect.x(), option.rect.y());
        //            doc.drawContents(painter);
        //            painter->restore();
        //        } else
        if (index.column() == 3) {
            m_View->openPersistentEditor(index);
            QStyledItemDelegate::paint(painter, op, index);
        } else {
            QStyledItemDelegate::paint(painter, op, index);
        }

    }
    painter->drawRect(option.rect);
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(index)
//    return QSize(option.rect.width(), LINE_HEIGHT);
//    if (m_View->header()->sectionSize(2) < 80) {
//        qDebug() << "thisxxxx";
//        return QStyledItemDelegate::sizeHint(option, index);
//    }
    if (m_View->currentIndex() == index) {
        return QStyledItemDelegate::sizeHint(option, index);
    }
    return QSize (QStyledItemDelegate::sizeHint(option, index).width(),LINE_HEIGHT);
}

bool ItemDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
    Q_UNUSED(model)
    if (index.column() != 1) {
        return false;
    }
    TreeItem *item = index.data(Qt::UserRole).value<TreeItem *>();
    int ident = item->level() * IDENT;
    QRect hotArea(option.rect.x(), option.rect.y(), ident, option.rect.height());
    QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);
    if (mEvent->type() == QEvent::MouseButtonPress && hotArea.contains(mEvent->pos())) {
        m_View->setExpanded(index.siblingAtColumn(0), !m_View->isExpanded(index.siblingAtColumn(0)));
        return true;
    }
    return false;
}

QWidget * ItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option)
    if (index.column() != 3) {
        QLineEdit *le = new QLineEdit(parent);
        connect(le, &QLineEdit::returnPressed, this, [le, index](){
            TreeItem *item = index.data(Qt::UserRole).value<TreeItem *>();
            item->setData(index.column(), QVariant(le->text()));
        });
        return le;
    }
    else {
    QPushButton *b = new QPushButton("777", parent);
    connect(b, &QPushButton::clicked, this, [b,this, option, index]() {
        for (int i = 0; i <= index.column(); i++) {
            // 提醒界面数据刷新了，重新从sizehint获取size大小
            const_cast<QAbstractItemModel*>(index.model())->dataChanged(index, index);
            // 界面刷新
            m_View->doItemsLayout();
//            m_View->viewport()->update();
        }
    });
        return b;
    }
}

void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    return;
}

void ItemDelegate::DrawTriangle(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex & index) const
{
    //TreeItem *item = index.data(Qt::UserRole).value<TreeItem *>();
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(QBrush(Qt::black));
    QPointF triangle[3];
    QModelIndex idx = index.siblingAtColumn(0);
    if (m_View->isExpanded(idx)) {
        // ↓
        triangle[0] = QPointF(option.rect.x() - PADDING, option.rect.y() + option.rect.height() / 2.0 - PADDING);
        triangle[1] = QPointF(triangle[0].x() - TRIANGLE_WIDTH * 2.0, triangle[0].y());
        triangle[2] = QPointF((triangle[0].x() + triangle[1].x()) / 2.0, triangle[0].y() + TRIANGLE_WIDTH);
    }
    else {
        // →
        triangle[0] = QPointF(option.rect.x() - PADDING, option.rect.y() + option.rect.height() / 2.0);
        triangle[1] = QPointF(triangle[0].x() - TRIANGLE_WIDTH, triangle[0].y() - TRIANGLE_WIDTH);
        triangle[2] = QPointF(triangle[1].x(), triangle[0].y() + TRIANGLE_WIDTH);
    }
    painter->drawPolygon(triangle, 3);
    painter->restore();
}

void ItemDelegate::setQss()
{
    m_View->header()->setDefaultAlignment(Qt::AlignCenter);
    QString qss = QString("QHeaderView::section { \
                    height:20px; \
                    border:0.5px solid rgb(210, 210, 210); \
                    background-color:qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 white, stop:1 rgba(239, 239, 239, 255)) \
                    } \
                    QTreeView{ \
                    outline:0px; \
                    } \
                    QTreeView::item:hover,QTreeView::branch:hover{ \
                    background:rgba(%1,%2,%3,%4); \
                    } \
                    QTreeView::item:selected,QTreeView::branch:selected{ \
                    background:rgba(%5,%6,%7,%8); \
                    }").arg(QString::number(COLOR_HOVER.red()), QString::number(COLOR_HOVER.green()), QString::number(COLOR_HOVER.blue()), QString::number(COLOR_HOVER.alpha()),
    QString::number(COLOR_SELECTED.red()), QString::number(COLOR_SELECTED.green()), QString::number(COLOR_SELECTED.blue()),QString::number(COLOR_SELECTED.alpha()));
    m_View->setStyleSheet(qss);
}

void ItemDelegate::DrawKeyword(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    TreeItem *item = index.data(Qt::UserRole).value<TreeItem *>();
    QStringList list = SpliteWord(item->ItemData()[index.column()].toString());
    QPen p = painter->pen();
    QFontMetrics fm = painter->fontMetrics();
    int ident = PADDING;
    for (int i = 0; i < list.size(); i++) {
        painter->save();
        if (list[i] == mMatchWord) {
            p.setColor(HIGHTLIGHT_TEXT_COLOR);
            painter->setPen(p);
            if (ident > option.rect.width()) {
                painter->restore();
                return;
            }
            QRect rc = option.rect.adjusted(ident, 0, 0, 0);
            painter->drawText(option.rect.adjusted(ident, 0, 0, 0), Qt::AlignVCenter, list[i]);
            ident += fm.width(list[i]);
        } else {
            if (option.state.testFlag(QStyle::State_Selected)) {
                p.setColor(option.palette.highlightedText().color());
                painter->setPen(p);
            } else {
                p.setColor(option.palette.text().color());
                painter->setPen(p);
            }
            if (ident > option.rect.width()) {
                painter->restore();
                return;
            }
            painter->drawText(option.rect.adjusted(ident, 0, 0, 0), Qt::AlignVCenter, list[i]);
            ident += fm.width(list[i]);
        }
        painter->restore();
    }
}

QStringList ItemDelegate::SpliteWord(QString originWord) const
{
    QStringList list;
    list = originWord.split(mMatchWord);
    if (list.size() == 2) {
        if (list[0] == "") {
            list.insert(0, mMatchWord);
        } else if (list[1] == "") {
            list.append(mMatchWord);
        } else {
            list.insert(1, mMatchWord);
        }
    }
    return list;
}
