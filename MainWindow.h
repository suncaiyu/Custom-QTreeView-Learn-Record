#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class TreeModel;
class TreeItem;
class ItemDelegate;
class MyFilterModel;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_returnPressed();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    TreeItem *selected = nullptr;
    TreeModel *mModel = nullptr;;
    ItemDelegate *mDelegate = nullptr;
    ItemDelegate *mFilterDelegate = nullptr;
    MyFilterModel *mFilterModel = nullptr;

    void DeletePtr();
};
#endif // MAINWINDOW_H
