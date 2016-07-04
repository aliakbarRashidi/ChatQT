#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QThreadPool>
#include <QDebug>
#include "runnable.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject * parent =0);
    void startServer();

protected:
    void incomingConnection(int m_descriptor);

signals:

public slots:

private:
    QThreadPool *pool;


};

#endif // SERVER_H
