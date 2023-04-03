#include "TreeDelegate.h"
#include <QLineEdit>
#include <QDebug>
#include <QPushButton>
#include <QPainter>
#include <QTreeView>
#include <QPainterPath>
#include <QMouseEvent>
#include "TreeItem.h"
#include <QPushButton>
#include <QApplication>
#include <QStyleOptionButton>
#include <QScroller>
#include <QEasingCurve>

TreeDelegate::TreeDelegate(QObject *parent, long long max) : QStyledItemDelegate(parent), maxValue(max)
{
    mTreeView = qobject_cast<QTreeView *>(parent);
    mTreeView->setColumnWidth(0, 150);
    mTreeView->setColumnWidth(1, 400);
    mTreeView->setColumnWidth(2, 80);
    mTreeView->viewport()->setMouseTracking(1);
    mTreeView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mTreeView->setMouseTracking(1);

    QScroller *scl = QScroller::scroller(mTreeView);
    QScrollerProperties properties = scl->scrollerProperties();

    properties.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor,1);//设置平滑滑动速度，当滑动后手离开屏幕后,进行平滑滑动的速度,此值应介于0和1之间。值越小，速度越慢。??实际没区别
    properties.setScrollMetric(QScrollerProperties::FrameRate,QScrollerProperties::Fps60);//设置滚动过程中画面的刷新率，60帧看着舒服，帧率越低画面越跳动
    properties.setScrollMetric(QScrollerProperties::DragStartDistance,0.03);//设置移动阀值(按下后需要移动最少多少米距离后,触发滑动),用来避免误操作
    properties.setScrollMetric(QScrollerProperties::DecelerationFactor,0.5);//设置减速因子,值越大，减速越快，进而会影响点击释放后滚动的距离。对于大多数类型，该值应在0.1到2.0的范围内
    properties.setScrollMetric(QScrollerProperties::AxisLockThreshold,0.5);//设置当运动方向与某一个轴的角度小于该设定值（0~1）时，则限定只有该轴方向的滚动
    properties.setScrollMetric(QScrollerProperties::ScrollingCurve,QEasingCurve::OutQuad);//设置当鼠标释放后自动滚动到停止时的运动曲线，参数为QEasingCurve类型，不能设置为QEasingCurve::Type类型，不会隐式转换
    properties.setScrollMetric(QScrollerProperties::MaximumClickThroughVelocity,0.5);//自动滚动过程中，鼠标点击操作会停止当前滚动，当速度大于该设定（m/s）时，鼠标事件不会传递给目标即不会停止滚动
    properties.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime ,2);//与AcceleratingFlickSpeedupFactor配合使用。设置一个时间和加速因子，开始自动滚动后，如果在该时间（s）内检测到轻滑加速手势，则滚动速度加速到：当前速度x加速因子，加速因子必须大于等于1.0。加速后的速度也不能超过QScrollerProperties::MaximumVelocity的设定值
    properties.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor,1);//与AcceleratingFlickMaximumTime配合使用，应>=1
    properties.setScrollMetric(QScrollerProperties::MinimumVelocity,0.1);//设置自动滑动的最小速度，m/s，如果手势的速度小于该速度，则不会触发自动滚动，所以可以设置得小一些
    properties.setScrollMetric(QScrollerProperties::MaximumVelocity,0.1);//设置自动滚动能达到得最大速度，m/s
    properties.setScrollMetric(QScrollerProperties::SnapTime,1.0);//设置滚动曲线的时间因子。设置滚动的时长,值越小，滚动时间越长
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy,QScrollerProperties::OvershootWhenScrollable);//设置垂直向允许过量拖拽的策略，可以设置滚动条出现时开启、始终关闭、始终开启三种策略
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor,0.5);//设置过量拖拽的移动距离与鼠标移动距离的比例，0~1，值越小表现出的阻塞感越强
    properties.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor,0.5);//设置过量拖拽的距离占页面的比例，0~1，比如设置0.5，过量拖拽垂直最多移动高度的一半
    properties.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor,0.1);//设置自动滚动时允许的过量滚动距离比例，类似于OvershootDragDistanceFactor
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime,0.3);//设置当拖拽过量时，释放后位置恢复所用时间（s）
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay,1);//设置鼠标事件延迟时间，单位s。当鼠标按下后，开始手势识别，如果在该时间内开始了拖拽手势，窗口不会收到鼠标事件，相当于取消了点击事件，和滑屏关系不大

    scl->grabGesture(mTreeView,QScroller::LeftMouseButtonGesture);
//    scl->setScrollerProperties(properties);
}

TreeDelegate::~TreeDelegate()
{

}

void TreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFont font = painter->font();
    font.setFamily("Microsoft YaHei");
    painter->setFont(font);
    if (option.features.testFlag(QStyleOptionViewItem::Alternate)) {
        QRect background = option.rect;
        if (index.column() == 0) {
            background.setX(0);
        }
        painter->fillRect(background, option.palette.brush(QPalette::Current,QPalette::AlternateBase));
    } else {
        QRect background = option.rect;
        if (index.column() == 0) {
            background.setX(0);
        }
        painter->fillRect(background, option.palette.brush(QPalette::Current,QPalette::Window));
    }
    if (option.state.testFlag(QStyle::State_Selected)) {
        QRect background = option.rect;
        if (index.column() == 0) {
            background.setX(0);
        }
        painter->fillRect(background, "#46b2b5");
        painter->save();
        QPen pen = painter->pen();
        pen.setColor(QColor("#46b2b5").darker(150));
        painter->setPen(pen);
        QRect borderRect = background.adjusted(0, 0, 0, -1);
        if (index.column() == 0) {
            QPainterPath path;
            path.addRect(borderRect);
            painter->drawLine(path.elementAt(0), path.elementAt(1));
            painter->drawLine(path.elementAt(2), path.elementAt(3));
            painter->drawLine(path.elementAt(4), path.elementAt(3));
        } else {
            QPainterPath path;
            path.addRect(borderRect);
            painter->drawLine(path.elementAt(0), path.elementAt(1));
            painter->drawLine(path.elementAt(2), path.elementAt(3));
        }
//        painter->drawRect(background.adjusted(0, 0, 0, -1));
        painter->restore();
    } else if (option.state.testFlag(QStyle::State_MouseOver)) {
        QRect background = option.rect;
        if (index.column() == 0) {
            background.setX(0);
        }
        painter->fillRect(background, "#8fd5d5");
        painter->save();
        QPen pen = painter->pen();
        pen.setColor(QColor("#8fd5d5").darker(150));
        painter->setPen(pen);
        QRect borderRect = background.adjusted(0, 0, 0, -1);
        if (index.column() == 0) {
            QPainterPath path;
            path.addRect(borderRect);
            painter->drawLine(path.elementAt(0), path.elementAt(1));
            painter->drawLine(path.elementAt(2), path.elementAt(3));
            painter->drawLine(path.elementAt(4), path.elementAt(3));
        } else if (index.column() == 1){
            QPainterPath path;
            path.addRect(borderRect);
            painter->drawLine(path.elementAt(0), path.elementAt(1));
            painter->drawLine(path.elementAt(2), path.elementAt(3));
        }/* else {
            QStyleOptionButton btnop;
            btnop.text = "查看";
            btnop.rect = option.rect;
            QApplication::style()->drawControl(QStyle::CE_PushButton, &btnop, painter);
        }*/
        painter->restore();
    }

    if (index.column() == 0) {
        painter->drawText(option.rect.adjusted(5, 0, 0, 0), Qt::AlignVCenter, index.data().toString());
    }
    if (index.column() == 1) {
        bool ok;
        long long currentValue = index.data().toLongLong(&ok);
        if (ok) {
            qreal percent = (qreal)currentValue / (qreal)maxValue;
            int width = option.rect.width();
            int process = width * percent;
            if (process < 1 && process != 0) {
                process = 1;
            }
            QRect processRect = option.rect;
            processRect.setWidth(process);
            processRect.adjust(0, 5, 0, -5);
            painter->fillRect(processRect, "#eea36b");
            painter->save();
            QPen pen = painter->pen();
            pen.setColor("#f3906b");
            painter->setPen(pen);
            painter->drawRect(processRect);
            painter->restore();
            float ppp = currentValue / std::pow(10, 9);
            painter->drawText(option.rect, Qt::AlignVCenter, QString::number(ppp));
        }
    }
    if (index.column() == 2){
        mTreeView->openPersistentEditor(index);
    }
    DrawBranchIcon(painter, option, index);
}

QSize TreeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize sz = QStyledItemDelegate::sizeHint(option, index);
    return sz;
}

QWidget *TreeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    if (index.column() == 2) {
    QPushButton *b = new QPushButton("查看", parent);
//    connect(b, &QPushButton::clicked, this, [b,this, option, index]() {
//        for (int i = 0; i <= index.column(); i++) {
//            // 提醒界面数据刷新了，重新从sizehint获取size大小
//            const_cast<QAbstractItemModel*>(index.model())->dataChanged(index, index);
//            // 界面刷新
//            m_View->doItemsLayout();
////            m_View->viewport()->update();
//        }
//    });
        return b;
    }
}

bool TreeDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QStyleOptionViewItem op = option;
    return QStyledItemDelegate::editorEvent(event, model, op, index);
}

void TreeDelegate::DrawBranchIcon(QPainter *p, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    p->save();
    p->setRenderHint(QPainter::Antialiasing);
    QPen pen = p->pen();
    pen.setWidthF(2);
    p->setPen(pen);
    TreeItem *item = index.data(Qt::UserRole).value<TreeItem *>();
    int level = item->level();
    int indent = mTreeView->indentation();
    QRect rc(indent * (level - 1), option.rect.y(), indent, option.rect.height());
    QPainterPath path;
    int leftPadding = rc.width() / 5;
    int topPadding = rc.height() / 12;
    if (index.column() == 0 && item->childCount() > 0) {
        if (mTreeView->isExpanded(index)) {
            path.moveTo(rc.center().x() - leftPadding, rc.center().y() - topPadding);
            path.lineTo(rc.center().x(), rc.center().y() + topPadding);
            path.lineTo(rc.center().x() + leftPadding, rc.center().y() - topPadding);
        } else {
            path.moveTo(rc.center().x() - topPadding, rc.center().y() - leftPadding);
            path.lineTo(rc.center().x() + topPadding, rc.center().y());
            path.lineTo(rc.center().x() - topPadding, rc.center().y() + leftPadding);

        }
        p->drawPath(path);
    }
    p->restore();
}

