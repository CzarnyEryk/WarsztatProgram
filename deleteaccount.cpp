//okno służące do usunięcia konta
#include "deleteaccount.h"
#include "ui_deleteaccount.h"

DeleteAccount::DeleteAccount(QSqlDatabase db, int id, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteAccount)
    ,m_db(db)
    ,m_id(id)
{
    ui->setupUi(this);
}

DeleteAccount::~DeleteAccount()
{
    delete ui;
}

//usunięcie konta użytkownika
void DeleteAccount::on_buttonBox_accepted()
{
    //pobranie hasła z pola Hasło
    QString userInputPassword = ui->password_input->text();
    QSqlQuery zapytanie;
    zapytanie.prepare("SELECT haslo FROM uzytkownicy WHERE id= :id");
    zapytanie.bindValue(":id", m_id);

    //sprawdzenie czy zapytanie się wykonało
    if(zapytanie.exec())
    {
        //sprawdzenie czy zapytanie zwróciło dane
        if(zapytanie.next())
        {
            //pobranie danych z bazy danych
            QString passwordUser = zapytanie.value("haslo").toString();

            //sprawdzenie zgodności haseł
            if(userInputPassword == passwordUser)
            {
                //stworzenie zapytania które usunie konto
                zapytanie.prepare("DELETE from uzytkownicy WHERE id= :id");
                zapytanie.bindValue(":id", m_id);

                //pozytywne usunięcie konta
                if(zapytanie.exec())
                {
                    //informacja zwrotna na temat konta oraz zamknięcie aplikacji
                    QMessageBox::information(this, "Info", "Konto usunięte");
                    QCoreApplication::quit();
                }
                //obsługa błędu zapytania w ramach usunięcia konta
                else
                {
                    //informacja o błędzie i zamknięcie aplikacji
                    QMessageBox::critical(this, "Błąd", "Błąd aplikacji \n" + zapytanie.lastError().text());
                }

            }
            //obsługa złego hasła
            else
            {
                QMessageBox::information(this, "Informacja", "Błędne hasło");
            }
        }
    }
    //obsługa błędu
    else

    {
        //zamknięcie aplikacji z powodu błedu
        QMessageBox::critical(this, "Błąd", "Błąd Aplikacji");
        QCoreApplication::quit();
    }


}


//anulowanie usunięcia konta
void DeleteAccount::on_buttonBox_rejected()
{
    QMessageBox::information(this, "Informacja", "Anulowano usunięcie konta");
}

