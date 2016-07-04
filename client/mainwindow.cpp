#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->login->show();
    ui->chat->hide();
    row = "NULL";
    filter_key = "";


    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connected()
{
    socket->write(QString("/me:" + ui->loginEdit->text() + "\n").toUtf8());
}

void MainWindow::readyRead()
{
    while(socket->canReadLine())
    {

        QString line = QString::fromUtf8(socket->readLine()).trimmed();


        QRegExp messageRegex("^([^:]+):(.*)$");
        QRegExp usersRegex("^/users:(.*)$");

        if(usersRegex.indexIn(line) != -1)
        {
            QStringList users = usersRegex.cap(1).split(",");
            ui->listWidget->clear();
            ui->listWidget->addItem("All");
            foreach(QString user, users)
                ui->listWidget->addItem(user);
        }
        else if(messageRegex.indexIn(line) != -1)
        {
            QString user = messageRegex.cap(1);
            QString message = messageRegex.cap(2);
            if(user != "Server")
            {
                split_message = message.split('/');
                ui->msgView->append ("<b>" + user + "</b>: " + split_message[1]);
                ui->msgView2->addItem(user + " : " + split_message[1]);
                message_memory.push_back(user + " : " + split_message[1]);
            }
            else{
                ui->msgView->append ("<b>" + user + "</b>: " + message);
                ui->msgView2->addItem(user + " : " + message);
                message_memory.push_back(user + " : " + message);
            }
        }
    }
}

void MainWindow::on_loginButton_clicked()
{
    socket->connectToHost("127.0.0.1", 1234);
    ui->login->hide();
    ui->chat->show();
}


void MainWindow::on_sendButton_clicked()
{
    QString message = ui->msgEdit->text().trimmed();

    if(!message.isEmpty())
    {
        socket->write(QString(row+ "/" +message + "\n").toUtf8());
    }

    ui->msgEdit->clear();
    ui->msgEdit->setFocus();
}


void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{

    row = index.data().toString();
    if(row == "All")
        filter_key = "";
    else
        filter_key = row+" : ";

    ui->msgView2->clear();

    QRegExp regExp(filter_key, Qt::CaseSensitive, QRegExp::Wildcard);
    QStringList temp = message_memory.filter(regExp);
    ui->msgView2->addItems(temp);
}
