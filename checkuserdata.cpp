//okno służące do pobrania informacji o użytkowniku
#include "checkuserdata.h"
#include "ui_checkuserdata.h"

//pobranie danych użytkownika z bazy
void CheckUserData::GetUserData()
{
    QSqlQuery zapytanie(m_db);
    QSqlQueryModel *model = new QSqlQueryModel();

    //przygotowanie i obsługa zapytania bazy danych
    zapytanie.prepare("SELECT imie, nazwisko, e_mail, nr_telefonu FROM uzytkownicy WHERE id = :id");
    //podmiana danych na zmienne
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
    //ustawienie dynamicznego rozmiaru okna tabelii
    ui->userData_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CheckUserData::CheckUserData(QSqlDatabase db, int id, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CheckUserData)
    ,m_db(db)
    ,m_id(id)
{
    ui->setupUi(this);

    //wyświetlenie danych po uruchomieniu okna
    GetUserData();
}

CheckUserData::~CheckUserData()
{
    delete ui;
}
