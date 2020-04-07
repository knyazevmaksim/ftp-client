#include "session.h"

Session::Session(QString ip, int port):ip{ip}, port{port}, status {true}, state{QAbstractSocket::UnconnectedState}
{


    commandSocket=new QTcpSocket;
    dataSocket=nullptr;

    connect(commandSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), SLOT(slotStateChange(QAbstractSocket::SocketState )));

    commandSocket->connectToHost(ip,port);
    readAllCommand();


}

QByteArray Session::readAllCommand()
{
    QByteArray data="";
    commandSocket->waitForReadyRead();
    data=commandSocket->readAll();
    return data;
}

int Session::getPassivePort()
{
    int port;
    QByteArray tmp, _port;
    int start, end;
    QString str;
    QByteArray command="epsv";
    command.append('\0');
    commandSocket->write(command);
    commandSocket->waitForBytesWritten();
    commandSocket->waitForReadyRead();
    tmp=commandSocket->readAll();
    start=tmp.indexOf("|||");
    end=tmp.indexOf("|", start+3);
    for(int i=start+3; i<end; i++)
    {
        _port+=tmp[i];
    }
    port=_port.toInt();
    return port;
}


void Session::sendCommand(QByteArray & command)
{
    commandSocket->write(command);
    commandSocket->waitForBytesWritten();
    readAllCommand();
}

void Session::download(QString way, QByteArray fileName)
{
    //статус сессии = "занята"
    status=false;
    if(dataSocket==nullptr)
        dataSocket=new QTcpSocket;
    QByteArray data, command;
    QString fullWay=way;
    fullWay+=fileName;
    QFile file;
    int bytes;
    file.setFileName(fullWay);
    port=getPassivePort();
    dataSocket->connectToHost(ip, port);



    command="retr ";
    command+=fileName;
    command+='\0';
    dataSocket->waitForConnected();
    sendCommand(command);

    dataSocket->waitForReadyRead();
    file.open(QIODevice::WriteOnly);

    bytes=dataSocket->bytesAvailable();
    while(bytes!=0)
    {
        data.resize(bytes);
        data=dataSocket->readAll();
        file.write(data);
        dataSocket->waitForReadyRead();
        bytes=dataSocket->bytesAvailable();
    }
    file.close();

    //статус сессии "свободна"
    dataSocket->disconnectFromHost();
    status=true;
    emit signalFree(this);
}


bool Session::login(const QString & name, const QString & pass)
{
    QByteArray tName, tPass;
    tName="user ";
    tName+=name;
    tName.append('\0');
    tPass="pass ";
    tPass+=pass;
    tPass=tPass.append('\0');
    sendCommand(tName);
    sendCommand(tPass);
    commandSocket->waitForDisconnected(1000);
    if (state==QAbstractSocket::ConnectedState)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Session::free()
{
    return status;
}

void Session::slotStateChange(QAbstractSocket::SocketState st)
{
    state=st;
}

void Session::disconnect()
{
    commandSocket->disconnectFromHost();
}

















