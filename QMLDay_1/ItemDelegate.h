#pragma once
#include <QStyledItemDelegate>
#include <QTreeView>
class ItemDelegate : public QStyledItemDelegate
{
public:
    ItemDelegate(QTreeView *view, QObject *parent = nullptr);
    virtual ~ItemDelegate() {};

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void setMatchWord(QString str) { mMatchWord = str; };

private:
    QTreeView *m_View;
    QString mMatchWord = "";
    bool mHasProxy;
    void DrawTriangle(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)const;
    void setQss();
    void DrawKeyword(QPainter *painter,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) const;
    QStringList SpliteWord(QString originWord) const;
};

