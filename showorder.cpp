#include "showorder.h"
#include "ui_showorder.h"

//pobranie oraz wyświetlenie danych z bazy dla zalogowanego użytkownika
void ShowOrder::ShowData()
{
    //wykonanie zapytania do bazy
    QSqlQuery zapytanie(m_db);
    QSqlQueryModel *model = new QSqlQueryModel();
    zapytanie.prepare("SELECT status, marka, model, rok_produkcji, pojemnosc_silnika, typ_silnika, vin, nr_rejestracyjny FROM zlecenia WHERE wlasciciel_zlecenia =:id");
    zapytanie.bindValue(":id", m_id);
    zapytanie.exec();
    model->setQuery(std::move(zapytanie));

    //ustawienie nagłówków tabelii
     model->setHeaderData(0, Qt::Horizontal, "Status:");
    model->setHeaderData(1, Qt::Horizontal, "Marka:");
    model->setHeaderData(2, Qt::Horizontal, "Model:");
    model->setHeaderData(3, Qt::Horizontal, "Rok Produkcji:");
     model->setHeaderData(4, Qt::Horizontal, "Poj Silnika:");
    model->setHeaderData(5, Qt::Horizontal, "Typ Silnika:");
    model->setHeaderData(6, Qt::Horizontal, "VIN:");
    model->setHeaderData(7, Qt::Horizontal, "Nr Rej:");
    model->setHeaderData(8, Qt::Horizontal, "Opis:");

    //wyświetlenie danych
    ui->order_view->setModel(model);
    ui->order_view->resizeColumnsToContents();

}


ShowOrder::ShowOrder(QSqlDatabase db, int id, QWidget *parent)
    : QWidget(parent)
    ,m_db(db)
    ,m_id(id)
    ,ui(new Ui::ShowOrder)
{
    ui->setupUi(this);
    ShowData();
}


ShowOrder::~ShowOrder()
{
    delete ui;
}


