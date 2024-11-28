//okno do wyświetlania zleceń
#include "showorder.h"
#include "ui_showorder.h"


//funckja do wyświetlenia odpowiednich danych
ShowOrder::ShowOrder(QSqlDatabase db, int id, QString rola, QWidget *parent)
    : QWidget(parent)
    ,m_db(db)
    ,m_id(id)
    ,m_rola(rola)
    ,ui(new Ui::ShowOrder)
{
    ui->setupUi(this);
    // Model bez filtrów
    model = new QSqlQueryModel(this);
    //Model z filtrami
    proxyModel = new QSortFilterProxyModel(this);


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


//pobranie oraz wyświetlenie danych z bazy dla zalogowanego użytkownika Panel Klienta
void ShowOrder::ShowDataClient()
{
    //przygotowanie połączenia z bazą
    QSqlQuery zapytanie(m_db);

    //przygotowanie zapytania
    zapytanie.prepare("SELECT status, marka, model, rok_produkcji, pojemnosc_silnika, typ_silnika, vin, nr_rejestracyjny, opis, cena FROM zlecenia WHERE wlasciciel_zlecenia =:id");

    //zamiana danych na zmienne
    zapytanie.bindValue(":id", m_id);

    if(!zapytanie.exec())
    {
        QMessageBox::critical(this, "Błąd", "Bład pobierania danych z bazy");
        return;
    }

    model->setQuery(zapytanie);

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
    model->setHeaderData(9, Qt::Horizontal, "Cena:");

    // Ustawienie modelu w proxy
    proxyModel->setSourceModel(model);

    // Ustawienie proxy modelu w widoku
    ui->order_view->setModel(proxyModel);

    //dopasowanie rozmiaru kolumn
    ui->order_view->resizeColumnsToContents();
    int lastColumnIndex = ui->order_view->model()->columnCount() - 1; // Ostatnia kolumna
    ui->order_view->horizontalHeader()->setSectionResizeMode(lastColumnIndex, QHeaderView::Stretch);

    zapytanie.prepare("SELECT SUM(cena) FROM zlecenia where wlasciciel_zlecenia =:id");
    zapytanie.bindValue(":id", m_id);

    if(!zapytanie.exec())
    {
        //wyświetl informację o błędzie
        QMessageBox::critical(this, "Błąd", "Błąd pobierania danych z bazy: \n" + zapytanie.lastError().text());
        return;
    }


    if (zapytanie.next())
    {
        float wartosc = zapytanie.value(0).toFloat();
        ui->ordersValue_label->setText(QString("Całkowita wartość zleceń: %1 zł").arg(wartosc));
    }

}

ShowOrder::~ShowOrder()
{
    delete ui;
}


//pobranie wszystkich dostępnych zleceń w bazie danych Panel Mechanika
void ShowOrder::ShowDataMechanik()
{
    QSqlQuery zapytanie(m_db);

    //przygotowanie zapytania
    zapytanie.prepare("SELECT  status,marka, model, rok_produkcji, pojemnosc_silnika, typ_silnika, vin, nr_rejestracyjny, opis, cena FROM zlecenia");

    //obsługa błędu zapytania
    if(!zapytanie.exec())
    {
        //wyświetl informację o błędzie
        QMessageBox::critical(this, "Błąd", "Błąd pobierania danych z bazy: \n" + zapytanie.lastError().text());
        return;
    }

    model->setQuery(zapytanie);
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
    model->setHeaderData(9, Qt::Horizontal, "Wartość:");


    // Ustawienie modelu w proxy
    proxyModel->setSourceModel(model);

    // Ustawienie proxy modelu w widoku
    ui->order_view->setModel(proxyModel);

    //dopasowanie rozmiarów kolumn
    ui->order_view->resizeColumnsToContents();
    int lastColumnIndex = ui->order_view->model()->columnCount() - 2; // Ostatnia kolumna
    ui->order_view->horizontalHeader()->setSectionResizeMode(lastColumnIndex, QHeaderView::Stretch);

    zapytanie.prepare("SELECT SUM(cena) FROM zlecenia");

    if(!zapytanie.exec())
    {
        //wyświetl informację o błędzie
        QMessageBox::critical(this, "Błąd", "Błąd pobierania danych z bazy: \n" + zapytanie.lastError().text());
        return;
    }


    if (zapytanie.next())
    {
        float wartosc = zapytanie.value(0).toFloat();
        ui->ordersValue_label->setText(QString("Całkowita wartość zleceń: %1 zł").arg(wartosc));
    }
}




void ShowOrder::on_useFilter_button_clicked()
{
    // Pobranie danych do filtra rejestracji
    QString rejFiltr = ui->rejFiltrInput->text();

    // Pobranie danych do filtra statusu
    QString statFiltr = ui->statusFiltrInput->currentText();


    // Sprawdzenie czy istnieje model
    if (proxyModel)
    {
        //obsługa 2 aktywnych funkcji
        if (!statFiltr.isEmpty() && !rejFiltr.isEmpty())
        {
            QMessageBox::information(this, "Informacja", "Można użyć tylko jednego filtra");
            ui->filterLabelStatus->setText("Błąd: Oba filtry aktywne");
            return;
        }

        // Jeśli filtr dla statusu aktywny
        if(!statFiltr.isEmpty())
        {
            proxyModel->setFilterKeyColumn(0);
            proxyModel->setFilterFixedString(statFiltr);
            ui->filterLabelStatus->setText("Filtr statusu aktywny");
        }

        //jeżeli filtr rejestracji wyłączony
        else if (!rejFiltr.isEmpty())
        {
            proxyModel->setFilterKeyColumn(7);
            proxyModel->setFilterFixedString(rejFiltr);
            ui->filterLabelStatus->setText("Filtr nr rejestracyjnego aktywny");
        }

        //jeżeli filtr statusu wyłączony
        else
        {
            proxyModel->setFilterRegularExpression(QRegularExpression());
            ui->filterLabelStatus->setText("Wyłączony");
        }
    }

    //odświeżenie widoku
    ui->order_view->update();
}


