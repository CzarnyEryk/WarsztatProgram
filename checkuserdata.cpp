#include "checkuserdata.h"
#include "ui_checkuserdata.h"

void CheckUserData::GetUserData()
{
    //wykonanie zapytania do bazy
    QSqlQuery zapytanie(m_db);
    QSqlQueryModel *model = new QSqlQueryModel();
    zapytanie.prepare("SELECT imie, nazwisko, e_mail, nr_telefonu FROM uzytkownicy WHERE id = :id");
    zapytanie.bindValue(":id", m_id);
    zapytanie.exec();
    model->setQuery(std::move(zapytanie));

    //ustawienie nagłówków tabelii
    model->setHeaderData(0, Qt::Horizontal, "Imię:");
    model->setHeaderData(1, Qt::Horizontal, "Nazwisko:");
    model->setHeaderData(2, Qt::Horizontal, "E-mail:");
    model->setHeaderData(3, Qt::Horizontal, "Nr Tel:");
    //wyświetlenie danych
    ui->userData_view->setModel(model);
    ui->userData_view->resizeColumnsToContents();
}

CheckUserData::CheckUserData(QSqlDatabase db, int id, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CheckUserData)
    ,m_db(db)
    ,m_id(id)
{
    ui->setupUi(this);
    GetUserData();
}

CheckUserData::~CheckUserData()
{
    delete ui;
}
