#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
 #include <QListWidgetItem>
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
    QString fileName;

signals:
    void signalGetHostAndIp(QString& , int & );
    void signalGetMessage(QByteArray&);
    void signalMakeDataConnection(int& );
    void signalGetUserNameAndPass(QString &, QString &);
    void signalDownloadText(QString &);
    void signalCd(QString &);
    void signalUpload(QString &, QString &);
    void signalRename(QString &, QString &);
    void signalMkDir(QString &);

private slots:
    void slotSendSignalHostAndIp();
    void slotSendMessage();
    void slotPrint(QString&);
    void slotMakeDataConnection();
    void slotSendUserNameAndPass();
    void slotAddServerFileList(QString &);
    void slotSendDownloadTextSignal();
    void slotGetChosenItem(QListWidgetItem*);
    void slotSendCd(QListWidgetItem*);
    void slotSendSignalUpload();
    void slotSendSignalRename();
    void slotSendSignalMkDir();

};

#endif // MAINWINDOW_H
