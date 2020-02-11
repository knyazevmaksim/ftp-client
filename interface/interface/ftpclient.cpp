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
    if (str.startsWith("229"))
        passivePort=getPassivePort(str);
    if (str.startsWith("150"))
    {

        /*serverFileList=getServerFile(str);
        emit signalAddServerFileList(serverFileList, numberFiles);*/
    }
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

void FtpClient:: slotLogIn(QByteArray& name, QByteArray & pass)
{
    name=name.prepend("user ");
    pass=pass.prepend("pass ");
    TcpSocketCommand->write(name);
    TcpSocketCommand->write(pass);
}

void FtpClient::slotShowServerFileList()
{
    TcpSocketCommand->write("epsv");
    //обработка строки ответа сервера с номером порта для data соединения

    //connect data соединения
    TcpSocketData->connectToHost(hostName, passivePort);
    //команда list
    TcpSocketCommand->write("list");
    //вывод ее результатов в QListWidget2

    emit signalAddServerFileList(serverFileList, numberFiles);





}

int FtpClient::getPassivePort(QString & str)
{
    int port;
    QString tmp;
    int start, end;
    start=str.indexOf("|||");
    end=str.indexOf("|", start+3);
    for(int i=start+3; i<end; i++)
    {
        tmp+=str[i];
    }
    port=tmp.toInt();
    return port;
}

QString* FtpClient::getServerFile(QString& str)
{
    QString * fileName;
    QString tmp;
    int start{0}, end{0};
    int count{0};
    while(end!=str.size())
    {
        start=str.indexOf(":", start)+3;
        end=str.indexOf("\n",end+1);
        for(int i=start; i<=end; i++)
        {
           tmp+=str[i];
        }
    count++;
    }
    fileName=new QString(count);
    numberFiles=count;
    for(int i=0; i<count; i++)
    {
        int j=0;
        while(tmp[j]!="\n")
        {
            fileName[i][j]=tmp[j];
            ++j;
        }
    }
    return fileName;
}





