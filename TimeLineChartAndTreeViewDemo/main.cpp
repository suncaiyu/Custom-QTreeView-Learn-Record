#include "MainWindow.h"
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    qputenv("QT_MESSAGE_pattern", "%{appname} %{type} %{time [yyyy-MM-dd hh:mm:ss]} %{file} %{line} %{function} : %{message}");
    QApplication a(argc, argv);
    int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/Simplified-Chinese3500.otf"));
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (fontFamilies.size() > 0) {
        QFont font;
        font.setFamily(fontFamilies[0]);//设置全局字体
        a.setFont(font);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
