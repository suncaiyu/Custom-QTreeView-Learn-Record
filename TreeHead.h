#ifndef TREEHEAD_H
#define TREEHEAD_H
#include <QHeaderView>
#include <QProxyStyle>
class CheckBoxStyle : public QProxyStyle {
public:
    CheckBoxStyle(QStyle *style) : QProxyStyle(style){

    }

protected:
    QRect subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget) const override;
};

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
