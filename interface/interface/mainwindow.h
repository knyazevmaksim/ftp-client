#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ftpclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, FtpClient *ptr=nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    FtpClient *p_ftpClient;

signals:
    void signalGetHostAndIp(QString& , int & );
    void signalGetMessage(QByteArray&);
    void signalMakeDataConnection(int& );
    void signalGetUserNameAndPass(QByteArray &, QByteArray &);

private slots:
    void slotSendSignalHostAndIp();
    void slotSendMessage();
    void slotPrint(QString&);
    void slotMakeDataConnection();
    void slotSendUserNameAndPass();
    void slotAddServerFileList(QString &);

};

#endif // MAINWINDOW_H
