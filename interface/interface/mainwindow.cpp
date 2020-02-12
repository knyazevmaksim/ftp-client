#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, FtpClient* ptr) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), p_ftpClient(ptr)
{
    ui->setupUi(this);
    //connect(ui->pushButton_2, SIGNAL(clicked()),p_ftpClient,SLOT(slotTest()));
    connect(ui->pushButton_2, SIGNAL(clicked()),SLOT(slotSendSignalHostAndIp()));
    connect(ui->lineEdit, SIGNAL(returnPressed()),SLOT(slotSendSignalHostAndIp()));
    connect(ui->lineEdit_2, SIGNAL(returnPressed()),SLOT(slotSendSignalHostAndIp()));

    connect(this, SIGNAL(signalGetHostAndIp(QString &, int&)),p_ftpClient,SLOT(slotConnectToHost(QString &, int&)));

    connect(ui->pushButton, SIGNAL(clicked()),SLOT(slotSendMessage()));
    connect(ui->lineEdit_3, SIGNAL(returnPressed()),SLOT(slotSendMessage()));
    connect(this, SIGNAL(signalGetMessage(QByteArray&)),p_ftpClient, SLOT(slotSendToServer(QByteArray&)));

    connect(p_ftpClient,SIGNAL(signalPrint(QString&)) , SLOT(slotPrint(QString&)));

    connect(ui->lineEdit_4, SIGNAL(returnPressed()), SLOT(slotMakeDataConnection()));
    connect(ui->pushButton_3, SIGNAL(clicked()), SLOT(slotMakeDataConnection()));
    connect(this, SIGNAL(signalMakeDataConnection(int&)), p_ftpClient, SLOT(slotMakeDataConnection(int&)));


    connect(ui->pushButton_6, SIGNAL(clicked()), SLOT(slotSendUserNameAndPass()));
    connect(ui->lineEdit_5, SIGNAL(returnPressed()), SLOT(slotSendUserNameAndPass()));
    connect(ui->lineEdit_6, SIGNAL(returnPressed()), SLOT(slotSendUserNameAndPass()));
    connect(this, SIGNAL(signalGetUserNameAndPass(QByteArray &, QByteArray &)), p_ftpClient, SLOT(slotLogIn(QByteArray &, QByteArray &)));
    //connect(this, SIGNAL(signalGetUserNameAndPass()), p_ftpClient, SLOT(slotShowServerFileList()));

    connect(p_ftpClient, SIGNAL(signalAddServerFileList(QString&)), SLOT(slotAddServerFileList(QString&)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotSendSignalHostAndIp()
{
    QString hostname=ui->lineEdit->text();
    QString ip=ui->lineEdit_2->text();
    int intIp=ip.toInt();
    emit signalGetHostAndIp(hostname, intIp);
}

void MainWindow::slotSendMessage()
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out<<ui->lineEdit_3->text();
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
    ui->lineEdit_3->setText("");
    emit signalGetMessage(data);
}

void MainWindow::slotPrint(QString & str)
{
    ui->textBrowser->append(str);
}

void MainWindow::slotMakeDataConnection()
{

    int port=ui->lineEdit_4->text().toInt();
    emit signalMakeDataConnection(port);
}

void MainWindow::slotSendUserNameAndPass()
{
    QByteArray name;
    QByteArray pass;
    QDataStream out(&name, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out<<ui->lineEdit_5->text();
    for(int i=0; i<name.size()-1; i++)
    {
        if(name[i]=='\0')
        {
            name=name.remove(i,1);
            i--;
        }
    }
    name=name.remove(0,1);
    name.append('\0');
    QDataStream out1(&pass, QIODevice::WriteOnly);
    out1.setVersion(QDataStream::Qt_5_0);
    out1<<ui->lineEdit_6->text();
    for(int i=0; i<pass.size()-1; i++)
    {
        if(pass[i]=='\0')
        {
            pass=pass.remove(i,1);
            i--;
        }
    }
    pass=pass.remove(0,1);
    pass.append('\0');

    emit signalGetUserNameAndPass(name, pass);
}

void MainWindow::slotAddServerFileList(QString& str)
{
    QString tmp;
    int start{0}, end{0};
    //очистить список файлов
    while(end!=str.size()-1)
    {
        end=str.indexOf("\n",end+1);
        for(int i=start; i<=end; i++)
        {
           tmp+=str[i];
        }
        start=end;
        ui->listWidget_2->addItem(tmp);
        tmp="";
    }

}














