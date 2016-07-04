#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QVector>
#include <QMap>
#include <QSet>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void startServer();
signals:

public slots:
    void readyRead();
    void disconnected();
    void sendUserList();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QSet<QTcpSocket*> clients;
    QMap<QTcpSocket*,QString> users;
    int identificator;
    QStringList list;
};

#endif // SERVER_H
