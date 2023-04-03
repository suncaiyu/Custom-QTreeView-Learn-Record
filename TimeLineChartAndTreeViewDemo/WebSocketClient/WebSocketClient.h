#ifndef WEBSOCKETCLIENT
#define WEBSOCKETCLIENT
#include <QWidget>
#include <QWebSocketServer>
#include <QUrl>
#include <QWebSocket>

class WebSocketClient : public QObject {
    Q_OBJECT
public:
    explicit WebSocketClient(QUrl url, QObject *parent = nullptr);
    ~WebSocketClient();
    void SendMessage(QString str);
    void SendMessage(QByteArray byte);

private:
    QWebSocket *mSocket;

signals:
    void TreeModelDataSignal(const QString &msg);
};
#endif
