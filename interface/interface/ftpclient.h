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
    QTextEdit* info;
    QLineEdit* input;
    QPushButton *button;
    QString hostName;


public:

    FtpClient(const QString& strHost, int port, QWidget* pwgt=0);
    FtpClient(QWidget* pwgt=0);

signals:
    void signalPrint(QString&);

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer(QByteArray &);
    void slotConnected();
    void slotTest();
    void slotConnectToHost(QString & , int& );
    void slotMakeDataConnection(int&);

};

#endif // FTPCLIENT_H
