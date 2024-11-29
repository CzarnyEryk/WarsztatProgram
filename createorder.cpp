//okno służące do utworzenia nowego zlecenia

#include "createorder.h"
#include "ui_createorder.h"

CreateOrder::CreateOrder(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateOrder)
{
    ui->setupUi(this);

    //ustawienie maski w celu wprowadzenia roku
    QIntValidator *yearValidator = new QIntValidator(1885, 2024, this);
    ui->year_input->setValidator(yearValidator);
    ui->year_input->setMaxLength(4);
    ui->year_input->setCursorPosition(0);

    //ustawienie maski dla pojemności silnika
    ui->volume_engine->setInputMask("9.9");
    ui->volume_engine->setCursorPosition(0);

    //ustawienie walidacji danych dla vin 17 znaków cyfry + duże litery
    QRegularExpression vinReg("^[A-HJ-NPR-Z0-9]{17}$");
    QValidator *vinValidator = new QRegularExpressionValidator(vinReg, this);
    ui->vin_input->setValidator(vinValidator);

    //ustawienie punktu początkowego naszego formularza
    ui->mark_input->setFocus();
}


CreateOrder::~CreateOrder()
{
    delete ui;
}


//funkcja do wrzucania danych do bazy
void CreateOrder::putDataToDb()
{
    //Pobranie danych z formularza
    QString mark = ui->mark_input->text().toUpper().trimmed();
    QString model = ui->model_input->text().toUpper().trimmed();
    QString year = ui->year_input->text().trimmed();
    QString volume = ui->volume_engine->text().trimmed();
    QString type = ui->type_input->currentText().toUpper().trimmed();
    QString vin = ui->vin_input->text().toUpper().trimmed();
    QString rej = ui->rej_input->text().toUpper().trimmed();
    QString desc = ui->desc_input->text().trimmed();

    //znacznik do poprawności walidacji danych
    bool walidacja = true;


    //walidacja pola marka
    if (mark.isEmpty())
    {
        ui->mark_label->setStyleSheet("color: red");
        walidacja = false;
    }
    else
    {
        ui->mark_label->setStyleSheet("color: white");
    }

    //walidacja pola model
    if (model.isEmpty())
    {
        ui->model_label->setStyleSheet("color: red");
        walidacja = false;
    }

    else
    {
        ui->model_label->setStyleSheet("color: white");
    }

    //walidacja pola rok
    if (year.isEmpty() || year.toInt() < 1885 || year.toInt() > 2024)
    {
        ui->year_label->setStyleSheet("color: red");
        walidacja = false;
    }

    else
    {
        ui->year_label->setStyleSheet("color: white");
    }

    //walidacja pola pojemność
    if (volume.isEmpty() || volume.length() < 3 || volume[0].isSpace())
    {
        ui->volume_label->setStyleSheet("color: red");
        walidacja = false;
    }

    else
    {
        ui->volume_label->setStyleSheet("color: white");
    }

    //walidacja pola typ silnika
    if (type.isEmpty())
    {
        ui->type_label->setStyleSheet("color: red");
        walidacja = false;
    }

    else
    {
        ui->type_label->setStyleSheet("color: white");
    }

    //walidacja pola vin
    QRegularExpression vinReg("^[A-HJ-NPR-Z0-9]{17}$");
    if (!vinReg.match(vin).hasMatch())
    {
        ui->vin_label->setStyleSheet("color: red");
        walidacja = false;
    }

    else
    {
        ui->vin_label->setStyleSheet("color: white");
    }

    //w alidacja pola numeru rejestracyjnego
    if (rej.isEmpty())
    {
        ui->rej_label->setStyleSheet("color: red");
        walidacja = false;
    }

    else
    {
        ui->rej_label->setStyleSheet("color: white");
    }

    //walidacja pola opisu
    if (desc.isEmpty())
    {
        ui->desc_label->setStyleSheet("color: red");
        walidacja = false;
    }

    else
    {
        ui->desc_label->setStyleSheet("color: white");
    }

    //jeżeli wszystkie pola są poprawne zatwierdź
    if (walidacja)
    {
        emit dataSubmit(mark, model, year, volume, type, vin, rej, desc);
        accept();
    }

    else
    {
        ui->empty_data_label->setStyleSheet("color: red");
        ui->empty_data_label->setText("Czerwone pola nie mogą być puste lub zawierać błędne dane!");
    }
}

//obsługa kliknięcia przycisku
void CreateOrder::on_pushButton_clicked()
{
    putDataToDb();
}

