#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "messenger.h"
#include "users.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    //Messenger *messenger;

private slots:
    void on_pushButton_clicked();
    void on_registerButton_clicked();

signals:
    void sendNickname(QString nickname);
};
#endif // MAINWINDOW_H
