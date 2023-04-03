#ifndef CHARTDATA_H
#define CHARTDATA_H
#include <QRect>
#include <QString>
#include <QVector>
#include <QCryptographicHash>
#if defined(BUILD32)
typedef long Long;
#elif defined(BUILD64)
typedef long long Long;
#endif
struct TimeStamp {
    Long start;
    Long end;
    int pid = -1;
    QString threadName = QString();
    QString tid = QString();
    QString processName = QString();
    QString name = QString();
    Long ts = 0;
    int level = 0;
    int correlationId = -1;
};

struct ShowedInfo{
    TimeStamp ts;
    QRect rc;
};
class ChartData {
public:
    explicit ChartData();
    virtual ~ChartData();
#ifdef SWAP
    void SetDraging(bool flag, int distance = 0);
    bool IsDraging() { return mIsDraging; }
    void SetFloating(bool flag);
    bool IsFloating() { return mIsFloating; }
    void SetFloatingYLocation(qreal fy);
    qreal GetFloatingYLocation() { return mFloatingY; }
    int GetDraging2TopDistance() { return mDraging2TopDistance; }
#endif

    QVector<int> pid;
    QVector<QString> tid;

    bool mIsVisible = true;
    bool mIsTitle = false;
    QRect mRect;
    QString mName;
    int mChartHeight = 50;

    ChartData *parent = nullptr;
    int mLevel = 0;
    int mIndentUnits = 10;
    bool mIsExpand = true;
    int mFlagWidth = 8;
    QRect mExpandRect = QRect();
    QVector<ChartData *> mChildrens;
    QVector<TimeStamp> mDatas;
    QVector<ShowedInfo> mShowedTimeStamp;

    QString GetNameHash(QString name);

    bool mCombineShow = false;
    QVector<TimeStamp> mConbineData;

private:
#ifdef SWAP
    bool mIsDraging = false;
    bool mIsFloating = false;
    int mDraging2TopDistance = 0;
    qreal mFloatingY = 0.0;
    qreal mLastFloatingY = 0.0;
    void SetChildrenDraging(bool flag, ChartData *child, int distance = 0);
    void SetChildrenFloatingYLocation(ChartData *data, qreal &fy);
    void SetChildrenFloating(bool flag, ChartData *data);
#endif // SWAP
};
#endif // !CHARTDATA_H
