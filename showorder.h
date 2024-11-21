#ifndef SHOWORDER_H
#define SHOWORDER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QMessageBox>

namespace Ui {
class ShowOrder;
}

class ShowOrder : public QWidget
{
    Q_OBJECT

public:
    explicit ShowOrder(QSqlDatabase db, int id, QWidget *parent = nullptr);
    ~ShowOrder();

private slots:
    void ShowData();

private:
    QSqlDatabase m_db;
    int m_id;
    Ui::ShowOrder *ui;
};

#endif // SHOWORDER_H
