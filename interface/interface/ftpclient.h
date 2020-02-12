#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
#include <QWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>


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

public:

    FtpClient(const QString& strHost, int port, QWidget* pwgt=0);
    FtpClient(QWidget* pwgt=0);

signals:
    void signalPrint(QString&);
    void signalAddServerFileList(QString &);

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

};

#endif // FTPCLIENT_H
