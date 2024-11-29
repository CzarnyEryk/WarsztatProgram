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

    //brak użytkownika o podanej roli błąd danych w bazie
    else
    {
        QMessageBox::critical(this, "Błąd", "Błędna rola użytkownika");
        QCoreApplication::exit(-1);
        return;
    }
}

ShowOrder::~ShowOrder()
{
    delete ui;
}


//PANEL KLIENTA ----------------------------------------------------------------------------------------------------------------------------------------------
//pobranie oraz wyświetlenie danych z bazy dla zalogowanego użytkownika Panel Klienta
void ShowOrder::ShowDataClient()
{
    //przygotowanie połączenia z bazą
    QSqlQuery zapytanie(m_db);

    //przygotowanie zapytania pobierającego wszystkie informacje na temat zlecenia klienta
    zapytanie.prepare("SELECT status, marka, model, rok_produkcji, pojemnosc_silnika, typ_silnika, vin, nr_rejestracyjny, opis, cena FROM zlecenia WHERE wlasciciel_zlecenia =:id");

    //zamiana danych na zmienne
    zapytanie.bindValue(":id", m_id);

    //obsługa błędu zapytania
    if(!zapytanie.exec())
    {
        QMessageBox::critical(this, "Błąd", "Bład pobierania zleceń z danych z bazy");
        return;
    }

    //przekazanie danych do modelu
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
    //dopasowanie rozmiaru ostatniej kolumny
    int lastColumnIndex = ui->order_view->model()->columnCount() - 1; // Ostatnia kolumna
    //dynamiczne dopasowanie kolumn do zawartości okna
    ui->order_view->horizontalHeader()->setSectionResizeMode(lastColumnIndex, QHeaderView::Stretch);


    //pobranie sumy wszytkich zleceń klienta
    zapytanie.prepare("SELECT SUM(cena) FROM zlecenia where wlasciciel_zlecenia =:id");
    zapytanie.bindValue(":id", m_id);

    //obsługa błędu zapytania
    if(!zapytanie.exec())
    {
        //wyświetl informację o błędzie
        QMessageBox::critical(this, "Błąd", "Błąd pobierania danych z bazy: \n" + zapytanie.lastError().text());
        return;
    }

    //wyświetlenie informacji na temat sumy zamówień
    if (zapytanie.next())
    {
        float wartosc = zapytanie.value(0).toFloat();
        ui->ordersValue_label->setText(QString("Całkowita wartość zleceń: %1 zł").arg(wartosc));
    }

}


//PANEL MECHANIKA ------------------------------------------------------------------------------------------------------------------------------------------------
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

    //przekazanie danych do modelu
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
    //dopasowanie rozmiaru kolumny
    int lastColumnIndex = ui->order_view->model()->columnCount() - 2; // Ostatnia kolumna
    //automatyczne dopasowanie szerokości kolumn do okna
    ui->order_view->horizontalHeader()->setSectionResizeMode(lastColumnIndex, QHeaderView::Stretch);

    //Pobranie sumy wartości zamówień
    zapytanie.prepare("SELECT SUM(cena) FROM zlecenia");

    //obsługa błedu pobrania sumy
    if(!zapytanie.exec())
    {
        //wyświetl informację o błędzie
        QMessageBox::critical(this, "Błąd", "Błąd pobierania sumy zleceń z bazy: \n" + zapytanie.lastError().text());
        return;
    }

    //wyświetlenie całkowitej wartości zleceń
    if (zapytanie.next())
    {
        float wartosc = zapytanie.value(0).toFloat();
        ui->ordersValue_label->setText(QString("Całkowita wartość zleceń: %1 zł").arg(wartosc));
    }

    //pobranie średniej wartości zamówienia
    zapytanie.prepare("SELECT AVG(cena) FROM zlecenia");

    //obsługa błedu pobrania średniej wartości
    if (!zapytanie.exec())
    {
        //wyświetl informację o błędzie
        QMessageBox::critical(this, "Błąd", "Błąd pobierania średniej wartości z bazy: \n" + zapytanie.lastError().text());
        return;
    }

    //wyświetlenie średniej wartości zleceń
    if (zapytanie.next())
    {
        float srednia_wartosc = zapytanie.value(0).toFloat();
        ui->ordersValueAvg_label->setText(QString("Średnia wartość zamówienia: %1 zł").arg(srednia_wartosc));

    }

    //pobranie minimalnej wartości zamówień
    zapytanie.prepare("SELECT MIN(cena) FROM zlecenia");

    //  //obsługa błedu pobrania wartości min
    if (!zapytanie.exec())
    {
        //wyświetl informację o błędzie
        QMessageBox::critical(this, "Błąd", "Błąd pobierania wartości min z bazy: \n" + zapytanie.lastError().text());
        return;
    }

    //wyświetlenie wartości minimalnej zlecenia
    if (zapytanie.next())
    {
        float minimalna_wartosc = zapytanie.value(0).toFloat();
        ui->ordersValueMin_label->setText(QString("Minimalna wartość zamówienia: %1 zł").arg(minimalna_wartosc));

    }

    //pobranie maksymalnej wartości zamówienia
    zapytanie.prepare("SELECT MAX(cena) FROM zlecenia");

      //obsługa błedu pobrania wartości maksymalnej
    if (!zapytanie.exec())
    {
        //wyświetl informację o błędzie
        QMessageBox::critical(this, "Błąd", "Błąd pobierania danych z bazy: \n" + zapytanie.lastError().text());
        return;
    }

    //wyświetlenie informacji o maksymalnej wartości zamówienia
    if (zapytanie.next())
    {
        float max_wartosc = zapytanie.value(0).toFloat();
        ui->ordersValueMax_label->setText(QString("Maksymalna wartość zamówienia: %1 zł").arg(max_wartosc));

    }
}


//ustawienie filtrowania
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


