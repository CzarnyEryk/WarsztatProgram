#ifndef EDITORDERS_H
#define EDITORDERS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class EditOrders;
}

class EditOrders : public QDialog
{
    Q_OBJECT

public:
    explicit EditOrders(QSqlDatabase db, QWidget *parent = nullptr);
    ~EditOrders();

private slots:
    void ChangeOrderStatus();
    void ShowOrderInfo(int id);

    void on_id_comboBox_editTextChanged(const QString &arg1);

    void on_id_comboBox_currentIndexChanged(int index);

    void on_changeStatus_button_clicked();

private:
    Ui::EditOrders *ui;
    QSqlDatabase m_db;
};

#endif // EDITORDERS_H


