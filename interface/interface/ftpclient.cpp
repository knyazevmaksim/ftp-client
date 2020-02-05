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
    QTextStream in(TcpSocketCommand);
    QString str;
    //qint64 tmp;
    while(!in.atEnd())
    {
        str+=in.readLine();
        str+='\n';
    }
    info->append(str);
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
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out<<input->text();
    for(int i=0; i<data.size()-1; i++)
    {
        if(data[i]=='\0')
        {
            data=data.remove(i,1);
            i--;
        }
    }
    data=data.remove(0,1);
    data.append('\0');
    TcpSocketCommand->write(data);
    input->setText("");
}


void FtpClient::slotConnected()
{
    info->append("Recieved the connected() signal");
}
