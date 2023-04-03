#ifndef TIMELINECONTEXT_H
#define TIMELINECONTEXT_H
#include <QTimer>
#include <memory>
#include <QColor>
#include <QDebug>
#include <QWidget>
#if defined(BUILD32)
typedef long Long;
#elif defined(BUILD64)
typedef long long Long;
#endif

namespace ScaleFactor {
    constexpr float ZOOM_RATIO = 1.22;
    constexpr double MIN_ZOOM = 1.0e-10;
    constexpr double MAX_ZOOM = 500.0;
}

class Color : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QColor TimelineBackGroundStart READ GetTimelineBackGroundStart WRITE SetTimelineBackGroundStart)
    Q_PROPERTY(QColor TimelineBackGroundEnd READ GetTimelineBackGroundEnd WRITE SetTimelineBackGroundEnd)
    Q_PROPERTY(QColor LabelBackGround READ GetLabelBackGround WRITE SetLabelBackGround)
    Q_PROPERTY(QColor LabelTextColor READ GetLabelTextColor WRITE SetLabelTextColor)
    Q_PROPERTY(QColor ChartTextColor READ GetChartTextColor WRITE SetChartTextColor)
    Q_PROPERTY(QColor TimelineTextColor READ GetTimelineTextColor WRITE SetTimelineTextColor)
    Q_PROPERTY(QColor BorderColor READ GetBorderColor WRITE SetBorderColor)
    Q_PROPERTY(QColor ChartBackGround READ GetChartBackGround WRITE SetChartBackGround)
    Q_PROPERTY(QColor ExpandFlagColor READ GetExpandFlagColor WRITE SetExpandFlagColor)
public:
    Color(QWidget *parent) : QWidget(parent) {}
    ~Color() {};
    void SetTimelineBackGroundStart(QColor c) { mTimelineBackGroundStart = c; }
    QColor GetTimelineBackGroundStart() { return mTimelineBackGroundStart; }
    void SetTimelineBackGroundEnd(QColor c) { mTimelineBackGroundEnd = c; }
    QColor GetTimelineBackGroundEnd() { return mTimelineBackGroundEnd; }
    void SetLabelBackGround(QColor c) { mLabelBackGround = c; }
    QColor GetLabelBackGround() { return mLabelBackGround; }
    void SetLabelTextColor(QColor c) { mLabelTextColor = c; }
    QColor GetLabelTextColor() { return mLabelTextColor; }
    void SetBorderColor(QColor c) { mBorderColor = c; }
    QColor GetBorderColor() { return mBorderColor; }
    void SetChartBackGround(QColor c) { mChartBackGround = c; }
    QColor GetChartBackGround() { return mChartBackGround; }
    void SetExpandFlagColor(QColor c) { mExpandFlagColor = c; }
    QColor GetExpandFlagColor() { return mExpandFlagColor; }
    void SetTimelineTextColor(QColor c) { mTimelineTextColor = c; }
    QColor GetTimelineTextColor() { return mTimelineTextColor; }
    void SetChartTextColor(QColor c) { mChartTextColor = c; }
    QColor GetChartTextColor() { return mChartTextColor; }

private:
    QColor mTimelineBackGroundStart;
    QColor mTimelineBackGroundEnd;
    QColor mLabelBackGround;
    QColor mLabelTextColor;
    QColor mChartTextColor;
    QColor mTimelineTextColor;
    QColor mBorderColor;
    QColor mChartBackGround;
    QColor mExpandFlagColor;
};

class TimelineContext : public QWidget
{
    Q_OBJECT

public:
    TimelineContext(QWidget *parent);
    virtual ~TimelineContext(){}
    double mZoom = 6e-02; // 缩放系数
    double mDistance = 0.0; // 超出屏幕的距离
    int mMouseOnX = 0;
    int mControlBarWidth = 100;
    bool mShowTimelineLabel = true;
    Long mLeftTime = 0;
    Long mRightTime = 0;
    Color mColor;
    void Update() {
        for (QWidget *w: mWidgets) {
            w->update();
        }
    }

    void AddUpdateWidget(QWidget *widget);
    void UpdateZoom(qreal mouseXLocation, int delta);
    void UpdateDistance(QPoint pos);
    void SetPressPoint(QPoint pos) { mPressedPoint = pos; };
    void ClearPressPoint() { mPressedPoint = QPoint(); }
    QPoint GetPressPoint() { return mPressedPoint; }
private:
    std::unique_ptr<QTimer> mTimer;
    QPoint mPressedPoint = QPoint();
    QVector<QWidget *> mWidgets;
};
#endif // TIMELINECONTEXT_H
