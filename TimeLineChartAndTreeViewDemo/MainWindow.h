#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QSplitter>
#include "TimelineWidget.h"
#include "ChartWidget.h"
#include "InfoWidget.h"
#include "WebSocketClient/WebSocketClient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
using namespace  std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<QVBoxLayout> mLayout;
    std::unique_ptr<QSplitter> mSplitter;
    std::unique_ptr<QVBoxLayout> mChartLayout;
    std::unique_ptr<InfoWidget> mInfoWidget;
    std::unique_ptr<ChartWidget> mChart;
    std::unique_ptr<TimelineWidget> mTimeline;
    WebSocketClient mClient;

    void keyPressEvent(QKeyEvent *e) override;
};
#endif // MAINWINDOW_H
