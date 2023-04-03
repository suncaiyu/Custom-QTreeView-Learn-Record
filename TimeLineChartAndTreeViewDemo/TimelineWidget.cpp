#include "TimelineWidget.h"
#include <QPainter>
#include <QWheelEvent>
#include <QPainterPath>

namespace {
    constexpr int AMPLIFICATION_FACTOR = 15;
    constexpr int WINDOW_HEIGHT = 25;
    constexpr int FONT_SIZE = 8;
    constexpr int BASE_LINE = 0;
    constexpr float TEXT_SPACING = 1.5;
    constexpr int TICK_HEIGHT = 8;
}

TimelineWidget::TimelineWidget(QWidget *parent)
    : QWidget{parent}, mContext(this), mMinStep(0), mPressedPoint(0)
{
    setMouseTracking(true);
    setFixedHeight(WINDOW_HEIGHT);
    mContext.AddUpdateWidget(this);
    zoomAnimation = new QPropertyAnimation(this, "");
    zoomAnimation->setEasingCurve(QEasingCurve::OutExpo);
    distanceAnimation = new QPropertyAnimation(this, "");
    distanceAnimation->setEasingCurve(QEasingCurve::OutExpo);
}

TimelineWidget::~TimelineWidget()
{
    if (zoomAnimation->state() != QPropertyAnimation::Stopped) {
        zoomAnimation->stop();
        delete zoomAnimation;
    }
    if (distanceAnimation->state() != QPropertyAnimation::Stopped) {
        distanceAnimation->stop();
        delete distanceAnimation;
    }
}

TimelineContext &TimelineWidget::GetTimelineContext()
{
    return mContext;
}

void TimelineWidget::ZoomOut()
{
    if (zoomAnimation->state() == QPropertyAnimation::Running) {
        zoomAnimation->stop();
    }
    //suo xiao
    double mousePos = mapFromGlobal(QCursor::pos()).x();
    double mouseOnTimePoint = (mousePos - mContext.mDistance - mContext.mControlBarWidth)
        / mContext.mZoom;
    double target = (mContext.mZoom / 1.3) < ScaleFactor::MIN_ZOOM ? ScaleFactor::MIN_ZOOM : mContext.mZoom / 1.3;
    zoomAnimation->setStartValue(mContext.mZoom);
    zoomAnimation->setEndValue(target);
    zoomAnimation->setDuration(200);
    connect(zoomAnimation, &QPropertyAnimation::valueChanged, [this, mouseOnTimePoint, mousePos](const QVariant &value) {
        mContext.mZoom = value.toDouble();
        mContext.mDistance = -(mouseOnTimePoint * mContext.mZoom - mousePos + mContext.mControlBarWidth);
    });
    zoomAnimation->start();
}

void TimelineWidget::ZoomIn()
{
    if (zoomAnimation->state() == QPropertyAnimation::Running) {
        zoomAnimation->stop();
    }
    double mousePos = mapFromGlobal(QCursor::pos()).x();
    double mouseOnTimePoint = (mousePos - mContext.mDistance - mContext.mControlBarWidth)
        / mContext.mZoom;
    double target = (mContext.mZoom * 1.3) < ScaleFactor::MIN_ZOOM ? ScaleFactor::MIN_ZOOM : mContext.mZoom * 1.3;
    zoomAnimation->setStartValue(mContext.mZoom);
    zoomAnimation->setEndValue(target);
    zoomAnimation->setDuration(200);
    connect(zoomAnimation, &QPropertyAnimation::valueChanged, [this, mouseOnTimePoint, mousePos](const QVariant &value) {
        mContext.mZoom = value.toDouble();
        mContext.mDistance = -(mouseOnTimePoint * mContext.mZoom - mousePos + mContext.mControlBarWidth);
        //Layout();
    });
    zoomAnimation->start();
}

void TimelineWidget::MoveLeft()
{
    if (distanceAnimation->state() == QPropertyAnimation::Running) {
        distanceAnimation->stop();
    }
    int step = 100;
    distanceAnimation->setStartValue(0);
    distanceAnimation->setEndValue(step);
    distanceAnimation->setDuration(500);
    double original = mContext.mDistance;
    connect(distanceAnimation, &QPropertyAnimation::valueChanged, [this, original](const QVariant &value) {
        mContext.mDistance = original - value.toDouble();
        //Layout();
    });
    distanceAnimation->start();
    //mContext.mDistance -= step;
    //Layout();
}

void TimelineWidget::MoveRight()
{
    if (distanceAnimation->state() == QPropertyAnimation::Running) {
        distanceAnimation->stop();
    }
    int step = 100;
    distanceAnimation->setStartValue(0);
    distanceAnimation->setEndValue(step);
    distanceAnimation->setDuration(500);
    double original = mContext.mDistance;
    connect(distanceAnimation, &QPropertyAnimation::valueChanged, [this, original](const QVariant &value) {
        mContext.mDistance = original + value.toDouble();
        //Layout();
    });
    distanceAnimation->start();
}

void TimelineWidget::paintEvent(QPaintEvent *painter)
{
    Layout();
    QPainter p(this);
    DrawBackGround(p);
    QFont font = p.font();
    //font.setFamily(PaintParameters::FONT_NAME);
    font.setPointSize(FONT_SIZE);
    p.setFont(font);
    QFontMetrics fm = p.fontMetrics();
    QPen pen = p.pen();
    Long startTick = mContext.mLeftTime - (mContext.mLeftTime % mMinStep) - mMinStep;
    p.save();
    pen.setColor(mContext.mColor.GetTimelineTextColor());
    p.setPen(pen);
    DrawTickAndLabel(p, startTick, mUnitTickPixel, fm);
    p.restore();
    DrawMouseMeasureLine(p);
    DrawControlBar(p);
    p.end();
}

void TimelineWidget::wheelEvent(QWheelEvent *e)
{
    int delta = e->angleDelta().y();
    double mousePos = e->position().x();
    mContext.UpdateZoom(mousePos, delta);
    mContext.Update();
}

bool TimelineWidget::CanChangeControlWidth()
{
    if (mContext.mMouseOnX > mContext.mControlBarWidth - 5 && mContext.mMouseOnX <= mContext.mControlBarWidth) {
        return true;
    } else {
        return false;
    }
}

void TimelineWidget::DrawMouseMeasureLine(QPainter &p)
{
    if (!mContext.mShowTimelineLabel) {
        return;
    }
    QFontMetrics fm = p.fontMetrics();
    Long mouseOnTime = (Long)(((double)(mContext.mMouseOnX)
        - mContext.mDistance - mContext.mControlBarWidth)
        / mContext.mZoom);
    Long lineLeft = mouseOnTime * mContext.mZoom + mContext.mDistance + mContext.mControlBarWidth;
    Long lineRight = (mouseOnTime + 1) * mContext.mZoom
        + mContext.mDistance + mContext.mControlBarWidth;
    int showX = 0;
    QString showTime;
    if (mContext.mMouseOnX - lineLeft > lineRight - mContext.mMouseOnX) {
        showX = lineRight;
        showTime = QString::number(mouseOnTime + 1) + "ns";
    } else {
        showX = lineLeft;
        showTime = QString::number(mouseOnTime) + "ns";
    }
    int textWidth = fm.horizontalAdvance(showTime) + 10;
    if (showX < rect().width() && showX > rect().x()) {
        QRect rc(showX - textWidth / 2, 0, textWidth, rect().height());
        QPainterPath path;
        path.addRoundedRect(rc, rect().height() / static_cast<qreal>(2), rect().height() / static_cast<qreal>(2));
        p.save();
        QPen pen = p.pen();
        pen.setColor(Qt::white);
        p.setPen(pen);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillPath(path, mContext.mColor.GetLabelBackGround());
        pen.setColor(mContext.mColor.GetLabelTextColor());
        p.setPen(pen);
        p.drawText(rc, Qt::AlignCenter, showTime);
        p.restore();
    }
}

void TimelineWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    if (e->pos().x() > mContext.mControlBarWidth) {
        mIsPressed = true;
    }
    if (CanChangeControlWidth()) {
        mCanChangeControlWidth = true;
    }
    mPressedPoint = e->pos().x();
    mContext.SetPressPoint(e->pos());
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
    mContext.mShowTimelineLabel = false;
    mContext.mMouseOnX = e->pos().x();
    if (CanChangeControlWidth()) {
        this->setCursor(Qt::SplitHCursor);
    } else {
        this->setCursor(Qt::ArrowCursor);
    }
    if (mIsPressed) {
        mContext.UpdateDistance(e->pos());
    }
    if (mCanChangeControlWidth) {
        mContext.mControlBarWidth = e->x();
    }
    //Layout();
    mContext.Update();
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
    mContext.ClearPressPoint();
    mIsPressed = false;
    mCanChangeControlWidth = false;
    mContext.Update();
}

void TimelineWidget::resizeEvent(QResizeEvent *e)
{
    Layout();
    QWidget::resizeEvent(e);
}

void TimelineWidget::Layout()
{
    if (qFuzzyIsNull(mContext.mZoom)) {
        return;
    }
    Long leftTime = (Long)(-mContext.mDistance / mContext.mZoom);
    Long rightTime = (Long)((double)(rect().width() - mContext.mControlBarWidth - mContext.mDistance) /
            mContext.mZoom);
    mContext.mLeftTime = leftTime;
    mContext.mRightTime = rightTime;
    Long tickOnWindow = rightTime - leftTime/* + 1*/;
    Long mPerPixel2Ns = tickOnWindow / rect().width();
    mMinStep = 1;
    while (mPerPixel2Ns > mMinStep || mMinStep * mContext.mZoom < 5) {
        mMinStep *= 10;
    }
    mUnitTickPixel = mMinStep * mContext.mZoom;
}

void TimelineWidget::DrawBackGround(QPainter &p)
{
    p.fillRect(rect(), BackGroundBrush());
    p.save();
    QPen pen = p.pen();
    pen.setColor(mContext.mColor.GetBorderColor());
    p.setPen(pen);
    p.drawRect(rect().adjusted(0, 0, -1, -1));
    p.restore();
}

void TimelineWidget::DrawTickAndLabel(QPainter &p, Long startTick, int unitTickPixel, QFontMetrics &fm)
{
    QString maxText = QString::number(mContext.mRightTime);
    int maxTextWidth = fm.horizontalAdvance(maxText);
    for (Long drawTime = startTick; drawTime < mContext.mRightTime + mMinStep; drawTime += mMinStep) {
        double onScreenX = double(drawTime) * mContext.mZoom
            + mContext.mDistance + mContext.mControlBarWidth;
        if (onScreenX < mContext.mControlBarWidth) {
            continue;
        }
        p.drawLine(onScreenX, BASE_LINE, onScreenX, BASE_LINE + TICK_HEIGHT);

        if (drawTime % (mMinStep * 100) == 0) {
            QString show = TransTime2String(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 100 > maxTextWidth * TEXT_SPACING) {
                p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
            }
            continue;
        }

        if (drawTime % (mMinStep * 10) == 0) {
            QString show = TransTime2String(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 10 > maxTextWidth * TEXT_SPACING) {
                p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
            } else {
                show = TransTime2String(drawTime % (mMinStep * 100));
                textWidth = fm.horizontalAdvance(show);
                if (unitTickPixel * 10 > textWidth * TEXT_SPACING) {
                    p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
                }
            }
            continue;
        }

        if (drawTime % (mMinStep * 5) == 0) {
            QString show = TransTime2String(drawTime);
            int textWidth = fm.horizontalAdvance(show);
            if (unitTickPixel * 5 > maxTextWidth * TEXT_SPACING) {
                p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
            }
            continue;
        }

        QString show = TransTime2String(drawTime);
        int textWidth = fm.horizontalAdvance(show);
        if (unitTickPixel > maxTextWidth * TEXT_SPACING) {
            p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
        }
    }
}

QString TimelineWidget::TransTime2String(Long nsVal)
{
    //if (nsVal < 0 || natureShow == 0) {
    //    return QString::number(nsVal);
    //}
    int zero = 0;
    Long tmpValue = nsVal;
    while (tmpValue > 0) {
        if (tmpValue % 10 == 0) {
            zero++;
            tmpValue /= 10;
        } else {
            break;
        }
    }
    QString resultVal;
    if (zero >= 9) {
        resultVal = QString::number(nsVal / 1000000000) + "s";
    } else if (zero >= 6) {
        resultVal = QString::number(nsVal / 1000000) + "ms";
    } else if (zero >= 3) {
        resultVal = QString::number(nsVal / 1000) + "us";
    } else {
        resultVal = QString::number(nsVal) + "ns";
    }
    return resultVal;
}

void TimelineWidget::DrawControlBar(QPainter &p)
{
    QRect controlBarRect = QRect(0, 0, mContext.mControlBarWidth, height());
    p.fillRect(controlBarRect, BackGroundBrush());
    p.save();
    QPen pen = p.pen();
    pen.setColor(mContext.mColor.GetBorderColor());
    p.setPen(pen);
    p.drawRect(controlBarRect.adjusted(0, 0, 0, -1));
    p.restore();
}

QBrush TimelineWidget::BackGroundBrush()
{
    QLinearGradient linearGradient;
    linearGradient.setStart(0, 0);
    linearGradient.setFinalStop(0, rect().height());
    linearGradient.setColorAt(0.0, mContext.mColor.GetTimelineBackGroundStart());
    linearGradient.setColorAt(1.0, mContext.mColor.GetTimelineBackGroundEnd());
    return linearGradient;
}
