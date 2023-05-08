#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QMessageBox"
#include "users.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->signInButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->regButton, SIGNAL(clicked()), this, SLOT(on_registerButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    string login = ui->loginLine->text().toStdString();
    string password = ui->passwordLine->text().toStdString();
    Users users;
    Messenger *messenger;

    users.load();

    if (users.find(login) != nullptr) {
        if (users.find(login, password) != nullptr) {
            messenger = new Messenger(this);
            connect(this, SIGNAL(sendNickname(QString)), messenger, SLOT(recieveNickname(QString)));
            emit sendNickname(ui->loginLine->text());
            messenger->show();
        } else {
            QMessageBox::warning(this, "Авторизация", "Неверный пароль!");
            ui->passwordLine->text().clear();
        }
    } else {
        QMessageBox::information(this, "Авторизация", "Такого аккаунта нет, пожалуйста, зарегистрируйтесь");
    }
}

void MainWindow::on_registerButton_clicked()
{
    string login = ui->loginLine->text().toStdString();
    string password = ui->passwordLine->text().toStdString();
    Users users;
    char *llogin = new char[login.length() + 1];
    char *ppassword = new char[password.length() + 1];
    strcpy(llogin, login.c_str());
    strcpy(ppassword, password.c_str());
    users.load();

    if (users.find(login) != nullptr) {
        QMessageBox::warning(this, "Регистрация", "Аккаунт с таким логином уже существует");
    } else {
        users.add(llogin, ppassword);
        users.save();
        QMessageBox::information(this, "Регистрация", "Аккаунт успешно зарегистрирован, можете входить");
    }
}
