#ifndef LOGIN_H
#define LOGIN_H

#include "mainwindow.h"
#include <QDialog>
#include <QSql>
#include <QSqlDatabase>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <Qmap>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_clicked();
    void configDatabase();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
