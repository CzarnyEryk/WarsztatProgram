#ifndef DELETEACCOUNT_H
#define DELETEACCOUNT_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class DeleteAccount;
}

class DeleteAccount : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteAccount(QSqlDatabase db, int id, QWidget *parent = nullptr);
    ~DeleteAccount();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::DeleteAccount *ui;
    QSqlDatabase m_db;
    int m_id;
};

#endif // DELETEACCOUNT_H
