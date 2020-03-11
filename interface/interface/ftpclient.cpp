#include "ftpclient.h"

#include <sstream>

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
        passivePort=getPassivePort();
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
    TcpSocketCommand->waitForBytesWritten();
    TcpSocketCommand->waitForReadyRead();
    data=TcpSocketCommand->readAll();
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

void FtpClient:: slotLogIn(QString& name, QString & pass)
{
    login(name, pass);
    //test();
}

void FtpClient::slotShowServerFileList()
{
    isServerFileList=true;
    QByteArray command, data;
    QString str,tmp;
    int start{0}, end{0};
    command="pwd ";
    command+='\0';
    data=readAllCommand();
    slotSendToServer(command);
    data=readAllCommand();
    str=data;
    start=str.indexOf('"',0);
    end=str.indexOf('"',start+1);
    for(int i=start; i<=end; i++)
    {
        tmp+=str[i];
    }
    list(tmp);
    emit signalAddServerFileList(serverFileList);
}

int FtpClient::getPassivePort()
{
    int port;
    QByteArray tmp, _port;
    int start, end;
    QString str;
    QByteArray command="epsv";
    command.append('\0');
    TcpSocketCommand->write(command);
    TcpSocketCommand->waitForBytesWritten();
    TcpSocketCommand->waitForReadyRead();
    tmp=TcpSocketCommand->readAll();
    start=tmp.indexOf("|||");
    end=tmp.indexOf("|", start+3);
    for(int i=start+3; i<end; i++)
    {
        _port+=tmp[i];
    }
    port=_port.toInt();
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


void FtpClient::slotDownload(QString & name)
{
    QFile file;
    //нужна установка пути
    //QString way="C:/Users/yhwh/Desktop/";
    QString way="C:/Users/knyazev.m/Desktop/";
    way+=name;
    file.setFileName(way);



    isServerFileList=false;
    get(name, &file);
    //getBin(name, &file);
}

void FtpClient::get(const QString & file, QIODevice * device)
{
    QByteArray tmp;
    QString str;
    download=true;
    QByteArray command;
    QByteArray name=QStringToQByteArray(file);
    passivePort=getPassivePort();
    slotMakeDataConnection(passivePort);
    command="retr ";
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
    passivePort=getPassivePort();
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

void FtpClient::cd(const QString & dir)
{
    QByteArray command="cwd ";
    QByteArray tmp=QStringToQByteArray(dir);
    command+=tmp;
    command.append('\0');
    slotSendToServer(command);
}

void FtpClient::slotCd(QString & dir)
{
    cd(dir);
    isServerFileList=true;
    list(dir);
    emit signalAddServerFileList(serverFileList);
}

void FtpClient::put(const QByteArray & data, const QString & file)
{
    QByteArray command;
    passivePort=getPassivePort();
    slotMakeDataConnection(passivePort);
    command="stor ";
    command+=QStringToQByteArray(file);
    command.append('\0');
    slotSendToServer(command);
    TcpSocketData->write(data);
    TcpSocketData->disconnectFromHost();
}

void FtpClient::slotPut(QByteArray & data, QString & file)
{
    QByteArray tmpData;
    //tmpData=QStringToQByteArray(data);
    //put(data, file);
    putBin(data,file);
}


void FtpClient::login(const QString & name, const QString & pass)
{
    QByteArray tName, tPass;
    tName=QStringToQByteArray(name);
    tPass=QStringToQByteArray(pass);
    tName=tName.prepend("user ");
    tName.append('\0');
    tPass=tPass.prepend("pass ");
    tPass=tPass.append('\0');
    slotSendToServer(tName);
    slotSendToServer(tPass);
}

void FtpClient::rename(const QString & oldName, const QString & newName)
{
    QByteArray command, data;
    command="rnfr ";
    command+=QStringToQByteArray(oldName);
    command+='\0';
    slotSendToServer(command);
    data=readAllCommand();

    //emit signalPrint(data);
    command="rnto ";
    command+=QStringToQByteArray(newName);
    command+='\0';
    slotSendToServer(command);
    data=readAllCommand();

    //emit signalPrint(data);

}

void FtpClient::remove(const QString & file)
{
    QByteArray command{"dele "};
    command+=QStringToQByteArray(file);
    command+='\0';
    slotSendToServer(command);
}

void FtpClient::mkDir(const QString & name)
{
    QByteArray command{"mkd "};
    command+=QStringToQByteArray(name);
    command+='\0';
    slotSendToServer(command);
}

void FtpClient::rmDir(const QString & name)
{
    QByteArray command{"rmd "};
    command+=QStringToQByteArray(name);
    command+='\0';
    slotSendToServer(command);
}

void FtpClient::slotRename(QString & oldName, QString & newName)
{
    rename(oldName, newName);
}

void FtpClient::slotMkDir(QString & name)
{
    mkDir(name);
}

void FtpClient::slotRmFile(QString & name)
{
    remove(name);
}

void FtpClient::slotRmDir(QString & name)
{
    rmDir(name);
}

void FtpClient::getBin(const QString & file, QIODevice * device )
{
    QByteArray data;
    QByteArray command="epsv";
    QByteArray name=QStringToQByteArray(file);
    command.append('\0');
    slotSendToServer(command);
    slotMakeDataConnection(passivePort);
    command="type i";
    command.append('\0');
    slotSendToServer(command);
    command="retr ";
    command+=name;
    command.append('\0');
    slotSendToServer(command);
    TcpSocketData->waitForReadyRead();
    int bytes=TcpSocketData->bytesAvailable();
    data.resize(bytes);
    device->open(QIODevice::Append);
    while(bytes!=0)
    {
        data=TcpSocketData->readAll();
        device->write(data);
        TcpSocketData->waitForReadyRead();
        bytes=TcpSocketData->bytesAvailable();
        data.resize(bytes);
    }
}

void FtpClient::putBin(const QByteArray & data, const QString & file)
{
    QByteArray command;
    QString str;
    passivePort=getPassivePort();
    slotMakeDataConnection(passivePort);
    command="type i";
    command.append('\0');
    slotSendToServer(command);
    command="stor ";
    command+=QStringToQByteArray(file);
    command.append('\0');
    slotSendToServer(command);
    TcpSocketData->write(data);
    TcpSocketData->disconnectFromHost();
}

void FtpClient::slotDownloadAll()
{
    QFile file;
    //нужна установка пути
    //QString way="C:/Users/yhwh/Desktop/";
    QString way="C:/Users/knyazev.m/Desktop/";
    QFile file2;
    QFile file3;
    QList <QFile*> files;

    QList<QString> lst;
    lst<<"test.txt"<<"test2.txt"<<"test3.txt";
    way+=lst[0];
    file.setFileName(way);
    way="C:/Users/knyazev.m/Desktop/";
    way+=lst[1];
    file2.setFileName(way);
    way="C:/Users/knyazev.m/Desktop/";
    way+=lst[2];
    file3.setFileName(way);
    std::vector <std::thread> v;
    files<<&file<<&file2<<&file3;
    int port{0};
    QByteArray command="retr ";
    QByteArray name;

    //попытка каждый раз устанавливать новый порт на стороне клиента для нового дата коннекта

    QHostAddress ip=TcpSocketCommand->localAddress();
    Q_IPV6ADDR ipV6=ip.toIPv6Address();
    QString addr="|2|";
    /*for(int i=0; i<16;++i)
    {
        addr+=ipV6[i];
    }*/
    addr+=ip.toString();


    for (int i=0;i<2;i++)
    {

        /*command="eprt ";
        command+=addr;

        if(i==0)
            command+="|5282|";
        else
            command+="|5283|";
        command+='\0';

        TcpSocketCommand->write(command);
        TcpSocketCommand->waitForReadyRead(-1);
                command=TcpSocketCommand->readAll();*/
        command="retr ";



        name=QStringToQByteArray(lst[i]);
        command+=name;
        command+='\0';
        //*/
        port=getPassivePort();

        std::thread tmp(&FtpClient::get_test,this, port, files[i],i+1);
        TcpSocketCommand->write(command);
        TcpSocketCommand->waitForReadyRead(-1);
        command=TcpSocketCommand->readAll();
        command="retr ";
        v.push_back(std::move(tmp));


    }

    /*for (int i=0; i<2;++i)
    {
        command="retr ";



        name=QStringToQByteArray(lst[i]);
        command+=name;
        command+='\0';

        TcpSocketCommand->write(command);
        TcpSocketCommand->waitForReadyRead(-1);
        command=TcpSocketCommand->readAll();
        command="retr ";


    }*/

    v[0].join();
    v[1].join();
    //v[2].join();

}

void FtpClient::get_test(int port, QIODevice * device, int type)
{ 
        /*if (type==2)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }//*/
        QByteArray name;
        QTcpSocket data;
        data.connectToHost(hostName,port);
        data.waitForConnected();
        QTcpSocket::SocketState state=data.state();
        data.waitForReadyRead();
        int bytes=data.bytesAvailable();
        device->open(QIODevice::WriteOnly);

        auto myid = std::this_thread::get_id();
        std::stringstream ss;
        ss << myid;
        std::string mystring = ss.str();

        while(bytes!=0)
        {
            qDebug() << mystring.c_str() << bytes;
            name.resize(bytes);
            name=data.readAll();
            device->write(name);
            data.waitForReadyRead();
            bytes=data.bytesAvailable();
            if (bytes==0)
            {
                qDebug() << mystring.c_str() << bytes << "Error";
                slotError(data.error());
                state=data.state();
                int tmp=bytes;
            }

        }

        qDebug() << mystring.c_str() << bytes;


}

void FtpClient::test()
{
    std::vector<QTcpSocket*> v;
    QTcpSocket * tmp;

    int port;
    for(int i=0; i<10; ++i)
    {
        tmp=new QTcpSocket();
        v.push_back(tmp);
        port=getPassivePort();
        tmp->connectToHost(hostName,port);
        tmp->waitForConnected();
    }
    QTcpSocket::SocketState state;
    for(int i=0; i<10; ++i)
    {
        state=v[i]->state();
        std::cout<<state<<std::endl;
    }

}








