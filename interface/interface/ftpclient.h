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
    void slotLogIn(QByteArray&, QByteArray&);
    void slotShowServerFileList();
    void slotDownloadTextFile(QString &);
    void slotDownload(QString &);

};

#endif // FTPCLIENT_H
