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
    QTextEdit* info;
    QLineEdit* input;
    QPushButton *button;
    quint16 nextBlockSize;

public:

    FtpClient(const QString& strHost, int port, QWidget* pwgt=0);

signals:

private slots:
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();

};

#endif // FTPCLIENT_H
