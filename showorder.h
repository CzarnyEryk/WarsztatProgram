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
    explicit ShowOrder(QSqlDatabase db, int id, QString rola, QWidget *parent = nullptr);
    ~ShowOrder();

private slots:
    //funkcja do wyświetlenia zleceń użytkownika Panel Klienta
    void ShowDataClient();

    //funkcja do wyświetlenia wszystkich zleceń Panel Mechanika
    void ShowDataMechanik();

private:
    QSqlDatabase m_db;
    int m_id;
    QString m_rola;
    Ui::ShowOrder *ui;
};

#endif // SHOWORDER_H
