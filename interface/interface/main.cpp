
#include "ftpclient.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    FtpClient client;
    MainWindow interface(nullptr, &client);
    interface.show();
    return a.exec();
}
