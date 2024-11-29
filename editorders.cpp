//okno służące do edycji zlecenia
#include "editorders.h"
#include "ui_editorders.h"


EditOrders::EditOrders(QSqlDatabase db, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditOrders)
    ,m_db(db)
{
    ui->setupUi(this);
    ChangeOrderStatus();

    //ustawienie maski do wprowadzania ceny
    ui->priceInput->setInputMask("99999.99");
}

EditOrders::~EditOrders()
{
    delete ui;
}

//pobranie danych o aktualnym zleceniu
void EditOrders::ShowOrderInfo(int id)
{
    //przygotowanie zapytania aby pobrać informacje o zleceniu
    QSqlQuery zapytanie(m_db);
    QSqlQueryModel *model = new QSqlQueryModel();
    zapytanie.prepare("SELECT id, status, cena, marka, model, pojemnosc_silnika, typ_silnika, nr_rejestracyjny, opis FROM zlecenia WHERE id =:id");

    //podmiana danych na zmienne
    zapytanie.bindValue(":id", id);

    //obsługa błędu zapytania
    if (!zapytanie.exec())
    {
        QMessageBox::critical(this, "Błąd", "Błąd pobrania informacji o zleceniu \n" + zapytanie.lastError().text());
    }

    //przekazanie własności obiektu model do zaptania
    model->setQuery(zapytanie);

    //ustawienie nagłówków tabelii
    model->setHeaderData(0, Qt::Horizontal, "ID:");
    model->setHeaderData(1, Qt::Horizontal, "Status:");
    model->setHeaderData(2, Qt::Horizontal, "Cena:");
    model->setHeaderData(3, Qt::Horizontal, "Marka:");
    model->setHeaderData(4, Qt::Horizontal, "Model:");
    model->setHeaderData(5, Qt::Horizontal, "Poj Silnika:");
    model->setHeaderData(6, Qt::Horizontal, "Typ Silnika:");
    model->setHeaderData(7, Qt::Horizontal, "Nr Rej:");
    model->setHeaderData(8, Qt::Horizontal, "Opis:");

    //wyświetlenie danych
    ui->orderInfoTableView->setModel(model);

    //ustawienie ruchomej kolumny -1 ostatnia kolumna
    int lastColumnIndex = ui->orderInfoTableView->model()->columnCount() - 2;
    //dynamiczne dostosowanie rozmiaru do rozmiaru okna
    ui->orderInfoTableView->horizontalHeader()->setSectionResizeMode(lastColumnIndex, QHeaderView::Stretch);
}

//pobranie informacji o id które jest w dostępnych zleceniach
void EditOrders::ChangeOrderStatus()
{
    QSqlQuery zapytanie(m_db);

    //obsługa błedu zapytania zmiany statusu
    if (!zapytanie.exec("SELECT id FROM zlecenia"))
    {
        //wyświetlenie informacji na temat błędu
        QMessageBox::critical(this, "Błąd", "Błąd pobrana zleceń");
        QCoreApplication::exit(-1);
        return;
    }

    //pobranie wartości id i dodanie do menu
    while (zapytanie.next())
    {
        //pobranie id z zapytania z bazy danych
        int id = zapytanie.value(0).toInt();
        //dodanie id do menu
        ui->id_comboBox->addItem(QString::number(id), QVariant(id));

    }

    //wyświetlenie danych o aktualnie wybranym zamówieniu
    int actual_id = ui->id_comboBox->currentText().toInt();
    ShowOrderInfo(actual_id);

}




//obsługa zmiany wybrania id w menu
void EditOrders::on_id_comboBox_currentIndexChanged(int index)
{
    //sprawdzenie czy element menu się róźni
    if (index != -1)
    {
        int actual_id = ui->id_comboBox->currentData().toInt();
        //odświeżenie widoku zleceń
        ShowOrderInfo(actual_id);
    }
}

//funkcja do edycji statusu zamówienia
void EditOrders::on_changeStatus_button_clicked()
{
    //pobranie id użytkownika
    int actual_id = ui->id_comboBox->currentData().toInt();

    //zmiana statusu zamóweinia
    //sprawdzenie czy znacznik zmiany statusu i ceny jest zaznaczony
    if (ui->statusChange->isChecked() and ui->priceChange->isChecked() )
    {
        //pobranie statusu zamówienia z pola wyboru
        QString status = ui->status_comboBox->currentText();

        //pobreanie ceny z pola UI
        double cena = ui->priceInput->text().toDouble();

        //wykonanie zamówienia zmieniającego status oraz cenę
        QSqlQuery zapytanie(m_db);
        zapytanie.prepare("UPDATE zlecenia SET status =:status, cena =:cena WHERE id =:id");
        zapytanie.bindValue(":status", status);
        zapytanie.bindValue(":cena", cena);
        zapytanie.bindValue(":id", actual_id);

        //obsługa błędu zapytania
        if (!zapytanie.exec())
        {
            QMessageBox::information(this, "Błąd", "Bład zmiany statusu oraz ceny zlecenia" + zapytanie.lastError().text());
            return;
        }

        //informacja o wykonaniu zmiany
        QMessageBox::information(this, "Informacja", "Poprawnie zmieniono status zlecenia oraz cene");
        return;
    }

    //zmiana statusu
    if (ui->statusChange->isChecked())
    {
        //pobranie danych na temat statusu z pola wyboru
        QString status = ui->status_comboBox->currentText();

        //wykonanie zapytania do bazy w celu zmiany statusu
        QSqlQuery zapytanie(m_db);
        zapytanie.prepare("UPDATE zlecenia SET status =:status WHERE id =:id");
        zapytanie.bindValue(":status", status);
        zapytanie.bindValue(":id", actual_id);

        //obsługa błędu zapytania
        if (!zapytanie.exec())
        {
            QMessageBox::information(this, "Błąd", "Bład zmiany statusu");
            return;
        }

        //informacje o wykonaniu zmiany
        QMessageBox::information(this, "Informacja", "Poprawnie zmieniono status zlecenia");
        return;
    }

    //zmiana ceny zlecenia
    if (ui->priceChange->isChecked())
    {
        //pobranie ceny z formularza
        double cena = ui->priceInput->text().toDouble();

        //przygotowanie zapytania w celu zmiany ceny zlecenia
        QSqlQuery zapytanie(m_db);
        zapytanie.prepare("UPDATE zlecenia SET cena =:cena WHERE id =:id");
        zapytanie.bindValue(":cena", cena);
        zapytanie.bindValue(":id", actual_id);

        //obsługa błędu zmiany statusu
        if (!zapytanie.exec())
        {
            QMessageBox::information(this, "Błąd", "Bład zmiany statusu");
            return;
        }

        //informacja o poprawnym wykonaniu zmiany ceny
        QMessageBox::information(this, "Informacja", "Poprawnie zmieniono cene zlecenia");
    }
}

