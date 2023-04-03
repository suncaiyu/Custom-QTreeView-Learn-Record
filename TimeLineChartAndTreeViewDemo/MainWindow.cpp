#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QFileDialog>
#include "TreeDetails.h"
#include <QPushButton>
//#include <emscripten.h>

void SetQSS(QString path, QWidget *use)
{
    QFile file(path);
    file.open(QFile::ReadOnly);
    QString styleSheet = (file.readAll());
    use->setStyleSheet(styleSheet);
    file.close();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), mClient(QUrl("ws://10.10.4.12:1234"))
{
    ui->setupUi(this);
    setFocus();
    mLayout = std::make_unique<QVBoxLayout>();
    mChartLayout = std::make_unique<QVBoxLayout>();
    ui->centralwidget->setLayout(mLayout.get());
    mTimeline = std::make_unique <TimelineWidget>();
    mChart = std::make_unique<ChartWidget>(mTimeline->GetTimelineContext());
    mSplitter = std::make_unique<QSplitter>(Qt::Orientation::Vertical);
    QPushButton *btn = new QPushButton("Open");
    btn->hide();
    connect(btn, &QPushButton::clicked, [this](bool f){
        auto fileContentReady = [this](const QString &fileName, const QByteArray &fileContent) {
            if (fileName.isEmpty()) {
                // No file was selected
            } else {
                // Use fileName and fileContent
                qDebug() << fileName;
                mChart->ParseJsonContent(fileContent);
            }
        };
        QFileDialog::getOpenFileContent(tr("Json Files (*.json)"),  fileContentReady);
        //MyOpenFile(tr("Json Files (*.json)"),  fileContentReady);
//        mChart->ParseJsonFile(QFileDialog::getOpenFileName(this, "open json", "", tr("Json Files (*.json)")));
    });
    mLayout->addWidget(mSplitter.get());
    mLayout->addWidget(btn);
    mLayout->setSpacing(0);
    mLayout->setContentsMargins(0,0,0,0);

    mChartLayout->setContentsMargins(0,0,0,0);
    mChartLayout->setSpacing(0);

    mChartLayout->addWidget(mTimeline.get());
    mChartLayout->addWidget(mChart.get());
    mSplitter->addWidget(new QWidget);
    mSplitter->widget(0)->setLayout(mChartLayout.get());

    mInfoWidget = std::make_unique<InfoWidget>();
    mSplitter->addWidget(mInfoWidget.get());

    SetQSS(":/qss1.qss", this);
    mSplitter->setSizes(QList<int>() << 700 << 300);
    mInfoWidget->hide();

    connect(mChart.get(), &ChartWidget::HideInfoWidgetSignal, this, [this](){
        mInfoWidget->hide();
    });

    connect(mChart.get(), &ChartWidget::ShowInfoWidgetSignal, this, [this](TimeStamp ts){
        mInfoWidget->SetInfoData(ts);
        mInfoWidget->show();
    });

    TreeDetails *details = new TreeDetails(mClient);
    mLayout->addWidget(details);
    mChart->ParseJsonFile(":/host_06fb1bade340pid_117_time_2023_01_13_03_15_23_518049.paddle_trace.json");
    QPushButton *change = new QPushButton("Change Model");
    mLayout->addWidget(change);
    connect(change, &QPushButton::clicked, [this, details](){
        QMetaObject::invokeMethod(details, "ChangeModel", Qt::QueuedConnection);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_1) {
        SetQSS(":/qss1.qss", this);
    }
    if (e->key() == Qt::Key_2) {
        SetQSS(":/qss2.qss", this);
    }
    if (e->key() == Qt::Key_W) {
        mTimeline->ZoomIn();
    }
    if (e->key() == Qt::Key_S) {
        mTimeline->ZoomOut();
    }
    if (e->key() == Qt::Key_A) {
        mTimeline->MoveLeft();
    }
    if (e->key() == Qt::Key_D) {
        mTimeline->MoveRight();
    }
    if (e->key() == Qt::Key_O && e->modifiers() == Qt::ControlModifier) {
        mChart->ParseJsonFile(QFileDialog::getOpenFileName(this, "open json", "", tr("Json Files (*.json)")));
    }
    QMainWindow::keyPressEvent(e);
}

