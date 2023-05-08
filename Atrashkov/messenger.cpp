#include "messenger.h"
#include "ui_messenger.h"
#include "users.h"

Messenger::Messenger(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::messenger)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Messenger::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &QTcpServer::deleteLater);
    connect(ui->connButton, SIGNAL(clicked()), this, SLOT(on_connectButton_clicked()));
    connect(ui->sendMessageButton, SIGNAL(clicked()), this, SLOT(on_sendButton_clicked()));
    nextBlockSize = 0;
}

Messenger::~Messenger()
{
    delete ui;
}

void Messenger::on_connectButton_clicked()
{
    socket->connectToHost("95.165.6.147", 55555);
}

void Messenger::SendToServer(QString login, QString str) //функция отправки сообщения серверу
{
    Users users;
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_4);
    out << quint16(0) << QTime::currentTime() << login << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
    ui->messageOfClient->clear();
}

void Messenger::slotReadyRead() //функция принятия сообщения от сервера
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_4);

    if (in.status() == QDataStream::Ok)
    {
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
            login = this->nickname;
            nextBlockSize = 0;
            ui->allMessages->append(time.toString() + " пользователь " + login + " написал: " + str);
        }
    } else  {
        ui->allMessages->append("ошибка чтения");
    }
}

void Messenger::recieveNickname(QString nickname)
{
    ui->nicknameLabel->setText(nickname);
    this->nickname = nickname;
}


void Messenger::on_sendButton_clicked() // Функция для отправки сообщения с помощью кнопки "Отправить"
{
    SendToServer(ui->nicknameLabel->text(), ui->messageOfClient->text());
}


void Messenger::on_messageOfClient_returnPressed() //функция для отправки сообщения с помощью клавиши ENTER
{
    SendToServer(ui->nicknameLabel->text(), ui->messageOfClient->text());
}

