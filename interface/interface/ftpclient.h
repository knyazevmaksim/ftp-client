#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

#include "session.h"


class FtpClient : public QWidget
{
    Q_OBJECT

private:
    QTcpSocket* TcpSocketCommand;
    QTcpSocket* TcpSocketData;
    QString hostName;
    int port;
    int passivePort;
    QString serverFileList;
    int getPassivePort();
    QString getServerFile(QString &);
    bool isServerFileList;
    bool download;
    QString fileName;
    QByteArray QStringToQByteArray (const QString &);
    QList<QByteArray> lst;



public:
    FtpClient(QWidget* pwgt=0);
    void get(const QString &, QIODevice * device=0);
    QByteArray readAllData();
    QByteArray readAllCommand();
    void list(const QString &);
    void cd(const QString &);
    void put(const QByteArray&,const QString &);
    void login(const QString &,const QString &);
    void rename(const QString &, const QString &);
    void remove(const QString &);
    void mkDir(const QString &);
    void rmDir(const QString &);
    void getBin(const QString &, QIODevice * device=0);
    void putBin(const QByteArray&,const QString &);

    void get_test(int port, QIODevice * device=0);
    void downloadList(QList<QByteArray>&);
    void downloadDeque();

signals:
    void signalPrint(QString&);
    void signalAddServerFileList(QString &);
    void signalDownload(QString &);

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer(QByteArray &);
    void slotConnected();
    void slotTest();
    void slotConnectToHost(QString & , int& );
    void slotMakeDataConnection(int&);
    void slotLogIn(QString&, QString&);
    void slotShowServerFileList();
    void slotDownload(QString &);
    void slotCd(QString &);
    void slotPut(QByteArray &, QString &);
    void slotRename(QString &, QString &);
    void slotMkDir(QString &);
    void slotRmFile(QString &);
    void slotRmDir(QString &);

    void slotDownloadAll();

    void slotAddNextFile(Session *);


};

#endif // FTPCLIENT_H
