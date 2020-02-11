#include "ftpclient.h"

FtpClient::FtpClient(QWidget* pwgt):QWidget(pwgt)
{
     TcpSocketCommand=new QTcpSocket();
     TcpSocketData=new QTcpSocket();
     connect(TcpSocketCommand, SIGNAL(connected()), SLOT(slotConnected()));
     connect(TcpSocketCommand, SIGNAL(readyRead()), SLOT(slotReadyRead()));
     connect(TcpSocketCommand, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));



     connect(TcpSocketData, SIGNAL(readyRead()), SLOT(slotTest()));
}

FtpClient::FtpClient(const QString& strHost, int port, QWidget* pwgt): QWidget(pwgt)
{
    TcpSocketCommand=new QTcpSocket();
    TcpSocketCommand->connectToHost(strHost, port);
    connect(TcpSocketCommand, SIGNAL(connected()), SLOT(slotConnected()));
    connect(TcpSocketCommand, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(TcpSocketCommand, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

    info=new QTextEdit;
    input=new QLineEdit;

    info->setReadOnly(true);

    button=new QPushButton("&Send");

    connect(button, SIGNAL(clicked()), SLOT(slotSendToServer()));
    connect(input, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));


    QVBoxLayout* vbxlayout = new QVBoxLayout;
    vbxlayout->addWidget(new QLabel("<H1>Client</H1>"));
    vbxlayout->addWidget(info);
    vbxlayout->addWidget(input);
    vbxlayout->addWidget(button);
    setLayout(vbxlayout);

}

void FtpClient::slotReadyRead()
{
    QTextStream in(TcpSocketCommand);
    QString str;
    //qint64 tmp;
    while(!in.atEnd())
    {
        str+=in.readLine();
        str+='\n';
    }
    emit signalPrint(str);
}


void FtpClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError="Error: " + (err==QAbstractSocket::HostNotFoundError ? "The host was not found."
                             :err==QAbstractSocket::RemoteHostClosedError ? "The remote host is closed."
                         :err==QAbstractSocket::ConnectionRefusedError ? "The connection was refused."
                                                                       :QString(TcpSocketCommand->errorString()));
    emit signalPrint(strError);
}


void FtpClient::slotSendToServer(QByteArray & data)
{

    TcpSocketCommand->write(data);

}


void FtpClient::slotConnected()
{
    QString str="Recieved the connected() signal";
    emit signalPrint(str);
}


void FtpClient::slotConnectToHost(QString & ip, int& port)
{
    hostName=ip;
    TcpSocketCommand->connectToHost(hostName, port);
}

void FtpClient::slotMakeDataConnection(int& port)
{
    TcpSocketData->connectToHost(hostName, port);
}

void FtpClient::slotTest()
{
    QTextStream in(TcpSocketData);
    QString str;
    //qint64 tmp;
    while(!in.atEnd())
    {
        str+=in.readLine();
        str+='\n';
    }
    emit signalPrint(str);
}
