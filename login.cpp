#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::configDatabase()
{
    //ścieżka do pliku z konfiguracją bazy danych
    QString filePath = QDir(QCoreApplication::applicationDirPath()).filePath("configDb.txt");

    //otwarcie pliku
    QFile fileConfigDb(filePath);

    //sprawdzenie czy plik działa
    if (!fileConfigDb.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Brak Pliku \n" + fileConfigDb.errorString());
        QCoreApplication::quit();
    }

    //kontener słownikowy do konfiguracji
    QMap<QString, QString> configData;

    QTextStream in (&fileConfigDb);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split("=");

        //sprawdzenie czy format pliku jest poprawny
        if (parts.size() == 2)
        {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();
            configData.insert(key,value);
        }else
        {
            QMessageBox::warning(this, "Ostrzeżenie", "Nieprawidłowy format pliku konfiguracyjnego \n" + line);
            QCoreApplication::quit();
        }

    }
    fileConfigDb.close();


    //obsługa bazy d
    QString login = ui->email_input->text();
    QString password = ui->password_input->text();


    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");



    //sprawdzenie przesłania danych z formularza logowania
    if(login.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Wprowadź dane logowania");
    }

    //sprawdzenie czy posiadamy adress hosta
    if(configData.contains("hostAddress"))
    {
        QString hostAddress = configData["hostAddress"];
        db.setHostName(hostAddress);
    }else
    {
        QMessageBox::critical(this, "Error", "Brak adresu hosta w pliku konfiguracyjnym");
        QCoreApplication::quit();
    }

    //sprawdzenie czy posiadamy user name
    if(configData.contains("user"))
    {
        QString dbUser = configData["user"];
        db.setUserName(dbUser);
    }else
    {
        QMessageBox::critical(this, "Warning", "Brak użytkownika bazy danych w pliku konfiguracyjnym");
        QCoreApplication::quit();
    }

    //sprawdzenie czy posiadamy user password
    if (configData.contains("password"))
    {
        QString dbPassword = configData["password"];
        db.setPassword(dbPassword);
    }
    else
    {
        QMessageBox::critical(this, "Warning", "Brak hasła do bazy danych pliku konfiguracyjnym");
        QCoreApplication::quit();
    }

    //sprawdzenie czy posiadamy nazwę bazy
    if (configData.contains("dbName"))
    {
        QString dbName = configData["dbName"];
        db.setDatabaseName(dbName);

    }else
    {
        QMessageBox::critical(this, "Warning", "Brak nazwy bazy w pliku konfiguracyjnym");
        QCoreApplication::quit();
    }

    //obsługa otwarcia bazy danych
    if(!db.open())
    {
        QMessageBox::critical(this, "Error", "Błąd połączenia z bazą" + db.lastError().text());
        QCoreApplication::quit();
    }

    //pobranie roli oraz id użytkownika
    QSqlQuery zapytanie;
    zapytanie.prepare("SELECT id, rola FROM uzytkownicy WHERE e_mail= :login AND haslo= :password");
    zapytanie.bindValue(":login", login);
    zapytanie.bindValue(":password", password);

    //sprawdzenie czy zapytanie się wykonało
    if(zapytanie.exec()){

        //sprawdzenie czy zapytanie zwróciło dane
        if(zapytanie.next()){

            //pobranie danych z bazy danych
            int id = zapytanie.value("id").toInt();
            QString rola = zapytanie.value("rola").toString();

            //sprawdzenie czy użytkownik jest klientem
            if(rola == "klient"){
                QMessageBox::information(this, "Status", "Zalogowano jako klient");
                MainWindow *mainWindow = new MainWindow(db, id, rola, nullptr);
                mainWindow->show();
                this->close();

            //sprawdzenie czy użytkownik jest mechanikiem
            }else if(rola == "mechanik"){
                QMessageBox::information(this, "Status", "Zalogowano jako mechanik");
                MainWindow *mainWindow = new MainWindow(db, id, rola, nullptr);
                mainWindow->show();
                this->close();
            }

            //brak użytkownika o podanej roli
            else
            {
                QMessageBox::warning(this, "Error", "Błąd bazy danych");
                db.removeDatabase("QMYSQL");
                QCoreApplication::quit();
            }
        }

        //brak użytkownika w bazie danych
        else
        {
            QMessageBox::warning(this, "Warning", "Błędne dane");
            db.removeDatabase("QMYSQL");
        }
    }
    //błąd wewnętrzny bazy
    else
    {
        QMessageBox::critical(this, "Error", "Błąd zapytania Mysql" + zapytanie.lastError().text());
        db.removeDatabase("QMYSQL");
        QCoreApplication::quit();
    }
}



void Login::on_pushButton_clicked()
{
    configDatabase();
}



