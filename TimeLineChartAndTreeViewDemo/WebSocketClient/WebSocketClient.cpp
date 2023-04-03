#include "WebSocketClient.h"
#include <qjsondocument.h>
#include <QJsonObject>
WebSocketClient::WebSocketClient(QUrl url, QObject *parent)
{
//    QString json = "{\"type\" : \"filename\" , \"value\" : 99}";
//    QJsonDocument jd = QJsonDocument::fromJson(json.toLocal8Bit());
//    QJsonObject jo = jd.object();
//    qDebug() << jo.value("value");
    mSocket = new QWebSocket();
    connect(mSocket, &QWebSocket::connected,
            [=]()
    {
        qDebug()<<("connect success");
        SendMessage(QString("user1"));
    }
    );
//    {"type" : xxx, "value" : ""};
    connect(mSocket, &QWebSocket::textMessageReceived, [=](const QString &msg) {
         QJsonDocument jd = QJsonDocument::fromJson(msg.toUtf8());
         if (!jd.isObject()) {
             qDebug() << "back!!,not json";
         }
         QJsonObject jo = jd.object();
         if (jo.value("type").isUndefined() || jo.value("type").isNull()) {
             qDebug() << "dont have type";
         }
         QJsonValue type = jo.value("type");
         if (type == "treemodel") {
//            qDebug().noquote() << jo.value("value");
//            jd = QJsonDocument::fromJson(jo.value("value").toString().toUtf8());
            qDebug() << "iiiii";
            emit TreeModelDataSignal(jo.value("value").toString());
         }
    });

    connect(mSocket, &QWebSocket::binaryMessageReceived,
            [=](const QByteArray &msg)
    {
         qDebug() << (msg);
    }
    );
    qDebug() << "connect" << url;
    mSocket->open(url);
}

WebSocketClient::~WebSocketClient()
{
    delete mSocket;
}

void WebSocketClient::SendMessage(QString str)
{
    mSocket->sendTextMessage(str);
}

void WebSocketClient::SendMessage(QByteArray byte)
{
    mSocket->sendBinaryMessage(byte);
}
