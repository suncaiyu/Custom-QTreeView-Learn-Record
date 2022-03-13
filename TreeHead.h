#ifndef TREEHEAD_H
#define TREEHEAD_H
#include <QHeaderView>

class TreeHead : public QHeaderView
{
    Q_OBJECT
public:
    TreeHead(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~TreeHead();

protected:
    void paintEvent(QPaintEvent *e) override;
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
};

#endif // TREEHEAD_H
