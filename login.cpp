//okno służące do logowania się do aplikacji
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

//konfiguracja danych bazy
void Login::configDatabase()
{
    //ścieżka do pliku z konfiguracją bazy danych
    QString filePath = QDir(QCoreApplication::applicationDirPath()).filePath("configDb.txt");

    //otwarcie pliku
    QFile fileConfigDb(filePath);

    //sprawdzenie czy plik działa
    if (!fileConfigDb.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Błąd", "Brak Pliku \n" + fileConfigDb.errorString());
        QCoreApplication::quit();
    }


    //kontener słownikowy do konfiguracji
    QMap<QString, QString> configData;

    QTextStream in (&fileConfigDb);
    while (!in.atEnd())
    {
        //pobranie pary danych
        QString line = in.readLine();
        QStringList parts = line.split("=");

        //sprawdzenie czy format pliku jest poprawny
        if (parts.size() == 2)
        {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();
            configData.insert(key,value);
        }
        //obsłużenie błędu konfiguracji pliku
        else
        {
            QMessageBox::warning(this, "Błąd", "Nieprawidłowy format pliku konfiguracyjnego \n" + line);
            QCoreApplication::quit();
        }

    }

    //zamknięcie pliku
    fileConfigDb.close();


    //pobranie loginu oraz hasła do bazy danych
    QString login = ui->email_input->text();
    QString password = ui->password_input->text();

    //dodanie drivera do obsługi mysql
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");



    //sprawdzenie przesłania danych z formularza logowania
    if(login.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Ostrzeżenie", "Wprowadź dane logowania");
    }

    //sprawdzenie czy posiadamy adress hosta
    if(configData.contains("hostAddress"))
    {
        //przypisanie adresu hosta do bazy danych
        QString hostAddress = configData["hostAddress"];
        db.setHostName(hostAddress);
    }
    //obsługa błędu braku hosta
    else
    {
        QMessageBox::critical(this, "Błąd", "Brak adresu hosta w pliku konfiguracyjnym");
        QCoreApplication::quit();
    }

    //sprawdzenie czy posiadamy user name
    if(configData.contains("user"))
    {
        //przypisanie użytkownika do konfiguracji bazy danych
        QString dbUser = configData["user"];
        db.setUserName(dbUser);
    }
    //obsługa błędu braku nazwy użytkownika
    else
    {
        QMessageBox::critical(this, "Błąd", "Brak użytkownika bazy danych w pliku konfiguracyjnym");
        QCoreApplication::quit();
    }

    //sprawdzenie czy posiadamy user password
    if (configData.contains("password"))
    {
        //przypisanie hasła do bazy danych
        QString dbPassword = configData["password"];
        db.setPassword(dbPassword);
    }
    //obsługa błędu braku hasła
    else
    {
        QMessageBox::critical(this, "Błąd", "Brak hasła do bazy danych pliku konfiguracyjnym");
        QCoreApplication::quit();
    }

    //sprawdzenie czy posiadamy nazwę bazy
    if (configData.contains("dbName"))
    {
        //przypisanie nazwy bazy do bazy
        QString dbName = configData["dbName"];
        db.setDatabaseName(dbName);

    }
    //obsługa błędu braku nazwy bazy danych
    else
    {
        QMessageBox::critical(this, "Błąd", "Brak nazwy bazy w pliku konfiguracyjnym");
        QCoreApplication::quit();
    }

    //połączenie z bazą danych i autoryzacja

    //obsługa błędu otwarcia bazy danych
    if(!db.open())
    {
        QMessageBox::critical(this, "Error", "Błąd połączenia z bazą \n" + db.lastError().text());
        QCoreApplication::quit();
    }


    //pobranie roli oraz id użytkownika
    QSqlQuery zapytanie;
    zapytanie.prepare("SELECT id, rola FROM uzytkownicy WHERE e_mail= :login AND haslo= :password");

    //podmiana danych na przekazane zmienne
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
                QMessageBox::information(this, "Informacja", "Zalogowano jako klient");

                //otwarice okna z przekazaniem danych z bazy
                MainWindow *mainWindow = new MainWindow(db, id, rola, nullptr);
                mainWindow->show();
                this->close();

            //sprawdzenie czy użytkownik jest mechanikiem
            }else if(rola == "mechanik"){
                QMessageBox::information(this, "Informacja", "Zalogowano jako mechanik");

                //otwarcie okna z przekazaniem danych z bazy
                MainWindow *mainWindow = new MainWindow(db, id, rola, nullptr);
                mainWindow->show();
                this->close();
            }

            //brak użytkownika o podanej roli
            else
            {
                QMessageBox::warning(this, "Błąd", "Błąd bazy danych \n Brak użytkownika w bazie o podanej roli");
                db.removeDatabase("QMYSQL");
                QCoreApplication::quit();
            }
        }

        //błędne dane podane podczas logowania lub brak takiego użytkownika
        else
        {
            QMessageBox::warning(this, "Warning", "Błędne dane");
            db.removeDatabase("QMYSQL");
        }
    }
    //błąd wewnętrzny bazy
    else
    {
        QMessageBox::critical(this, "Błąd", "Błąd zapytania Mysql \n" + zapytanie.lastError().text());
        db.removeDatabase("QMYSQL");
        QCoreApplication::quit();
    }
}


//obsługa kliknięcia przycisku logowania
void Login::on_pushButton_clicked()
{
    configDatabase();
}



