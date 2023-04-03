#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>
#include <QVector>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "ChartData.h"
class QTreeWidgetItem;
class TreeItem;
class JsonParser
{
public:
    JsonParser();
    ~JsonParser()
    {
    }
    void LoadFile(QString file);
    void LoadContent(QByteArray content);
    QVector<QJsonValue> traceArrays;
    void GetThread(QVector<ChartData *>process);
    void GetProcess(ChartData *data);
    void GetData(QVector<ChartData *> thread);
    void InitChartData(ChartData *root);

    void LoadFileForTree(QString file,TreeItem *rootItem);
    void LoadDataForTree(QString data, TreeItem *rootItem);
    long long maxValue = 0;
private:
    void GetTrace(QJsonObject &obj);
    QJsonValue GetArg(QString argname, QJsonObject obj);
};

#endif // JSONPARSER_H
