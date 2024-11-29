#ifndef SHOWORDER_H
#define SHOWORDER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QRegularExpression>

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

    //obsługa filtra
    void on_useFilter_button_clicked();

private:
    QSqlDatabase m_db;
    int m_id;
    QString m_rola;
    Ui::ShowOrder *ui;
    QSqlQueryModel *model;
    QSortFilterProxyModel *proxyModel;
};

#endif // SHOWORDER_H
