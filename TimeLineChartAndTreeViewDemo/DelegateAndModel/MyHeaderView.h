#ifndef MYHEADERVIEW_H
#define MYHEADERVIEW_H
#include <QHeaderView>

class MyHeaderView : public QHeaderView
{
public:
    MyHeaderView(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~MyHeaderView();
private:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
};

#endif // MYHEADERVIEW_H
