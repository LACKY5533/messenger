#include "server.h"

Server::Server()
{
    if(this->listen(QHostAddress::Any, 55555))
    {
        qDebug() << "сервер запущен";
    } else {
        qDebug() << "ошибка";
    }

    nextBlockSize = 0;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &QTcpServer::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "клиент подключился";
}

void Server::slotReadyRead() //функция принятия сообщения от клиента
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_4);

    if (in.status() == QDataStream::Ok)
    {
        qDebug() << "чтение...";

        for (;;) {
            if (nextBlockSize == 0) {
                if (socket->bytesAvailable() <2) {
                    break;
                }
                in >> nextBlockSize;
            }

            if (socket->bytesAvailable() < nextBlockSize) {
                break;
            }

            QString str, login;
            QTime time;
            in >> time >> login >> str;
            nextBlockSize = 0;
            SendToClient(login, str);
            break;
        }
    } else {
        qDebug() << "ошибка потока данных";
    }
}

void Server::SendToClient(QString login, QString str) //функция отправки сообщения клиенту
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << quint16(0) << QTime::currentTime() << login << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    for (int i = 0; i < Sockets.size(); i++) {
        Sockets[i]->write(Data);
    }
}
