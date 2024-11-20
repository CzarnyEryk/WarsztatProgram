#include "createorder.h"
#include "ui_createorder.h"

CreateOrder::CreateOrder(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateOrder)
{
    ui->setupUi(this);
}


CreateOrder::~CreateOrder()
{
    delete ui;
}

void CreateOrder::on_pushButton_clicked()
{
    //pobranie danych z pól
    QString mark = ui->mark_input->text();
    QString model = ui->model_input->text();
    QString year = ui->year_input->text();
    QString volume = ui->volume_engine->text();
    QString type = ui->type_input->currentText();
    QString vin = ui->vin_input->text();
    QString rej = ui->rej_input->text();
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
        qDebug() << "Emit data wartośc model:  " + model;
        emit dataSubmit(mark, model, year, volume, type, vin, rej, desc);
        accept();
    }




}

