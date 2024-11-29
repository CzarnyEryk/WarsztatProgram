//główne okno aplikacji
#include "mainwindow.h"
#include "qsqlquery.h"
#include "ui_mainwindow.h"
#include "createorder.h"
#include "showorder.h"
#include "checkuserdata.h"
#include "deleteaccount.h"
#include "editorders.h"

MainWindow::MainWindow(QSqlDatabase db, int id, QString rola,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,m_db(db)
    ,m_id(id)
    ,m_rola(rola)
{
    ui->setupUi(this);

    //zmiana wyglądu okna jeżeli zalogował się mechanik
    if (m_rola == "mechanik")
    {
        //zmiana przycisku Utwórz zlecenie na zmień status
        ui->createOrder->setText("Zmień Status");
        //ukrycie okna do usuwania konta
        ui->deleteAcount->setVisible(false);
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

//utwórzenie funkcji do przejęcia danych z utworzenia zlecenia
void MainWindow::openCreateDialog()
{
    CreateOrder *createOrder = new CreateOrder(nullptr);
    connect(createOrder, &CreateOrder::dataSubmit, this, &MainWindow::getDataDialogUpploadDb);
    createOrder->exec();
}

//pobranie danych z dialogu utwórz zlecenie i wrzucenie ich do bazy
void MainWindow::getDataDialogUpploadDb(const QString &marka, const QString &model, const QString &rok, const QString &poj, const QString &typ, const QString &vin,
                   const QString &rej, const QString &opis)
{
    //przygotowanie zapytania
    QSqlQuery zapytanie;
   zapytanie.prepare("INSERT INTO zlecenia (marka, model, rok_produkcji, pojemnosc_silnika, typ_silnika, vin, nr_rejestracyjny, opis, wlasciciel_zlecenia, status) "
                     "VALUES (:marka, :model, :rok, :poj, :typ, :vin, :rej, :opis, :m_id, :status_zlecenia) ");

    //zamiana danych na zmienne
    zapytanie.bindValue(":marka", marka);
    zapytanie.bindValue(":model", model);
    zapytanie.bindValue(":rok", rok);
    zapytanie.bindValue(":poj", poj);
    zapytanie.bindValue(":typ", typ);
    zapytanie.bindValue(":vin", vin);
    zapytanie.bindValue(":rej", rej);
    zapytanie.bindValue(":opis", opis);
    zapytanie.bindValue(":m_id", m_id);
    zapytanie.bindValue(":status_zlecenia", "złożone");

    //obsługa błedu zapytania
    if(!zapytanie.exec())
    {
        //informacja zwrotna na temat błędu
        QMessageBox::warning(this, "Ostrzeżenie", "Błąd dodawania danych do bazy \n" + zapytanie.lastError().text());
        return;
    }

    //informacja o poprawnym dodaniu danych do bazy
    QMessageBox::information(this, "Informacja", "Dane zostały dodane do bazy");
}


//funkja obsługująca przycisk utwórz zlecenie dla klienta oraz zmień status zlecenia dla mechanika
void MainWindow::on_createOrder_clicked()
{
    //zmień status Panel Mechanika
    if (m_rola == "mechanik")
    {
        //wyświetlenie okna do edycji zlecenia
        EditOrders *editOrder = new EditOrders(m_db, nullptr);
        editOrder->show();
    }
    //utwórz zlecenie Panel Klienta
    else
    {
        //wyświetlenie formularza tworzenia zlecenia
        openCreateDialog();
    }
}


//przycisk do sprawdzenia zleceń
void MainWindow::on_checkOrder_clicked()
{
    //sprawdzenie wszystkich dostępnych zleceń w bazie danych
    if (m_rola == "mechanik")
    {
        //wyświetlenie wszystkich zleceń z bazy danych
        ShowOrder *showOrderMechanik = new ShowOrder(m_db, 0, m_rola, nullptr);
        showOrderMechanik->show();
    }

    //Sprawdzenie zleceń zalogowanego użytkownika
    else if(m_rola == "klient")
    {
        //wywołanie okna do wyświetlenia zleceń użytkownika
        ShowOrder *showOrderClient = new ShowOrder(m_db, m_id, m_rola, nullptr);
        showOrderClient->show();
    }
    //brak użytkownika o podanej roli
    else
    {
        QMessageBox::critical(this, "Błąd", "Brak użytkownika o podanej roli \n" + m_rola);
        //zamknięcie aplikacji
        QCoreApplication::exit(-1);
    }
}


//obsługa kliknięcia przycisku
void MainWindow::on_checkData_clicked()
{
    //sprawdzenie danych mechanika
    if (m_rola == "mechanik")
    {
        //wyświetlenie okna z sprawdzeniem danych mechanika
        CheckUserData *mechanikDataWindow = new CheckUserData(m_db, m_id, nullptr);
        mechanikDataWindow->show();
    }

    //sprawdzenie danych klienta
    else
    {
        //wyświetlenie okna do sprawdzenia danych użytkownika
        CheckUserData *userDataWindow = new CheckUserData(m_db, m_id, nullptr);
        userDataWindow->show();
    }
}

//usunięcie konta
void MainWindow::on_deleteAcount_clicked()
{
    //usunięcie konta
    if (m_rola == "klient")
    {
        //wyświetlenie okna do usuwania konta użytkownika
        DeleteAccount *deleteAccount = new DeleteAccount(m_db, m_id, nullptr);
        deleteAccount->show();
    }

}

//obsługa przucisku do wylogowania się
void MainWindow::on_logout_clicked()
{
    //informacja o wylogowaniu
    QMessageBox::information(this, "Informacja", "Pomyślnie wylogowano");
    QCoreApplication::quit();
}

