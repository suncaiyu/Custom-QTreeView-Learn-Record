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
#include <QApplication>
#include <QCheckBox>


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
//    setQss();
//    m_View->setIndentation(0);
//    m_View->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_View->setUniformRowHeights(false);
    m_View->setWordWrap(true);
    connect(m_View->header(), &QHeaderView::sectionResized, this, [this](int logicalIndex, int oldSize, int newSize){
//        m_View->itemDelegate()->sizeHintChanged(QModelIndex());
         m_View->doItemsLayout();
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
        if (index.column() == 2) {
            painter->drawText(op.rect.adjusted(5, 0, 0, -5), Qt::TextWrapAnywhere | Qt::AlignVCenter, index.data(Qt::DisplayRole).value<QString>());
        } else
        if (index.column() == 3) {
            if (item->data(index.column()).canConvert<QCheckBox *>()) {
                QCheckBox *checkBox = qvariant_cast<QCheckBox*>(item->data(index.column()));
                QStyleOptionButton checkBoxOption;
                checkBoxOption.state |= checkBox->isChecked() ? QStyle::State_On : QStyle::State_Off;
                if (checkBox->isDown()) {
                    checkBoxOption.state |= QStyle::State_MouseOver;
                }
                checkBoxOption.rect = option.rect;
                int xOffset = (option.rect.width() - QApplication::style()->pixelMetric(QStyle::PM_IndicatorWidth)) / 2;
                checkBoxOption.rect.adjust(xOffset, 0, xOffset, 0);
                QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkBoxOption, painter);
            }
//            m_View->openPersistentEditor(index);
//            QStyledItemDelegate::paint(painter, op, index);
        } else {
            QStyledItemDelegate::paint(painter, op, index);
        }

    }
    QModelIndex id = index;
    int count = 1;
    if (index.column() == 0) {
        while (id.parent().isValid()) {
            id = id.parent();
            count++;
        }
        op.rect.setX(op.rect.x() - m_View->indentation() * count);
        painter->drawRect(op.rect);
    } else {
        painter->drawRect(option.rect);
    }
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(index)
//    return QSize(option.rect.width(), LINE_HEIGHT);
//    if (m_View->header()->sectionSize(2) < 80) {
//        qDebug() << "thisxxxx";
//        return QStyledItemDelegate::sizeHint(option, index);
//    }
//    if (m_View->currentIndex() == index) {
//        return QStyledItemDelegate::sizeHint(option, index);
//    }
//    return QSize (QStyledItemDelegate::sizeHint(option, index).width(),LINE_HEIGHT);
    QString str = index.data(Qt::DisplayRole).toString();
    QFont font = index.data(Qt::FontRole).value<QFont>();
    QFontMetrics fm(font);
    int wordWidth = fm.width(str);
    int lineCount = wordWidth / (m_View->header()->sectionSize(index.column()) - 10);
    if (wordWidth % (m_View->header()->sectionSize(index.column()) - 10) > 0) {
        lineCount++;
    }
    QSize sz = QStyledItemDelegate::sizeHint(option, index);
    sz.setHeight(fm.height() * lineCount + fm.lineSpacing() * (lineCount + 1));
    sz.setWidth(m_View->header()->sectionSize(index.column()));
    return sz;
}

bool ItemDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
    Q_UNUSED(model)
    //    if (index.column() != 1) {
    //        return false;
    //    }
    TreeItem *item = index.data(Qt::UserRole).value<TreeItem *>();
    QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);
    if (index.column() == 1) {
        int ident = item->level() * IDENT;
        QRect hotArea(option.rect.x(), option.rect.y(), ident, option.rect.height());
        if (mEvent->type() == QEvent::MouseButtonPress && hotArea.contains(mEvent->pos())) {
            m_View->setExpanded(index.siblingAtColumn(0), !m_View->isExpanded(index.siblingAtColumn(0)));
            return true;
        }
    }
    if (item->data(index.column()).canConvert<QCheckBox *>()) {
        QCheckBox *checkBox = qvariant_cast<QCheckBox*>(item->data(index.column()));
        QRect checkBoxRect = option.rect;
        int xOffset = (option.rect.width() - QApplication::style()->pixelMetric(QStyle::PM_IndicatorWidth)) / 2;
        int yOffset = (option.rect.height() - QApplication::style()->pixelMetric(QStyle::PM_IndicatorHeight)) / 2;
        checkBoxRect.adjust(xOffset, yOffset, xOffset, yOffset);
        checkBoxRect = QRect(checkBoxRect.x(), checkBoxRect.y(), checkBoxRect.height(), checkBoxRect.height());
        if (event->type() == QEvent::MouseButtonRelease && checkBoxRect.contains(mEvent->pos())) {
            checkBox->setChecked(!checkBox->isChecked());
            return true;
        }
        if (event->type() == QEvent::MouseMove && checkBoxRect.contains(mEvent->pos())) {
            checkBox->setDown(true);
            return true;
        }
        if (event->type() == QEvent::MouseMove && !checkBoxRect.contains(mEvent->pos())) {
            checkBox->setDown(false);
            return true;
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QWidget * ItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(option)
    if (index.column() != 3) {
        QLineEdit *le = new QLineEdit(parent);
        return le;
    }
    else {
    QPushButton *b = new QPushButton("777", parent);
    connect(b, &QPushButton::clicked, this, [b,this, option, index]() {
        for (int i = 0; i <= index.column(); i++) {
            // ���ѽ�������ˢ���ˣ����´�sizehint��ȡsize��С
            const_cast<QAbstractItemModel*>(index.model())->dataChanged(index, index);
            // ����ˢ��
            m_View->doItemsLayout();
//            m_View->viewport()->update();
        }
    });
        return b;
    }
}

void ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() != 3) {
        TreeItem *item = index.data(Qt::UserRole).value<TreeItem *>();
        item->setData(index.column(), QVariant(qobject_cast<QLineEdit *>(editor)->text()));
    }
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
        // ��
        triangle[0] = QPointF(option.rect.x() - PADDING, option.rect.y() + option.rect.height() / 2.0 - PADDING);
        triangle[1] = QPointF(triangle[0].x() - TRIANGLE_WIDTH * 2.0, triangle[0].y());
        triangle[2] = QPointF((triangle[0].x() + triangle[1].x()) / 2.0, triangle[0].y() + TRIANGLE_WIDTH);
    }
    else {
        // ��
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
                    } \
                    QTreeView::branch:closed:has-children{undefined \
                    image: url(:/res/minus.png); \
                    } \
                    QTreeView::branch:open:has-children{undefined \
                    image: url(:/res/minus.png); \
                    }").arg(QString::number(COLOR_HOVER.red()), QString::number(COLOR_HOVER.green()), QString::number(COLOR_HOVER.blue()), QString::number(COLOR_HOVER.alpha()),
    QString::number(COLOR_SELECTED.red()), QString::number(COLOR_SELECTED.green()), QString::number(COLOR_SELECTED.blue()),QString::number(COLOR_SELECTED.alpha()));
//    m_View->setStyleSheet(qss);
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
            ident += fm.horizontalAdvance(list[i]);
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
            ident += fm.horizontalAdvance(list[i]);
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
