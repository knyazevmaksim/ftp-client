#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
#include <QWidget>
#include <QTcpSocket>
#include <QFile>


class FtpClient : public QWidget
{
    Q_OBJECT

private:
    QTcpSocket* TcpSocketCommand;
    QTcpSocket* TcpSocketData;
    QString hostName;
    int passivePort;
    QString serverFileList;
    int getPassivePort(QString &);
    QString getServerFile(QString &);
    bool isServerFileList;
    bool download;
    QString fileName;
    QByteArray QStringToQByteArray (const QString &);

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

};

#endif // FTPCLIENT_H
