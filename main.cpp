#include "MainWindow.h"

#include <QApplication>
#include "Utils.h"
#include "LoginWindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFont font("小米兰亭");
    app.setFont(font);





//    MainWindow mainWindow;
//    mainWindow.show();

    LoginWindow loginWindow;
    loginWindow.show();


    return app.exec();
}
