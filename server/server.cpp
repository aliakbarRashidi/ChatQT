#include "server.h"
#include <QTcpSocket>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
}

void Server::startServer()
{
    int port = 1234;

    if(!this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";
    }
}

void Server::incomingConnection(qintptr socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    clients.insert(client);

    qDebug() << "New client from:" << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}


void Server::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    while(client->canReadLine())
    {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        qDebug() << "Read line:" << line;

        QRegExp meRegex("^/me:(.*)$");

        if(meRegex.indexIn(line) != -1)
        {
            QString user = meRegex.cap(1);
            users[client] = user;

            foreach(QTcpSocket *client, clients)
                client->write(QString("Server:" + user + " has joined.\n").toUtf8());
            sendUserList();
        }
        else if(users.contains(client))
        {

            list = line.split('/');
            qDebug() << "[0]: "<< list[0];
            qDebug() << "[1]: "<< list[1];

            QString message = line;
            QString user = users[client];

            qDebug() << "User:" << user;
            qDebug() << "Message:" << message;

            foreach(QTcpSocket *otherClient, clients){
                if(users[otherClient] == list[0]){
                    otherClient->write(QString(user + ":" + message + "\n").toUtf8());
                } else if(list[0] == "All") {
                    otherClient->write(QString(user + ":" + message + "\n").toUtf8());
                }
            }


        }
        else
        {
            qWarning() << "Got bad message from client:" << client->peerAddress().toString() << line;
        }
    }
}

void Server::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    clients.remove(client);

    QString user = users[client];
    foreach(QTcpSocket *client, clients)
        client->write(QString("Server:" + user + " has been disconnected.\n").toUtf8());
    users.remove(client);
    sendUserList();
}

void Server::sendUserList()
{
    QStringList userList;
    foreach(QString user, users.values())
        userList << user;

    foreach(QTcpSocket *client, clients)
        client->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
}
