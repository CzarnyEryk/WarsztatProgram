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
                zapytanie.prepare("DELETE from uzytkownicy WHERE id= :id");
                zapytanie.bindValue(":id", m_id);

                if(zapytanie.exec())
                {
                     QMessageBox::information(this, "Info", "Konto usunięte");
                    QCoreApplication::quit();
                }
                else
                {
                    QMessageBox::critical(this, "Error", "Błąd aplikacji");
                    qDebug() << "Bład przy usuwaniu konta wykonanie zapytania delete";
                    qDebug() << zapytanie.lastError().text();
                }

            }else
            {
                QMessageBox::information(this, "Info", "Błędne hasło");
            }
        }
    }else

    {
        QMessageBox::critical(this, "Error", "Błąd Aplikacji");
        QCoreApplication::quit();
    }


}


//anulowanie usunięcia konta
void DeleteAccount::on_buttonBox_rejected()
{
    QMessageBox::information(this, "test", "NIE");
}

