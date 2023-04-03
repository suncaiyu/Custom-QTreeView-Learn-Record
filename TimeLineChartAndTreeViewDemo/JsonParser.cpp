#include "JsonParser.h"
#include <QFile>
#include <math.h>
#include <QTreeWidgetItem>
#include "DelegateAndModel/TreeItem.h"
JsonParser::JsonParser()
{

}

void JsonParser::LoadFile(QString path)
{
    traceArrays.clear();
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << file.errorString();
    }

    QJsonDocument mDocument = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (mDocument.isObject()) {
        QJsonObject obj = mDocument.object();
        GetTrace(obj);
    } else {
        qDebug() << "document is not a object";
    }
}

void JsonParser::LoadContent(QByteArray content)
{
    QJsonDocument mDocument = QJsonDocument::fromJson(content);
    if (mDocument.isObject()) {
        QJsonObject obj = mDocument.object();
        GetTrace(obj);
    } else {
        qDebug() << "document is not a object";
    }
}

void JsonParser::GetThread(QVector<ChartData *> process)
{
    QVector<ChartData *> thread;
    for (int i = 0 ; i < traceArrays.count(); i++) {
        QJsonObject obj = traceArrays.at(i).toObject();
        if (obj.value("name") == "thread_name") {
            QJsonValue val = obj.value("args");
            ChartData *cd = new ChartData;
            cd->mName = GetArg("name", val.toObject()).toString();
            cd->pid.push_back(obj.value("pid").toInt());
            QJsonValue tid = obj.value("tid");
            if (tid.isString()) {
                cd->tid.push_back(obj.value("tid").toString());
            }
            else {
                cd->tid.push_back(QString::number(obj.value("tid").toInt()));
            }
            bool finded = false;
            for (int j = 0; j < process.size(); j++) {
                if (process.at(j)->pid.contains(cd->pid.at(0)) && process.at(j)->tid.contains(cd->tid.at(0))) {
                    cd->parent = process.at(j);
                    process.at(j)->mChildrens.push_back(cd);
                    cd->mLevel = process.at(j)->mLevel + 1;
                    finded = true;
                    thread.push_back(cd);
                    break;
                }
            }
            if (!finded) {
                delete cd;
            }
        }
    }
    GetData(thread);
}

void JsonParser::GetProcess(ChartData *data)
{
    QVector<ChartData *> process;
    for (int i = 0 ; i < traceArrays.count(); i++) {
        QJsonObject obj = traceArrays.at(i).toObject();
        if (obj.value("name") == "process_name") {
            bool finded = false;
            QJsonValue val = obj.value("args");
            for (int j = 0; j < process.count(); j++) {
                if (process.at(j)->mName == GetArg("name", val.toObject()).toString()) {
                    process.at(j)->pid.push_back(obj.value("pid").toInt());
                    QJsonValue tid = obj.value("tid");
                    if (tid.isString()) {
                        process.at(j)->tid.push_back(obj.value("tid").toString());
                    }
                    else {
                        process.at(j)->tid.push_back(QString::number(obj.value("tid").toInt()));
                    }
                   finded = true;
                   break;
                }
            }
            if (finded) {
                continue;
            }
            ChartData *cd = new ChartData;
            cd->mIsTitle = true;
            cd->mName = GetArg("name", val.toObject()).toString();
            cd->pid.push_back(obj.value("pid").toInt());
            QJsonValue tid = obj.value("tid");
            if (tid.isString()) {
                cd->tid.push_back(obj.value("tid").toString());
            }
            else {
                cd->tid.push_back(QString::number(obj.value("tid").toInt()));
            }
            data->mChildrens.push_back(cd);
            cd->parent = data;
            cd->mLevel = data->mLevel + 1;
            process.push_back(cd);
        }
    }
    GetThread(process);
}

void JsonParser::GetData(QVector<ChartData *> thread)
{
    for (int i = 0; i < traceArrays.size(); i++) {
        QJsonObject obj = traceArrays.at(i).toObject();
        if (obj.contains("args")) {
            QJsonValue val = obj.value("args");
            QJsonObject subobj = val.toObject();
            if (subobj.contains("start_time") && subobj.contains("end_time")) {
                QString start = GetArg("start_time", subobj).toString();
                QString end = GetArg("end_time", subobj).toString();
                QJsonValue correlation = GetArg("correlation id", subobj);
                QJsonValue pid = GetArg("pid", obj);
                QJsonValue tid = GetArg("tid", obj);
                TimeStamp tm;
                if (!correlation.isNull()) {
                tm.correlationId = correlation.toInt();
                }
                tm.ts = GetArg("ts", obj).toDouble();
                tm.name = GetArg("name", obj).toString();
                tm.pid = pid.toInt();
                if (tid.isString()) {
                    tm.tid = tid.toString();
                } else {
                    tm.tid = QString::number(tid.toInt());
                }
                for (int j = 0; j < thread.size(); j++) {
                    if (thread.at(j)->pid.contains(tm.pid) && thread.at(j)->tid.contains(tm.tid)) {
                        tm.threadName = thread.at(j)->mName;
                        if (thread.at(j)->parent != nullptr) {
                            tm.processName = thread.at(j)->parent->mName;
                        }
                        int startIndex = start.indexOf(" ");
                        QString unit = start.mid(startIndex + 1);
                        Long stime = 0;
                        //qDebug() << start.left(startIndex).toDouble();
                        if (unit == "us") {
                            stime = ceil(start.left(startIndex).toDouble() * 1000);
                        } else if (unit == "ms") {
                            stime = start.left(startIndex).toDouble() * 1000 * 1000;
                        } else if (unit == "s") {
                            stime = start.left(startIndex).toDouble() * 1000 * 1000 * 1000;
                        } else {
                            stime = start.left(startIndex).toDouble();
                        }
                        startIndex = end.indexOf(" ");
                        unit = end.mid(startIndex + 1);
                        //qDebug() << end.left(startIndex).toDouble();
                        Long etime = 0;
                        if (unit == "us") {
                            etime = ceil(end.left(startIndex).toDouble() * 1000);
                        } else if (unit == "ms") {
                            etime = end.left(startIndex).toDouble() * 1000 * 1000;
                        } else if (unit == "s") {
                            etime = end.left(startIndex).toDouble() * 1000 * 1000 * 1000;
                        } else {
                            etime = end.left(startIndex).toDouble();
                        }
                        tm.start = stime;
                        tm.end = etime;
                        thread.at(j)->mDatas.push_back(tm);
                    }
                }
            }
        }
    }
}

void JsonParser::InitChartData(ChartData *root)
{
    if (root == nullptr) {
        return;
    }
    GetProcess(root);
}

void ParasePerfData(QJsonArray array, TreeItem *item2, long long &maxValue)
{
    for (int i = 0; i < array.size(); i++) {
        QJsonObject dataObj = array.at(i).toObject();
        TreeItem *item3 = new TreeItem(item2);
        QString name;
        long long begin = 0, end = 0;
        for (int j = 0; j < dataObj.keys().size(); j++) {
            if (dataObj.keys().at(j) == "Perf Name") {
                name = dataObj.value(dataObj.keys().at(j)).toString();
            }
            else if (dataObj.keys().at(j) == "Perf begin value") {
                begin = dataObj.value(dataObj.keys().at(j)).toVariant().toLongLong();
            }
            else if (dataObj.keys().at(j) == "Perf end value") {
                end = dataObj.value(dataObj.keys().at(j)).toVariant().toLongLong();
            }
        }
        QVector<QVariant> data3;
        data3 << name << QString::number(end - begin);
        item3->setItemData(data3);
        item2->appendChild(item3);
        if ((end - begin) > maxValue) {
            maxValue = end - begin;
        }
    }
}

void ParaseFunction(QJsonArray array, TreeItem *item2, long long &maxValue)
{
    for (int i = 0; i < array.size(); i++) {
        QJsonObject funcObj = array.at(i).toObject();
        TreeItem *item3 = new TreeItem(item2);
        QString name, line;
        for (int j = 0; j < funcObj.keys().size(); j++) {
            if (funcObj.keys().at(j) == "Kernel Name") {
                name = funcObj.value(funcObj.keys().at(j)).toString();
            }
            else if (funcObj.keys().at(j) == "Kernel Line") {
                line = QString::number(funcObj.value(funcObj.keys().at(j)).toDouble());
            }
            else {
                QJsonArray dataArray = funcObj.value(funcObj.keys().at(j)).toArray();
                ParasePerfData(dataArray, item3, maxValue);
            }
        }

        QVector<QVariant> data3;
        data3 << name + line;
        item3->setItemData(data3);
        item2->appendChild(item3);
    }
}

void JsonParser::LoadFileForTree(QString path, TreeItem *rootItem)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << file.errorString();
    }
    QJsonDocument mDocument = QJsonDocument::fromJson(file.readAll());
    file.close();
    if (mDocument.isObject()) {
        QJsonObject obj = mDocument.object();

        TreeItem *item1 = new TreeItem(rootItem);
        QVector<QVariant> data1;
        data1 << "Device id" + obj.value("Device id").toVariant().toString();
        item1->setItemData(data1);
        rootItem->appendChild(item1);

       if (obj.contains("Perf Data")) {
            QJsonValue value = obj.value("Perf Data");
            QJsonArray array = value.toArray();
            for (int i = 0; i < array.size(); i++) {
                traceArrays.push_back(array.at(i));
                QJsonObject coreObject = array.at(i).toObject();
                TreeItem *item2 = new TreeItem(item1);
                for (int j = 0; j < coreObject.keys().size(); j++) {
                    if (coreObject.keys().at(j).contains("Slave_Core_id")) {
                        QString id = QString::number(coreObject.value(coreObject.keys().at(j)).toDouble());

                        QVector<QVariant> data2;
                        data2 << coreObject.keys().at(j) + ":" + id;
                        item2->setItemData(data2);
                        item1->appendChild(item2);

                    }
                    else {
                        // Ï¸·Öfunction
                        QJsonArray funcArray = coreObject.value(coreObject.keys().at(j)).toArray();
                        ParaseFunction(funcArray, item2, maxValue);
                    }
                }
            }
       }
    } else {
        qDebug() << "document is not a object";
    }
}

void JsonParser::LoadDataForTree(QString data, TreeItem *rootItem)
{
    QJsonDocument mDocument = QJsonDocument::fromJson(data.toUtf8());
    if (mDocument.isObject()) {
        QJsonObject obj = mDocument.object();

        TreeItem *item1 = new TreeItem(rootItem);
        QVector<QVariant> data1;
        data1 << "Device id" + obj.value("Device id").toVariant().toString();
        item1->setItemData(data1);
        rootItem->appendChild(item1);

       if (obj.contains("Perf Data")) {
            QJsonValue value = obj.value("Perf Data");
            QJsonArray array = value.toArray();
            for (int i = 0; i < array.size(); i++) {
                traceArrays.push_back(array.at(i));
                QJsonObject coreObject = array.at(i).toObject();
                TreeItem *item2 = new TreeItem(item1);
                for (int j = 0; j < coreObject.keys().size(); j++) {
                    if (coreObject.keys().at(j).contains("Slave_Core_id")) {
                        QString id = QString::number(coreObject.value(coreObject.keys().at(j)).toDouble());

                        QVector<QVariant> data2;
                        data2 << coreObject.keys().at(j) + ":" + id;
                        item2->setItemData(data2);
                        item1->appendChild(item2);

                    }
                    else {
                        // Ï¸·Öfunction
                        QJsonArray funcArray = coreObject.value(coreObject.keys().at(j)).toArray();
                        ParaseFunction(funcArray, item2, maxValue);
                    }
                }
            }
       }
    } else {
        qDebug() << "document is not a object";
    }
}

void JsonParser::GetTrace(QJsonObject &obj)
{
    if (!obj.contains("traceEvents")) {
        qDebug() << "file do not have traceEvents";
        return;
    }
    QJsonValue value = obj.value("traceEvents");
    QJsonArray array = value.toArray();
    for (int i = 0; i < array.size(); i++) {
        traceArrays.push_back(array.at(i));
    }
}

QJsonValue JsonParser::GetArg(QString argname, QJsonObject obj)
{
    if (obj.contains(argname)) {
        return obj.value(argname);
    }
    return QJsonValue();
}
