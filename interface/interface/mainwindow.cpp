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
    connect(this, SIGNAL(signalGetHostAndIp(QString &, int)),p_ftpClient,SLOT(slotConnectToHost(QString &, int)));
    connect(ui->pushButton, SIGNAL(clicked()),SLOT(slotSendMessage()));
    connect(ui->lineEdit_3, SIGNAL(returnPressed()),SLOT(slotSendMessage()));
    connect(this, SIGNAL(signalGetMessage(QByteArray&)),p_ftpClient, SLOT(slotSendToServer(QByteArray&)));
    connect(p_ftpClient,SIGNAL(signalPrint(QString&)) , SLOT(slotPrint(QString&)));
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
