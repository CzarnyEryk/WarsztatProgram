#include "createorder.h"
#include "ui_createorder.h"

CreateOrder::CreateOrder(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateOrder)
{
    ui->setupUi(this);
    ui->year_input->setInputMask("99.99.9999;_");
    ui->year_input->setMaxLength(10);
    ui->year_input->setCursorPosition(0);
}


CreateOrder::~CreateOrder()
{
    delete ui;
}


//funkcja do wrzucania danych do bazy
void CreateOrder::putDataToDb()
{
    //pobranie danych z pól
    QString mark = ui->mark_input->text().toUpper();
    QString model = ui->model_input->text().toUpper();
    QString year = ui->year_input->text().toUpper();
    QString volume = ui->volume_engine->text().toUpper();
    QString type = ui->type_input->currentText().toUpper();
    QString vin = ui->vin_input->text().toUpper();
    QString rej = ui->rej_input->text().toUpper();
    QString desc = ui->desc_input->text();

    //zaznaczenie pustych pól
    if (mark.isEmpty()) ui->mark_label->setStyleSheet("color: red");
    if (model.isEmpty()) ui->model_label->setStyleSheet("color: red");
    if (year.isEmpty()) ui->year_label->setStyleSheet("color: red");
    if (volume.isEmpty()) ui->volume_label->setStyleSheet("color: red");
    if (type.isEmpty()) ui->type_label->setStyleSheet("color: red");
    if (vin.isEmpty()) ui->vin_label->setStyleSheet("color: red");
    if (rej.isEmpty()) ui->rej_label->setStyleSheet("color: red");
    if (desc.isEmpty()) ui->desc_label->setStyleSheet("color: red");

    //po kolejnym kliknieciu odznaczyć pola które są uzupełnione
    if (!mark.isEmpty()) ui->mark_label->setStyleSheet("color: white");
    if (!model.isEmpty()) ui->model_label->setStyleSheet("color: white");
    if (!year.isEmpty()) ui->year_label->setStyleSheet("color: white");
    if (!volume.isEmpty()) ui->volume_label->setStyleSheet("color: white");
    if (!type.isEmpty()) ui->type_label->setStyleSheet("color: white");
    if (!vin.isEmpty()) ui->vin_label->setStyleSheet("color: wite");
    if (!rej.isEmpty()) ui->vin_label->setStyleSheet("color: wite");
    if (!desc.isEmpty()) ui->desc_label->setStyleSheet("color: white");

    //sprawdzenie czy któreś pole nie jest puste
    if ( mark.isEmpty() || model.isEmpty() || year.isEmpty() || volume.isEmpty() || type.isEmpty() ||
        vin.isEmpty() || desc.isEmpty() )
    {
        ui->empty_data_label->setStyleSheet("color: red");
        ui->empty_data_label->setText("Czerwone pola nie mogą być puste !!!");
    }
    else
    {
        emit dataSubmit(mark, model, year, volume, type, vin, rej, desc);
        accept();
    }
}
void CreateOrder::on_pushButton_clicked()
{
    putDataToDb();
}

