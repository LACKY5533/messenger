#ifndef MESSENGER_H
#define MESSENGER_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>

namespace Ui {
class messenger;
}

class Messenger : public QDialog
{
    Q_OBJECT

public:
    explicit Messenger(QWidget *parent = nullptr);
    ~Messenger();

private:
    Ui::messenger *ui;
    QTcpSocket *socket;
    QByteArray Data;
    quint16 nextBlockSize;
    QString nickname;
    void SendToServer(QString login, QString str);

private slots:
    void on_connectButton_clicked();

    void on_sendButton_clicked();

    void on_messageOfClient_returnPressed();

public slots:
    void slotReadyRead();
    void recieveNickname(QString nickname); //слот для приёма никнейма
};

#endif // MESSENGER_H
