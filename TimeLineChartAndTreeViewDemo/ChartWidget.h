#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include "TimelineContext.h"
#include "ChartData.h"
#include <QScrollBar>
#include <QMenu>

struct HighLight {
    TimeStamp ts;
    QRect rc;
};

struct LinkLine{
    int mPan = 0;
    ShowedInfo start;
    ShowedInfo end;
};

class ChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartWidget(TimelineContext &ctx, QWidget *parent = nullptr);
    virtual ~ChartWidget();
    void InitData();
    void ParseJsonFile(QString path);
    void ParseJsonContent(QByteArray content);

signals:
    void HideInfoWidgetSignal();
    void ShowInfoWidgetSignal(TimeStamp ts);

private:
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void dropEvent(QDropEvent* event);
    //void keyPressEvent(QKeyEvent *e)override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void UpdateScrollbar();
    void DrawChart(QPainter &p, int &h, ChartData *data);
    void DrawControlBar(QPainter &p, ChartData *data, QRect &rect);
    void DrawExpandFlag(QPainter &p, ChartData *data, QRect &rect);
    void DrawData(QPainter &p, ChartData *data, QRect &rect);
    void DrawHighLight(QPainter &p);
    void DrawDragingChart(QPainter &p, ChartData *data);
    void DrawFloatingChart(QPainter &p, ChartData *data);
    void DrawCombineChart(QPainter &p, ChartData *data);
    void DrawCombineControlBar(QPainter &p, ChartData *data, QRect &rect);
    void DrawCombineData(QPainter &p, ChartData *data, QRect &rect, int maxLevel);
    void DrawLinkLine(QPainter &p, QRect rect);
    ChartData *GetChartData(QPoint pos, ChartData *data, int start, int width);

    TimelineContext *mContext = nullptr;
    ChartData *mRoot = nullptr;
    QScrollBar *mScrollbar = nullptr;
    int mChartHeight = 0;
    int mPan = 0;
    HighLight *mHighLightTimeStamp = nullptr;
    ChartData *mIsDragingChart = nullptr;
    QMenu *mSelectMenu = nullptr;
    QVector<ShowedInfo> mAllShowTs; //当前界面上展示的所有的数据
    LinkLine *mLinkLine = nullptr;
};
#endif
