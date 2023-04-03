#include "TimelineContext.h"
#include <QObject>
#include <QWidget>

TimelineContext::TimelineContext(QWidget *parent) : QWidget(parent), mColor(this)
{
    mTimer = std::make_unique<QTimer>();
    mTimer->start(30);
}

void TimelineContext::AddUpdateWidget(QWidget *widget)
{
    QObject::connect(mTimer.get(), &QTimer::timeout, widget, [widget]() { widget->update(); });
    mWidgets.push_back(widget);
}

void TimelineContext::UpdateZoom(qreal mousePos, int delta)
{
    double mouseOnTimePoint = (mousePos - mDistance - mControlBarWidth)
        / mZoom;
    double target;
    if (delta > 0) {
        target = mZoom * ScaleFactor::ZOOM_RATIO > ScaleFactor::MAX_ZOOM ? ScaleFactor::MAX_ZOOM : mZoom * ScaleFactor::ZOOM_RATIO;
        mZoom = mZoom * ScaleFactor::ZOOM_RATIO > ScaleFactor::MAX_ZOOM ? ScaleFactor::MAX_ZOOM : mZoom * ScaleFactor::ZOOM_RATIO;
    }
    else {
        target = mZoom / ScaleFactor::ZOOM_RATIO < ScaleFactor::MIN_ZOOM ? ScaleFactor::MIN_ZOOM : mZoom / ScaleFactor::ZOOM_RATIO;
        mZoom = mZoom / ScaleFactor::ZOOM_RATIO < ScaleFactor::MIN_ZOOM ? ScaleFactor::MIN_ZOOM : mZoom / ScaleFactor::ZOOM_RATIO;
    }

    mDistance = -(mouseOnTimePoint * mZoom - mousePos + mControlBarWidth);
}

void TimelineContext::UpdateDistance(QPoint pos)
{
    double distance = pos.x() - mPressedPoint.x();
    mPressedPoint = pos;
    // 暂定可向左拖动1000像素
    if (mDistance + distance < 1000) {
        mDistance = mDistance + distance;
    }
    else {
        mDistance = 1000;
    }
}
