#ifndef TREEDETAILS_H
#define TREEDETAILS_H

#include "ui_TreeDetails.h"
#include "JsonParser.h"
#include "WebSocketClient/WebSocketClient.h"

class TreeModel;
class TreeDelegate;
class TreeDetails : public QWidget, private Ui::TreeDetails
{
    Q_OBJECT
public:
    explicit TreeDetails(WebSocketClient &client,QWidget *parent = nullptr);
    ~TreeDetails();
private:
    TreeModel *mModel = nullptr;
    TreeDelegate *mDelegate = nullptr;
    JsonParser *mParser = nullptr;
    WebSocketClient *mClient = nullptr;
    bool detailsModel = false;
    TreeModel *fliterModel;

private slots:
    void ChangeModel();
};

#endif // TREEDETAILS_H
