#include "ftpclient.h"


FtpClient::FtpClient(const QString& strHost, int port, QWidget* pwgt): QWidget(pwgt), nextBlockSize(0)
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
    QDataStream in(TcpSocketCommand);
    for(;;)
    {
        if(!nextBlockSize)
        {
            if(TcpSocketCommand->bytesAvailable()<sizeof(quint16))
                break;
            in>>nextBlockSize;
        }
        if(TcpSocketCommand->bytesAvailable()<nextBlockSize)
            break;
       // QTime time;
        QString str;
        in>>str;
        info->append(str);
        nextBlockSize=0;
    }
}


void FtpClient::slotError(QAbstractSocket::SocketError err)
{
    QString strError="Error: " + (err==QAbstractSocket::HostNotFoundError ? "The host was not found."
                             :err==QAbstractSocket::RemoteHostClosedError ? "The remote host is closed."
                         :err==QAbstractSocket::ConnectionRefusedError ? "The connection was refused."
                                                                       :QString(TcpSocketCommand->errorString()));
    info->append(strError);
}


void FtpClient::slotSendToServer()
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out<<input->text();
    out.device()->seek(0);
    out<<quint16(arrBlock.size()-sizeof(quint16));
    TcpSocketCommand->write(arrBlock);
    input->setText("");
}


void FtpClient::slotConnected()
{
    info->append("Recieved the connected() signal");
}
