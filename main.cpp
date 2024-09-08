#include "MainWindow.h"
#include <QDir>
#include <QApplication>
#include <QDebug>
#include "Utils.h"
#include "LoginWindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFont font("微软雅黑");
    app.setFont(font);

    QString path = DOCUMENT_PATH + "/OOPM/passports/"; // 指定新建文件夹的路径
    QDir dir;

    if (!dir.exists(path)) {
        // 如果文件夹不存在，则创建它
        if (dir.mkpath(path)) {
            qDebug() << "Folder created successfully!";
        } else {
            qDebug() << "Failed to create folder.";
        }
    } else {
        qDebug() << "Folder already exists.";
    }

    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}
