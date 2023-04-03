#include "ChartWidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QPainterPath>
#include <qdebug.h>
#include <qpalette.h>
#include "JsonParser.h"
#include <QMimeData>
#include <QMimeDatabase>
#include <QDir>

ChartWidget::ChartWidget(TimelineContext &ctx, QWidget *parent) : QWidget(parent), mContext(&ctx)
{
    ctx.AddUpdateWidget(this);
    setMouseTracking(true);
//    InitData();
    mScrollbar = new QScrollBar(this);
    connect(mScrollbar, &QScrollBar::valueChanged, [this](int value) {
        mPan = -value;
    });
    setAcceptDrops(true);
    //ParseJsonFile("C:\\Users\\Administrator\\Desktop\\host_06fb1bade340pid_117_time_2023_01_13_03_15_23_518049.paddle_trace.json");
//    ParseJsonFile("C:\\Users\\Administrator\\Desktop\\bert.pt.trace.json");

}

void ChartWidget::ParseJsonFile(QString path)
{
    JsonParser j;
    j.LoadFile(path);
    if (mRoot != nullptr) {
        delete mRoot;
    }
    mRoot = new ChartData;
    mRoot->mChartHeight = 0;
    j.InitChartData(mRoot);
}

void ChartWidget::ParseJsonContent(QByteArray content)
{
    JsonParser j;
    j.LoadContent(content);
    if (mRoot != nullptr) {
        delete mRoot;
    }
    mRoot = new ChartData;
    mRoot->mChartHeight = 0;
    j.InitChartData(mRoot);
}

ChartWidget::~ChartWidget()
{
    if (mRoot != nullptr) {
        delete mRoot;
    }
    if (mScrollbar != nullptr) {
        delete mScrollbar;
    }
    if (mHighLightTimeStamp != nullptr) {
        delete mHighLightTimeStamp;
    }
    if (mSelectMenu != nullptr) {
        qDeleteAll(mSelectMenu->actions());
        delete mSelectMenu;
    }
    if (mLinkLine != nullptr) {
        delete mLinkLine;
        mLinkLine = nullptr;
    }
}

void ChartWidget::InitData()
{
    mRoot = new ChartData();
    mRoot->mIsTitle = true;
    mRoot->mName = "Root";

    ChartData *sub1 = new ChartData();
    sub1->mIsTitle = true;
    sub1->mName = "RootSub1";
    sub1->mChartHeight = 50;
    mRoot->mChildrens.push_back(sub1);
    sub1->mLevel = mRoot->mLevel + 1;
    sub1->parent = mRoot;

    ChartData *sub2 = new ChartData();
    sub2->mIsTitle = true;
    sub2->mName = "RootSub2";
    sub2->mChartHeight = 50;
    mRoot->mChildrens.push_back(sub2);
    sub2->mLevel = mRoot->mLevel + 1;
    sub2->parent = mRoot;

    ChartData *sub3 = new ChartData();
    sub3->mIsTitle = true;
    sub3->mName = "RootSub3";
    sub3->mChartHeight = 50;
    mRoot->mChildrens.push_back(sub3);
    sub3->mLevel = mRoot->mLevel + 1;
    sub3->parent = mRoot;

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mName = "data" + QString::number(i);
        d->mChartHeight = 50;
        sub1->mChildrens.push_back(d);
        d->mLevel = sub1->mLevel + 1;
        d->parent = sub1;
        int start = 0;
        for (int j = 0; j < 1000; j++) {
            start += rand() % 1000;
            TimeStamp ts;
            ts.start = start;
            start += rand() % 1000;
            ts.end = start;
            d->mDatas.push_back(ts);
        }
    }

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mName = "data" + QString::number(i);
        d->mChartHeight = 50;
        sub2->mChildrens.push_back(d);
        d->mLevel = sub2->mLevel + 1;
        d->parent = sub2;
        Long start = 0;
        for (int j = 0; j < 1000; j++) {
            start += rand() % 1000;
            TimeStamp ts;
            ts.start = start;
            start += rand() % 1000;
            ts.end = start;
            d->mDatas.push_back(ts);
        }
    }

    ChartData *sub3sub = new ChartData;
    sub3sub->mIsTitle = true;
    sub3sub->mName = "Sub3Sub1";
    sub3sub->mChartHeight = 50;
    sub3->mChildrens.push_back(sub3sub);
    sub3sub->mLevel = sub3->mLevel + 1;
    sub3sub->parent = sub3;

    for (int i = 0; i < 3; i++) {
        ChartData *d = new ChartData;
        d->mName = "data" + QString::number(i);
        d->mChartHeight = 50;
        sub3sub->mChildrens.push_back(d);
        d->mLevel = sub3sub->mLevel + 1;
        d->parent = sub3sub;
        Long start = 0;
        for (int j = 0; j < 1000; j++) {
            start += rand() % 10000;
            TimeStamp ts;
            ts.start = start;
            start += rand() % 10000;
            ts.end = start;
            d->mDatas.push_back(ts);
        }
    }
}


void ChartWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    if (e->button() == Qt::LeftButton && e->modifiers() == Qt::NoModifier) {
        ChartData *tmp = GetChartData(e->pos(), mRoot, 0, width());
        if (tmp != nullptr && tmp->mExpandRect.contains(e->pos())) {
            tmp->mIsExpand = !tmp->mIsExpand;
            return;
        }
        if (mHighLightTimeStamp != nullptr) {
            if (mHighLightTimeStamp->rc.contains(e->pos())) {
                delete mHighLightTimeStamp;
                mHighLightTimeStamp = nullptr;
                return;
            }
        }
        if (tmp == nullptr) {
            return;
        }
        ShowedInfo tmpInfo;
        for (int i = 0; i < tmp->mShowedTimeStamp.size(); i++) {
            if (tmp->mShowedTimeStamp.at(i).rc.contains(e->pos())) {
                if (mHighLightTimeStamp != nullptr) {
                    delete mHighLightTimeStamp;
                }
//                mHighLightTimeStamp = new HighLight;
//                mHighLightTimeStamp->ts.end = tmp->mShowedTimeStamp.at(i).ts.end;
//                mHighLightTimeStamp->ts.start = tmp->mShowedTimeStamp.at(i).ts.start;
//                qDebug() << tmp->mShowedTimeStamp.at(i).ts.start << tmp->mShowedTimeStamp.at(i).ts.end;
//                return;
            }
        }
    }
#ifdef SWAP
    else if (e->button() == Qt::RightButton && e->modifiers() == Qt::NoModifier) {
        ChartData *tmp = GetChartData(e->pos(), mRoot, 0, mContext->mControlBarWidth);
        if (tmp == nullptr) {
            return;
        }
        tmp->SetDraging(true, e->y() - tmp->mRect.y());
        mIsDragingChart = tmp;
        mIsDragingChart->SetFloatingYLocation(e->y());
    }
#endif // DEBUG
    else if (e->button() == Qt::RightButton/* && e->modifiers() == Qt::ControlModifier*/) {
        if (mLinkLine != nullptr) {
            delete mLinkLine;
            mLinkLine = nullptr;
        }
        HideInfoWidgetSignal();
        ChartData *tmp = GetChartData(e->pos(), mRoot, 0, mContext->mControlBarWidth);
        if (tmp == nullptr) {
            return;
        }
        if (!tmp->mIsTitle) {
            return;
        }
        if (mSelectMenu != nullptr) {
            qDeleteAll(mSelectMenu->actions());
            delete mSelectMenu;
        }
        mSelectMenu = new QMenu(this);
        QAction *ac = new QAction("Combine Show", mSelectMenu);
        connect(ac, &QAction::triggered, this, [tmp](bool flag) {
            tmp->mCombineShow = flag;
        if (flag == false) {
            tmp->mConbineData.clear();
            }
        });
        ac->setCheckable(true);
        mSelectMenu->addAction(ac);
        if (tmp->mCombineShow) {
            ac->setChecked(true);
        }
//        for (int i = 0; i < tmp->mChildrens.size(); i++) {
//            ac = new QAction(tmp->mChildrens.at(i)->mName, mSelectMenu);
//            connect(ac, &QAction::triggered, this, [tmp, i](bool flag) {
//                tmp->mChildrens.at(i)->mIsVisible = flag;
//            });
//            ac->setCheckable(true);
//            if (tmp->mChildrens.at(i)->mIsVisible) {
//                ac->setChecked(true);
//            }

//            mSelectMenu->addAction(ac);
//        }
//        mSelectMenu->exec();
        qDebug() << "right button clicked!!!!";
        mSelectMenu->popup(QCursor::pos());
//        mSelectMenu->setGeometry(QRect(QCursor::pos().x(), QCursor::pos().y(), mSelectMenu->width(), mSelectMenu->height()));
//        mSelectMenu->show();
    }
    else if (e->button() == Qt::LeftButton && e->modifiers() == Qt::CTRL) {
        mContext->SetPressPoint(e->pos());
    }
    mContext->Update();
}

void ChartWidget::mouseMoveEvent(QMouseEvent *e)
{
    mContext->mShowTimelineLabel = true;
    mContext->mMouseOnX = e->x();
    if (e->modifiers() == Qt::CTRL && mContext->GetPressPoint() != QPoint()) {
        mContext->UpdateDistance(e->pos());
    }
#ifdef SWAP
    if (mIsDragingChart != nullptr) {
        int distance = e->y() - mIsDragingChart->GetFloatingYLocation();
        mIsDragingChart->SetFloatingYLocation(e->y());

        ChartData *parent = mIsDragingChart->parent;
        if (parent == nullptr) {
            return;
        }
        int index = parent->mChildrens.indexOf(mIsDragingChart);
        if (distance > 0) {
            // down
            if (mIsDragingChart == parent->mChildrens.last()) {
                return;
            }
            ChartData *tmp = parent->mChildrens.at(index + 1);
            if (mIsDragingChart->GetFloatingYLocation() > tmp->mRect.center().y()) {
                tmp->SetFloating(true);
                tmp->SetFloatingYLocation(tmp->mRect.y());
                parent->mChildrens.swapItemsAt(index, index + 1);
            }
        }
        else if (distance < 0) {
            // up
            if (mIsDragingChart == parent->mChildrens.first()) {
                return;
            }
            ChartData *tmp = parent->mChildrens.at(index - 1);
            if (mIsDragingChart->GetFloatingYLocation() < tmp->mRect.center().y()) {
                tmp->SetFloating(true);
                tmp->SetFloatingYLocation(tmp->mRect.y());
                parent->mChildrens.swapItemsAt(index, index - 1);
            }
        }
    }
#endif
    mContext->Update();
}

void ChartWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
    mContext->ClearPressPoint();
    if (mLinkLine != nullptr) {
        delete mLinkLine;
        mLinkLine = nullptr;
    }

    if (e->button() == Qt::LeftButton && e->modifiers() == Qt::NoModifier) {
        ChartData *tmp = GetChartData(e->pos(), mRoot, 0, width());
        if (tmp == nullptr) {
            return;
        }
        ShowedInfo tmpInfo;
        bool find = false;
        for (int i = 0; i < tmp->mShowedTimeStamp.size(); i++) {
            if (tmp->mShowedTimeStamp.at(i).rc.contains(e->pos())) {
                tmpInfo = tmp->mShowedTimeStamp.at(i);
                find = true;
                break;
            }
        }
        if (find) {
            ShowInfoWidgetSignal(tmpInfo.ts);
        // 传showedinfo，并且展示
        } else {
            // 发送hide
            HideInfoWidgetSignal();
        }
        qDebug() << tmpInfo.ts.correlationId << tmpInfo.ts.name << tmpInfo.ts.processName << tmpInfo.ts.threadName;
        if (tmpInfo.ts.correlationId == -1) {
            return;
        } else {
            for (int i = 0; i < mAllShowTs.size(); i++) {
                if (mAllShowTs.at(i).ts.correlationId == tmpInfo.ts.correlationId && mAllShowTs[i].rc != tmpInfo.rc) {
                    if (mLinkLine != nullptr) {
                        delete mLinkLine;
                    }
                    mLinkLine = new LinkLine;
                    mLinkLine->mPan = this->mPan;
                    if (tmpInfo.rc.center().y() > mAllShowTs[i].rc.center().y()) {
                        std::swap(mLinkLine->start, tmpInfo);
                        std::swap(mLinkLine->end, mAllShowTs[i]);
                    } else {
                        std::swap(mLinkLine->end, tmpInfo);
                        std::swap(mLinkLine->start, mAllShowTs[i]);
                    }
                    return;
                }
            }
        }
    }

#ifdef SWAP
    if (mIsDragingChart != nullptr) {
        mIsDragingChart->SetDraging(false);
        mIsDragingChart->SetFloating(true);
        mIsDragingChart = nullptr;
    }
#endif // SWAP
}

void ChartWidget::paintEvent(QPaintEvent *e)
{
    mAllShowTs.clear();
    QPainter p(this);
    p.fillRect(rect(), palette().brush(QPalette::Window));
    QFont font = p.font();
    p.setFont(font);
    QPen pen = p.pen();
    pen.setColor(mContext->mColor.GetBorderColor());
    p.setPen(pen);
    mChartHeight = 0;
    DrawChart(p, mChartHeight, mRoot);
    p.drawRect(rect().adjusted(0, 0, 0, -1));
    DrawLinkLine(p, rect());
#ifdef SWAP
    DrawDragingChart(p, mIsDragingChart);
#endif // SWAP
    DrawHighLight(p);
    p.end();
    UpdateScrollbar();
}

void ChartWidget::UpdateScrollbar()
{
    if (mChartHeight - height() > 0) {
        mScrollbar->setMaximum(mChartHeight - height());
        mScrollbar->setPageStep(height());
    } else {
        mScrollbar->setValue(0);
        mScrollbar->setMaximum(0);
        mScrollbar->setPageStep(height());
    }
}

void ChartWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    if (mScrollbar != nullptr) {
        mScrollbar->setGeometry(width() - 15, 0, 15, height());
    }
}

void ChartWidget::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);
    int delta = e->angleDelta().y();
    if (e->modifiers() == Qt::NoModifier) {
        if (delta < 0) {
            mScrollbar->setValue(mScrollbar->value() + 10);
        }
        else if (delta > 0) {
            mScrollbar->setValue(mScrollbar->value() - 10);
        }
    }
     else if (e->modifiers() == Qt::ALT) {
        ChartData *tmp = GetChartData(e->position().toPoint(), mRoot, 0, mContext->mControlBarWidth);
        if (tmp == nullptr) {
            return;
        }
        int delta = e->angleDelta().y();
        if (delta < 0) {
            tmp->mChartHeight++;
        }
        if (delta > 0) {
            tmp->mChartHeight--;
        }
    }
     else if (e->modifiers() == Qt::CTRL) {
        mContext->UpdateZoom(e->position().x(), delta);
    }
    mContext->Update();
}

void ChartWidget::DrawChart(QPainter &p, int &h, ChartData *data)
{
    if (data == nullptr) {
        return;
    }
    if (!data->mIsVisible) {
        data->mRect = QRect();
        return;
    }
    QRect tmpRect;
    tmpRect.setX(0);
    tmpRect.setY(h + mPan);
    tmpRect.setWidth(width());
    tmpRect.setHeight(data->mChartHeight);
    data->mRect = tmpRect;
    h += data->mChartHeight;
#ifdef SWAP
    DrawFloatingChart(p, data);
#endif // SWAP
    if ((tmpRect.bottom() > 0 || tmpRect.top() < height())
#ifdef SWAP
        && data->IsDraging() == false && data->IsFloating() == false
#endif
        ) {
        if (data->parent != nullptr && data->parent->mCombineShow) {
            DrawCombineChart(p, data);
        } else {
            p.fillRect(tmpRect, mContext->mColor.GetChartBackGround());
            p.drawRect(tmpRect);
            DrawData(p, data, tmpRect);
//            DrawLinkLine(p, tmpRect);
            DrawControlBar(p, data, tmpRect);
        }
        DrawExpandFlag(p, data, tmpRect);
    }
    if (!data->mIsExpand) {
        return;
    }
    for (int i = 0; i < data->mChildrens.size(); ++i) {
        DrawChart(p, h, data->mChildrens.at(i));
    }
}

void ChartWidget::DrawControlBar(QPainter &p, ChartData *data, QRect &rect)
{
    QRect labelRect(0, rect.y(), mContext->mControlBarWidth, data->mChartHeight);
    p.fillRect(labelRect, mContext->mColor.GetChartBackGround());
    p.drawRect(labelRect);
    p.save();
    QPen pen = p.pen();
    pen.setColor(mContext->mColor.GetChartTextColor());
    p.setPen(pen);
    labelRect = QRect((data->mLevel + 1) * data->mIndentUnits + data->mFlagWidth, rect.y(),
        mContext->mControlBarWidth - (data->mLevel + 1) * data->mIndentUnits - data->mFlagWidth, data->mChartHeight);
    p.drawText(labelRect, Qt::AlignVCenter, data->mName);
    p.restore();
}

void ChartWidget::DrawExpandFlag(QPainter &p, ChartData *data, QRect &rect)
{
    if (data->mChildrens.size() == 0) {
        data->mExpandRect = QRect();
        return;
    }
    int ident = data->mLevel * data->mIndentUnits;
    QPainterPath path;
    if (data->mIsExpand) {
        path.moveTo(ident, rect.y() + rect.height() / 2 - data->mFlagWidth / 4);
        path.lineTo(ident + data->mFlagWidth, rect.y() + rect.height() / 2 - data->mFlagWidth / 4);
        path.lineTo(ident + data->mFlagWidth / 2, rect.y() + rect.height() / 2 + data->mFlagWidth / 4);
        path.closeSubpath();
    }
    else {
        path.moveTo(ident + data->mFlagWidth / 2 - data->mFlagWidth / 4, rect.y() + rect.height() / 2 - data->mFlagWidth / 2);
        path.lineTo(ident + data->mFlagWidth / 2 - data->mFlagWidth / 4, rect.y() + rect.height() / 2 + data->mFlagWidth / 2);
        path.lineTo(ident + data->mFlagWidth / 2 + data->mFlagWidth / 4, rect.y() + rect.height() / 2);
        path.closeSubpath();
    }
    data->mExpandRect = QRect(ident, rect.y(), data->mFlagWidth, data->mChartHeight);
    p.fillPath(path, mContext->mColor.GetExpandFlagColor());
}

void ChartWidget::DrawData(QPainter &p, ChartData *data, QRect &rect)
{
    data->mShowedTimeStamp.clear();
    for (int i = 0; i < data->mDatas.size(); i++) {
        if (data->mDatas.at(i).end < mContext->mLeftTime) {
            continue;
        }
        if (data->mDatas.at(i).start > mContext->mRightTime) {
            break;
        }
        Long leftPosition = data->mDatas.at(i).start * mContext->mZoom + mContext->mDistance +
            mContext->mControlBarWidth;
        leftPosition = leftPosition < mContext->mControlBarWidth ? mContext->mControlBarWidth : leftPosition;
        Long rightPosition = data->mDatas.at(i).end * mContext->mZoom + mContext->mDistance +
            mContext->mControlBarWidth;
        rightPosition = rightPosition > width() ? width() : rightPosition;
        QRect rc(leftPosition, rect.y() + data->mChartHeight / 4, rightPosition - leftPosition,
            data->mChartHeight / 2);
        if (rc.width() == 0) {
            rc.setWidth(1);
        }
        ShowedInfo si;
        si.rc = rc;
        si.ts = data->mDatas.at(i);
        data->mShowedTimeStamp.push_back(si);
        mAllShowTs.push_back(si);
        QColor backColor("#" + data->GetNameHash(data->mDatas[i].name));
        p.fillRect(rc, backColor);
        p.save();
        QPen pen = p.pen();
        if (backColor.red() * 0.299 + backColor.green() * 0.587 + backColor.blue() * 0.114 >= 168.0) {
            pen.setColor(Qt::black);
        } else {
            pen.setColor(Qt::white);
        }
        p.setPen(pen);
        QString text = p.fontMetrics().elidedText(data->mDatas[i].name, Qt::ElideRight, rc.width());
        p.drawText(rc, Qt::AlignCenter, text);
        p.restore();
    }
}

void ChartWidget::DrawHighLight(QPainter &p)
{
    if (mHighLightTimeStamp == nullptr) {
        return;
    }
    Long leftPosition = mHighLightTimeStamp->ts.start * mContext->mZoom + mContext->mDistance +
        mContext->mControlBarWidth;
    Long rightPosition = mHighLightTimeStamp->ts.end * mContext->mZoom + mContext->mDistance +
        mContext->mControlBarWidth;
    Long time = mHighLightTimeStamp->ts.end - mHighLightTimeStamp->ts.start;
    QFontMetrics fm = p.fontMetrics();
    int textWidth = fm.horizontalAdvance(QString::number(time) + "ns");
    QRect textRect;
    if (mHighLightTimeStamp->ts.start < mContext->mLeftTime) {
        QRect right(rightPosition < mContext->mControlBarWidth ? mContext->mControlBarWidth : rightPosition, 0,
            width() - rightPosition, height());
        p.fillRect(right, QColor(0, 0, 0, 100));
        textRect = QRect(mContext->mControlBarWidth, 0, right.x() - mContext->mControlBarWidth, fm.height());
    } else if (mHighLightTimeStamp->ts.end > mContext->mRightTime) {
        QRect left(mContext->mControlBarWidth, 0, leftPosition - mContext->mControlBarWidth, height());
        p.fillRect(left, QColor(0, 0, 0, 100));
        textRect = QRect(left.x() + left.width(), 0, width() - left.x() - left.width(), fm.height());
    } else {
        QRect left(mContext->mControlBarWidth, 0, leftPosition - mContext->mControlBarWidth, height());
        p.fillRect(left, QColor(0, 0, 0, 100));
        QRect right(rightPosition, 0, width() - rightPosition, height());
        p.fillRect(right, QColor(0, 0, 0, 100));
        textRect = QRect(left.x() + left.width(), 0, right.x() - left.x() - left.width(), fm.height());
    }
    if (textRect.width() > textWidth + 10) {
        textWidth = textWidth + 10;
        p.save();
        p.setRenderHint(QPainter::Antialiasing);
        QRect labelRect(textRect.center().x() - textWidth / 2, 0, textWidth, fm.height());
        QPainterPath path;
        path.addRoundedRect(labelRect, fm.height() / 2, fm.height() / 2);
        p.fillPath(path, mContext->mColor.GetLabelBackGround());
        p.drawText(textRect, Qt::AlignCenter, QString::number(time) + "ns");
        p.restore();
    }
    mHighLightTimeStamp->rc = QRect(rightPosition, 0, 15, 15);
    p.save();
    p.setClipRect(rect().adjusted(mContext->mControlBarWidth, 0, 0, 0));
    p.fillRect(mHighLightTimeStamp->rc, Qt::red);
    QPen pen = p .pen();
    pen.setColor(Qt::white);
    p.setPen(pen);
    p.drawLine(mHighLightTimeStamp->rc.topLeft() + QPoint(2, 2), mHighLightTimeStamp->rc.bottomRight() - QPoint(2, 2));
    p.drawLine(mHighLightTimeStamp->rc.topRight() + QPoint(-2, 2), mHighLightTimeStamp->rc.bottomLeft() - QPoint(-2, 2));
    p.restore();
}

#ifdef SWAP
void ChartWidget::DrawDragingChart(QPainter &p, ChartData *data)
{
    if (data == nullptr) {
        return;
    }
    if (!data->mIsVisible) {
        data->mRect = QRect();
        return;
    }
    QRect dragingRect(0, data->GetFloatingYLocation() - data->GetDraging2TopDistance(), data->mRect.width(),
        data->mChartHeight);
    if (data->parent != nullptr && data->parent->mCombineShow) {
        DrawCombineChart(p, data);
    }
    else {
        p.fillRect(dragingRect, mContext->mColor.GetChartBackGround());
        p.drawRect(dragingRect);
        DrawData(p, data, dragingRect);
        DrawControlBar(p, data, dragingRect);
    }
    DrawExpandFlag(p, data, dragingRect);
    if (!data->mIsExpand) {
        return;
    }
    for (int i = 0; i < data->mChildrens.size(); i++) {
        DrawDragingChart(p, data->mChildrens.at(i));
    }
}

void ChartWidget::DrawFloatingChart(QPainter &p, ChartData *data)
{
    if (!data->IsFloating()) {
        return;
    }
    qreal ftop = data->GetFloatingYLocation();
    qreal top = data->mRect.top();
    qreal cy = (4.0 * ftop + top) / 5.0;
    if (qAbs(top - ftop) <= 2.0) {
        data->SetFloating(false);
    } else {
        data->SetFloatingYLocation(cy);
    }
    QRect dragingRect(0, data->GetFloatingYLocation() - data->GetDraging2TopDistance(), data->mRect.width(), data->mChartHeight);
    if (data->parent != nullptr && data->parent->mCombineShow) {
        DrawCombineChart(p, data);
    }
    else {
        p.fillRect(dragingRect, mContext->mColor.GetChartBackGround());
        p.drawRect(dragingRect);
        DrawData(p, data, dragingRect);
        DrawControlBar(p, data, dragingRect);
    }
    DrawExpandFlag(p, data, dragingRect);
}
#endif // SWAP

void ChartWidget::DrawCombineChart(QPainter &p, ChartData *data)
{
    if (data->parent != nullptr && data->parent->mChildrens.last() != data) {
        return;
    }
    int chartHeight = 0;
    for (int i = 0; i < data->parent->mChildrens.size(); i++) {
        chartHeight += data->parent->mChildrens.at(i)->mChartHeight;
    }
    int maxLevel = 0;
    if (data->parent->mConbineData.size() == 0) {
        for (int i = 0; i < data->parent->mChildrens.size(); i++) {
            data->parent->mConbineData.append(data->parent->mChildrens.at(i)->mDatas);
        }
        std::sort(data->parent->mConbineData.begin(), data->parent->mConbineData.end(), [](TimeStamp a, TimeStamp b) {return a.ts < b.ts; });
        QVector<TimeStamp> tmp;

        for (int i = 0; i < data->parent->mConbineData.size(); i++) {
            TimeStamp &ts = (TimeStamp &)(data->parent->mConbineData.at(i));
            for (int j = 0; j < tmp.size(); j++) {
                TimeStamp tmpts = tmp.at(j);
                if (tmpts.end < ts.start || tmpts.start > ts.end) {
                    continue;
                } else {
                    ts.level = tmpts.level + 1 > ts.level ? tmpts.level + 1 : ts.level;
                    if (ts.level > maxLevel) {
                        maxLevel = ts.level;
                    }
                }
            }
            tmp.push_back(ts);
        }
    }
    else {
        for (int i = 0; i < data->parent->mConbineData.size(); i++) {
            if (data->parent->mConbineData.at(i).level > maxLevel) {
                maxLevel = data->parent->mConbineData.at(i).level;
            }
        }
    }
    QRect combineRect;
#ifdef SWAP
    if (data->IsDraging() || data->IsFloating()) {
        combineRect = QRect(data->parent->mChildrens.at(0)->mRect.x(), data->parent->mChildrens.at(0)->GetFloatingYLocation() - data->parent->mChildrens.at(0)->GetDraging2TopDistance(),
            data->parent->mChildrens.at(0)->mRect.width(), chartHeight);
    }
    else 
    {
#endif // SWAP
        combineRect = QRect(data->parent->mChildrens.at(0)->mRect.x(), data->parent->mChildrens.at(0)->mRect.y(),
            data->parent->mChildrens.at(0)->mRect.width(), chartHeight);
#ifdef SWAP
    }
#endif
    p.fillRect(combineRect, mContext->mColor.GetChartBackGround());
    p.drawRect(combineRect);

    // draw data
    DrawCombineData(p, data, combineRect, maxLevel);
    DrawCombineControlBar(p, data, combineRect);
}

void ChartWidget::DrawCombineControlBar(QPainter &p, ChartData *data, QRect &rect)
{
    QRect controlRect = rect;
    controlRect.setWidth(mContext->mControlBarWidth);
    p.fillRect(controlRect, mContext->mColor.GetChartBackGround());
    p.drawRect(controlRect);
    p.save();
    QPen pen = p.pen();
    pen.setColor(mContext->mColor.GetChartTextColor());
    p.setPen(pen);
    p.drawText(controlRect, Qt::AlignCenter, data->mName);
    p.restore();
}

void ChartWidget::DrawCombineData(QPainter &p, ChartData *data, QRect &rect, int maxLevel)
{
    for (int i = 0; i < data->parent->mChildrens.size(); i++) {
        data->parent->mChildrens[i]->mShowedTimeStamp.clear();
    }
    int unitLevelHeight = rect.height() / (maxLevel + 1);
    for (int i = 0; i < data->parent->mConbineData.size(); i++) {
        if (data->parent->mConbineData.at(i).end < mContext->mLeftTime) {
            continue;
        }
        if (data->parent->mConbineData.at(i).start > mContext->mRightTime) {
            continue;
        }
        Long leftPosition = data->parent->mConbineData.at(i).start * mContext->mZoom + mContext->mDistance +
            mContext->mControlBarWidth;
        leftPosition = leftPosition < mContext->mControlBarWidth ? mContext->mControlBarWidth : leftPosition;
        Long rightPosition = data->parent->mConbineData.at(i).end * mContext->mZoom + mContext->mDistance +
            mContext->mControlBarWidth;
        rightPosition = rightPosition > width() ? width() : rightPosition;
        QRect rc(leftPosition, rect.y() + data->parent->mConbineData.at(i).level * unitLevelHeight, rightPosition - leftPosition, unitLevelHeight);
        if (rc.width() == 0) {
            rc.setWidth(1);
        }
        ShowedInfo si;
        si.rc = rc;
        si.ts = data->parent->mConbineData.at(i);
        for (int j = 0; j < data->parent->mChildrens.size(); j++) {
            data->parent->mChildrens[j]->mShowedTimeStamp.push_back(si);
        }
        mAllShowTs.push_back(si);
        QColor backColor("#" + data->GetNameHash(data->parent->mConbineData[i].name));
        p.fillRect(rc, backColor);
        p.save();
        QPen pen = p.pen();
        if (backColor.red() * 0.299 + backColor.green() * 0.587 + backColor.blue() * 0.114 >= 168.0) {
            pen.setColor(Qt::black);
        }
        else {
            pen.setColor(Qt::white);
        }
        p.setPen(pen);
        QString text = p.fontMetrics().elidedText(data->parent->mConbineData[i].name, Qt::ElideRight, rc.width());
        p.drawText(rc, Qt::AlignCenter, text);
        p.restore();
    }
}

void ChartWidget::DrawLinkLine(QPainter &p, QRect rect)
{
    if (mLinkLine == nullptr) {
        return;
    }
    p.save();
    p.setRenderHint(QPainter::Antialiasing);
    QRect clipRect(mContext->mControlBarWidth, 0, width() - mContext->mControlBarWidth, height());
    p.setClipRect(clipRect);
    QPen pen = p.pen();
    pen.setColor(QColor(185, 122, 87));
    pen.setWidth(3);
    p.setPen(pen);
    if (rect.contains(mLinkLine->start.rc.center())) {
        TimeStamp start = mLinkLine->start.ts;
        TimeStamp end = mLinkLine->end.ts;
        Long leftPosition = start.start * mContext->mZoom + mContext->mDistance +
            mContext->mControlBarWidth;
        leftPosition = leftPosition < mContext->mControlBarWidth ? mContext->mControlBarWidth : leftPosition;
        Long rightPosition = end.end * mContext->mZoom + mContext->mDistance +
            mContext->mControlBarWidth;
        QPainterPath path;
        path.moveTo(leftPosition, mLinkLine->start.rc.center().y() + this->mPan - mLinkLine->mPan);
        path.cubicTo(leftPosition - 50, mLinkLine->start.rc.center().y() + this->mPan - mLinkLine->mPan, rightPosition + 50, mLinkLine->end.rc.center().y()  + this->mPan - mLinkLine->mPan,
                     rightPosition, mLinkLine->end.rc.center().y() + this->mPan - mLinkLine->mPan);
        p.drawPath(path);
    }
    p.restore();
}

ChartData *ChartWidget::GetChartData(QPoint pos, ChartData *data, int start, int width)
{
    if (data == nullptr) {
        return nullptr;
    }
    QRect rect(start, data->mRect.y(), width, data->mChartHeight);
    if (rect.contains(pos)) {
        return data;
    }
    if (data->mIsExpand) {
        for (int i = 0; i < data->mChildrens.size(); ++i) {
            ChartData *d = GetChartData(pos, data->mChildrens.at(i), start, width);
            if (d) {
                return d;
            }
        }
    }
    return nullptr;
}

// 有东西被拖进来（拖着，未松开鼠标）
void ChartWidget::dragEnterEvent(QDragEnterEvent* event)
{
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(event->mimeData()->urls().at(0).toString());
//    qDebug() << "formats : " << mime.name();
    if (mime.name().startsWith("application/json")) {
        event->acceptProposedAction();
    }
}

// 东西拖进来移动
void ChartWidget::dragMoveEvent(QDragMoveEvent* event)
{
//    qDebug() << "dragMoveEvent!" << endl;
}

// 东西拖进来没有放下离开了
void ChartWidget::dragLeaveEvent(QDragLeaveEvent* event)
{
//    qDebug() << "dragLeaveEvent!" << endl;
}

// 有东西被放下（松开鼠标）
void ChartWidget::dropEvent(QDropEvent* event)
{
    ParseJsonFile(event->mimeData()->urls().at(0).path().remove(0,1));
}
