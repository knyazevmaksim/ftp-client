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
    connect(this, SIGNAL(signalGetUserNameAndPass(QString &, QString &)), p_ftpClient, SLOT(slotLogIn(QString &, QString &)));
    //connect(this, SIGNAL(signalGetUserNameAndPass(QByteArray &, QByteArray &)), p_ftpClient, SLOT(slotShowServerFileList()));


    connect(ui->pushButton_4, SIGNAL(clicked()), p_ftpClient, SLOT(slotShowServerFileList()));


    connect(p_ftpClient, SIGNAL(signalAddServerFileList(QString&)), SLOT(slotAddServerFileList(QString&)));


    connect(ui->downLoadButton,SIGNAL(clicked()), SLOT(slotSendDownloadTextSignal()));
    connect(this, SIGNAL(signalDownloadText(QString &)), p_ftpClient, SLOT(slotDownload(QString &)));
    connect(ui->listWidget_2, SIGNAL(itemClicked(QListWidgetItem *)), SLOT(slotGetChosenItem(QListWidgetItem *)));


    connect(ui->listWidget_2, SIGNAL(itemDoubleClicked(QListWidgetItem *)), SLOT(slotSendCd(QListWidgetItem *)));
    connect(this, SIGNAL(signalCd(QString &)), p_ftpClient, SLOT(slotCd(QString &)));


    connect(ui->pushButton_5, SIGNAL(clicked()), SLOT(slotSendSignalUpload()));
    connect(this, SIGNAL(signalUpload(QByteArray &, QString &)), p_ftpClient, SLOT(slotPut(QByteArray &, QString &)));


    connect(ui->pushButton_7, SIGNAL(clicked()), SLOT(slotSendSignalRename()));
    connect(this, SIGNAL(signalRename(QString&, QString &)), p_ftpClient, SLOT(slotRename(QString &, QString &)));

    connect(ui->pushButton_8, SIGNAL(clicked()), SLOT(slotSendSignalMkDir()));
    connect(this, SIGNAL(signalMkDir(QString &)), p_ftpClient, SLOT(slotMkDir(QString&)));

    connect(ui->pushButton_9, SIGNAL(clicked()), SLOT(slotSendSignalRmFile()));
    connect(this, SIGNAL(signalRmFile(QString &)), p_ftpClient, SLOT(slotRmFile(QString &)));


    connect(ui->pushButton_10, SIGNAL(clicked()), SLOT(slotSendSignalRmDir()));
    connect(this, SIGNAL(signalRmDir(QString &)), p_ftpClient, SLOT(slotRmDir(QString &)));
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
    QString name;
    QString pass;
    name=ui->lineEdit_5->text();
    pass=ui->lineEdit_6->text();

    emit signalGetUserNameAndPass(name, pass);
}

void MainWindow::slotAddServerFileList(QString& str)
{
    QString tmp;
    int start{0}, end{0};
    int count=ui->listWidget_2->count();
    for (int i=0; i<=count; i++)
        ui->listWidget_2->takeItem(0);
    //очистить список файлов
    while(end!=str.size()-1)
    {
        end=str.indexOf("\n",end+1);
        for(int i=start; i<end; i++)
        {
           tmp+=str[i];
        }
        start=end+1;
        ui->listWidget_2->addItem(tmp);
        tmp="";
    }
}

void MainWindow::slotSendDownloadTextSignal()
{
    emit signalDownloadText(fileName);
}

void MainWindow::slotGetChosenItem(QListWidgetItem * item)
{
    fileName=item->text();
    fileName=fileName.remove(0,1);
    fileName=fileName.remove(fileName.size()-1,1);
}

void MainWindow::slotSendCd(QListWidgetItem* item)
{
    QString tmp;
    tmp=item->text();
    tmp=tmp.remove(0,1);
    tmp.prepend("/");
    //tmp=tmp.remove(tmp.size()-1,1);
    emit signalCd(tmp);
}

void MainWindow::slotSendSignalUpload()
{
    QFile file;
    file.setFileName("C:/Users/yhwh/Desktop/miyagi-endshpil_-_kosandra.mp3");
    file.open(QIODevice::ReadOnly);
    QByteArray data;
    data=file.readAll();

    QString name="miyagi-endshpil_-_kosandra.mp3";
    emit signalUpload(data,name);
}

void MainWindow::slotSendSignalRename()
{
    QString tmp=ui->lineEdit_7->text();
    emit signalRename(fileName, tmp);
}

void MainWindow::slotSendSignalMkDir()
{
    QString tmp=ui->lineEdit_8->text();
    emit signalMkDir(tmp);
}

void MainWindow::slotSendSignalRmFile()
{
    emit signalRmFile(fileName);
}

void MainWindow::slotSendSignalRmDir()
{
    emit signalRmDir(fileName);
}








