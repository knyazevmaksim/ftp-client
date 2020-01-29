
#include "ftpclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FtpClient client("localhost", 21);
    client.show();
    return a.exec();
}
