#include "server.h"

Server::Server(QObject * parent):
    QTcpServer(parent)
{
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(5);
}

void Server::startServer()
{
    if (!this->listen(QHostAddress::Any, 1234))
        qDebug() << "Server could not start";
    else
        qDebug() << "Server started";
}


void Server::incomingConnection(int socketDescriptor)
{
   Runnable * task = new Runnable();
   task->setAutoDelete(true);

   qDebug()<< "COS JEST innConnection";

   task->socketDescriptor = socketDescriptor;
   pool->start(task);

}
