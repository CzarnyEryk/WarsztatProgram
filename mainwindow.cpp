#include "mainwindow.h"
#include "qsqlquery.h"
#include "ui_mainwindow.h"
#include "createorder.h"
#include "showorder.h"
#include "checkuserdata.h"
#include "deleteaccount.h"

MainWindow::MainWindow(QSqlDatabase db, int id, QString rola,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,m_db(db)
    ,m_id(id)
    ,m_rola(rola)
{
    ui->setupUi(this);

    if (m_rola == "mechanik")
    {
        ui->createOrder->setText("Zmień Status");
        ui->deleteAcount->setVisible(false);
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

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
    QSqlQuery zapytanie;
   zapytanie.prepare("INSERT INTO zlecenia (marka, model, rok_produkcji, pojemnosc_silnika, typ_silnika, vin, nr_rejestracyjny, opis, wlasciciel_zlecenia, status) "
                     "VALUES (:marka, :model, :rok, :poj, :typ, :vin, :rej, :opis, :m_id, :status_zlecenia) ");


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

    if(!zapytanie.exec())
    {
        QMessageBox::warning(this, "Ostrzeżenie", "Błąd dodawania danych do bazy" + zapytanie.lastError().text());
        qDebug() <<  zapytanie.lastError().text();
    }
    else
    {
        QMessageBox::information(this, "Informacja", "Dane zostały dodane do bazy");
    }
}

void MainWindow::on_createOrder_clicked()
{
    if (m_rola == "mechanik")
    {

    }
    else
    {
        openCreateDialog();
    }
}


void MainWindow::on_checkOrder_clicked()
{
    if (m_rola == "mechanik")
    {

    }

    else
    {
        ShowOrder *showOrder = new ShowOrder(m_db, m_id, nullptr);
        showOrder->show();
    }
}


void MainWindow::on_checkData_clicked()
{
    if (m_rola == "mechanik")
    {

    }

    else
    {
        CheckUserData *userDataWindow = new CheckUserData(m_db, m_id, nullptr);
        userDataWindow->show();
    }
}


void MainWindow::on_deleteAcount_clicked()
{
    if (m_rola == "mechanik")
    {

    }

    else
    {
        DeleteAccount *deleteAccount = new DeleteAccount(m_db, m_id, nullptr);
        deleteAccount->show();
    }
}

