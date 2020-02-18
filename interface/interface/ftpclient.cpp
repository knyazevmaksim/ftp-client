#include "ftpclient.h"

FtpClient::FtpClient(QWidget* pwgt):QWidget(pwgt), isServerFileList{false}, download{false}
{
     TcpSocketCommand=new QTcpSocket();
     TcpSocketData=new QTcpSocket();
     connect(TcpSocketCommand, SIGNAL(connected()), SLOT(slotConnected()));
    // connect(TcpSocketCommand, SIGNAL(readyRead()), SLOT(slotReadyRead()));
     connect(TcpSocketCommand, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));

    // connect(TcpSocketData, SIGNAL(readyRead()), SLOT(slotTest()));
     connect(TcpSocketData, SIGNAL(connected()), SLOT(slotConnected()));
     //connect(TcpSocketData, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));


     connect(this, SIGNAL(signalDownload(QString &)), SLOT(slotDownloadTextFile(QString &)));
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
    if (isServerFileList||download)
    {
        passivePort=getPassivePort(str);
    }
    emit signalPrint(str);
    /*if (str.startsWith("150"))
        isServerFileList=true;*///условие работает когда не нужно
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
    TcpSocketCommand->flush();
    TcpSocketCommand->waitForReadyRead();


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
    if (isServerFileList)
    {
        serverFileList=getServerFile(str);
        isServerFileList=false;
    }
    if (download)
        emit signalDownload(str);
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
    isServerFileList=true;
    /*download=true;
    QByteArray command="epsv";
    command+='\0';
    slotSendToServer(command);

    //обработка строки ответа сервера с номером порта для data соединения
    //connect data соединения
    TcpSocketData->connectToHost(hostName, passivePort);
    //команда list
    command="list";
    command+='\0';
    slotSendToServer(command);
    TcpSocketData->waitForReadyRead();
    //вывод ее результатов в QListWidget2
    isServerFileList=false;
    download=false;
    */
    list("/");
    emit signalAddServerFileList(serverFileList);





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

QString FtpClient::getServerFile(QString& str)
{
    QString tmp;
    int start{0}, end{0};
    while(end!=str.size()-1)
    {
        start=str.indexOf(":", start)+3;
        end=str.indexOf("\n",end+1);
        for(int i=start; i<=end; i++)
        {
           tmp+=str[i];
        }
    }
    return tmp;
}

void FtpClient::slotDownloadTextFile(QString & data)
{
    QFile file;
    file.setFileName("C:/Users/knyazev.m/Desktop/"+fileName);
    file.open(QIODevice::WriteOnly);
    QByteArray tmp;
    QDataStream out(&tmp, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out<<data;
    for(int i=0; i<tmp.size()-1; i++)
    {
        if(tmp[i]=='\0')
        {
            tmp=tmp.remove(i,1);
            i--;
        }
    }
    tmp=tmp.remove(0,1);
    tmp.append('\0');

    file.write(tmp);
    file.close();
}

void FtpClient::slotDownload(QString & name)
{
    QFile file;
    //file.setFileName("C:/Users/knyazev.m/Desktop/test.txt");
    file.setFileName("C:/Users/yhwh/Desktop/test.txt");


    isServerFileList=false;
    get(name, &file);

    /*download=true;
    QByteArray command="epsv";
    command.append('\0');
    TcpSocketCommand->write(command);
    TcpSocketData->connectToHost(hostName, 50001);
    QByteArray tmp;
    QDataStream out(&tmp, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out<<name;
    for(int i=0; i<tmp.size()-1; i++)
    {
        if(tmp[i]=='\0')
        {
            tmp=tmp.remove(i,1);
            i--;
        }
    }
    tmp=tmp.remove(0,1);
    command="retr";
    command+=tmp;
    command.append('\0');
    TcpSocketCommand->write(command);
    fileName=name;*/
}

void FtpClient::get(const QString & file, QIODevice * device)
{
    download=true;
    QByteArray command="epsv";
    QByteArray name=QStringToQByteArray(file);
    command.append('\0');
    slotSendToServer(command);
    slotMakeDataConnection(passivePort);
    command="retr";
    command+=name;
    command.append('\0');
    slotSendToServer(command);
    command=readAllData();
    device->open(QIODevice::WriteOnly);
    device->write(command);

}

QByteArray FtpClient::QStringToQByteArray (const QString & str)
{
    QByteArray tmp;
    QDataStream out(&tmp, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out<<str;
    for(int i=0; i<tmp.size()-1; i++)
    {
        if(tmp[i]=='\0')
        {
            tmp=tmp.remove(i,1);
            i--;
        }
    }
    tmp=tmp.remove(0,1);
    return tmp;
}

QByteArray FtpClient::readAllData()
{
    TcpSocketData->waitForReadyRead();
    QByteArray data="";
    data=TcpSocketData->readAll();
    return data;
}

void FtpClient::list(const QString & dir)
{
    QByteArray command,tmp;
    QString str;
    command="epsv";
    command+='\0';
    slotSendToServer(command);
    tmp=readAllCommand();
    str=QStringToQByteArray(tmp);
    passivePort=getPassivePort(str);
    slotMakeDataConnection(passivePort);
    command="list ";
    command+=QStringToQByteArray(dir);
    command+='\0';
    slotSendToServer(command);
    tmp=readAllData();//необработанный список файлов сервера в tmp


    //его можно обработать и отобразить
    str=QStringToQByteArray(tmp);
    str=getServerFile(str);
    serverFileList=str;

}

QByteArray FtpClient::readAllCommand()
{
    QByteArray data="";
    data=TcpSocketCommand->readAll();
    return data;
}





























