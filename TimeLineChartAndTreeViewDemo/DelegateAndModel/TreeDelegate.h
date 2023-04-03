#ifndef TREEDELEGATE_H
#define TREEDELEGATE_H
#include <QStyledItemDelegate>
class QTreeView;
class TreeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TreeDelegate(QObject *parent, long long max);
    ~TreeDelegate();

protected:
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    bool editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index) override;
private:
    QTreeView *mTreeView = nullptr;
    long long maxValue;

    void DrawBranchIcon(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // TREEDELEGATE_H
