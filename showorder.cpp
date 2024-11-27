//okno do wyświetlania zleceń
#include "showorder.h"
#include "ui_showorder.h"

//pobranie oraz wyświetlenie danych z bazy dla zalogowanego użytkownika Panel Klienta
void ShowOrder::ShowDataClient()
{
    //przygotowanie połączenia z bazą
    QSqlQuery zapytanie(m_db);
    QSqlQueryModel *model = new QSqlQueryModel();

    //przygotowanie zapytania
    zapytanie.prepare("SELECT status, marka, model, rok_produkcji, pojemnosc_silnika, typ_silnika, vin, nr_rejestracyjny, opis FROM zlecenia WHERE wlasciciel_zlecenia =:id");

    //zamiana danych na zmienne
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
    //dynamiczna zmiana rozmiaru kolumn
    ui->order_view->resizeColumnsToContents();
    int lastColumnIndex = ui->order_view->model()->columnCount() - 1; // Ostatnia kolumna
    ui->order_view->horizontalHeader()->setSectionResizeMode(lastColumnIndex, QHeaderView::Stretch);

}

//pobranie wszystkich dostępnych zleceń w bazie danych Panel Mechanika
void ShowOrder::ShowDataMechanik()
{
    QSqlQuery zapytanie(m_db);
    QSqlQueryModel *model = new QSqlQueryModel();
    //przygotowanie zapytania
    zapytanie.prepare("SELECT  status, marka, model, rok_produkcji, pojemnosc_silnika, typ_silnika, vin, nr_rejestracyjny, opis FROM zlecenia");

    //obsługa błędu zapytania
    if(!zapytanie.exec())
    {
        //wyświetl informację o błędzie
        QMessageBox::critical(this, "Błąd", "Błąd pobierania danych z bazy: \n" + zapytanie.lastError().text());
        QCoreApplication::exit(-1);
        return;
    }

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
    //dynamiczna zmiana rozmiaru kolumn
    ui->order_view->resizeColumnsToContents();
    int lastColumnIndex = ui->order_view->model()->columnCount() - 1; // Ostatnia kolumna
    ui->order_view->horizontalHeader()->setSectionResizeMode(lastColumnIndex, QHeaderView::Stretch);

}


//funckja do wyświetlenia odpowiednich danych
ShowOrder::ShowOrder(QSqlDatabase db, int id, QString rola, QWidget *parent)
    : QWidget(parent)
    ,m_db(db)
    ,m_id(id)
    ,m_rola(rola)
    ,ui(new Ui::ShowOrder)
{
    ui->setupUi(this);

    //wyświetlenie wszystkich zleceń mechanika
    if (m_rola == "mechanik")
    {
        ui->label->setText("Zlecenia: ");
        ShowDataMechanik();
    }
    //wyświetlenie wszystkich zleceń zalogowaniego użytkownika
    else if (m_rola == "klient")
    {
        ShowDataClient();
    }
    else
    {
        QMessageBox::critical(this, "Błąd", "Błędna rola użytkownika");
        QCoreApplication::exit();
        return;
    }
}


ShowOrder::~ShowOrder()
{
    delete ui;
}


