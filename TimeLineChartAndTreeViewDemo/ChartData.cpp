#include "ChartData.h"
#include <qalgorithms.h>
#include <QDebug>
ChartData::ChartData()
{
}

ChartData::~ChartData()
{
    qDeleteAll(mChildrens);
}

#ifdef SWAP
void ChartData::SetDraging(bool flag, int distance)
{
    SetChildrenDraging(flag, this, distance);
}

void ChartData::SetFloating(bool flag)
{
    SetChildrenFloating(flag, this);
}

void ChartData::SetFloatingYLocation(qreal fy)
{
    SetChildrenFloatingYLocation(this, fy);
}

void ChartData::SetChildrenDraging(bool flag, ChartData *data, int distance)
{
    data->mIsDraging = flag;
    data->mDraging2TopDistance = distance;
    for (int i = 0; i < data->mChildrens.size(); i++) {
        SetChildrenDraging(flag, data->mChildrens.at(i), distance);
    }
}

void ChartData::SetChildrenFloatingYLocation(ChartData *data, qreal &fy)
{
    if (data->mLastFloatingY == fy) {
        data->mFloatingY = false;
        data->mLastFloatingY = 0.0;
    } else {
        data->mLastFloatingY = data->mFloatingY;
        data->mFloatingY = fy;
    }
    fy += data->mChartHeight;
    for (int i = 0; i < data->mChildrens.size(); i++) {
        if (data->mChildrens.at(i)->mIsExpand) {
            SetChildrenFloatingYLocation(data->mChildrens.at(i), fy);
        }
    }
}

void ChartData::SetChildrenFloating(bool flag, ChartData *data)
{
    data->mIsFloating = flag;
    data->mLastFloatingY = 0.0;
    for (int i = 0; i < data->mChildrens.size(); i++) {
        SetChildrenFloating(flag, data->mChildrens.at(i));
    }
}
#endif

QString ChartData::GetNameHash(QString name)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(name.toLatin1());
    return hash.result().toHex().left(6);
}
