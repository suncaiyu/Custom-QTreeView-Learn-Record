#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "createmodel.h"
#include <QQmlContext>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    //参数：qmlRegisterType<C++类型名> (命名空间 主版本 次版本 QML中的类型名)
    qmlRegisterType<CreateModel>("CreateModel", 1, 0, "CreateModel");
    qmlRegisterType<MyFunction>("MyFunction", 1, 0, "MyFunction");
    qRegisterMetaType<TreeModel *>("TreeModel *");
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/TreeViewDemo.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
