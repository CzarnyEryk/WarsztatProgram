#ifndef CHECKUSERDATA_H
#define CHECKUSERDATA_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
class CheckUserData;
}

class CheckUserData : public QWidget
{
    Q_OBJECT

public:
    explicit CheckUserData(QSqlDatabase db, int id, QWidget *parent = nullptr);
    ~CheckUserData();

private slots:
    void GetUserData();

private:
    Ui::CheckUserData *ui;
    QSqlDatabase m_db;
    int m_id;
};

#endif // CHECKUSERDATA_H
