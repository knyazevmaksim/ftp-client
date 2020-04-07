#ifndef SESSION_H
#define SESSION_H

#include <QWidget>
#include <QString>
#include <QTcpSocket>
#include <thread>
#include <QFile>

class Session: public QWidget
{
    Q_OBJECT
public:
    Session(QString ip="", int=0);
    QByteArray readAllCommand();
    int getPassivePort();
    void sendCommand(QByteArray&);
    void download(QString , QByteArray);
    bool login(const QString &, const QString &);
    bool free();
    void disconnect();

private:
    QString ip;
    int port;
    QTcpSocket * commandSocket;
    QTcpSocket * dataSocket;
    //true=свободна, false=занята
    bool status;
    QAbstractSocket::SocketState state;


signals:
    void signalFree(Session*);

private slots:
    void slotStateChange(QAbstractSocket::SocketState);



};

#endif // SESSION_H
