#include "MainWindow.h"

#include <QApplication>
#include <QTextStream>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFont font("小米兰亭"); // 字体名称和大小
    app.setFont(font);

    QFile file(":/styles/darkstyle.qss");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 打印错误信息到标准错误输出
        qWarning() << "Failed to open file:" << file.errorString();
        return -1; // 或者其他错误处理
    }

    QTextStream stream(&file);
    QString styleSheet = stream.readAll();
    app.setStyleSheet(styleSheet);


    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
