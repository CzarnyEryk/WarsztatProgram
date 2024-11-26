#ifndef CREATEORDER_H
#define CREATEORDER_H

#include <QDialog>
#include <QRegularExpressionValidator>

namespace Ui {
class CreateOrder;
}

class CreateOrder : public QDialog
{
    Q_OBJECT

public:
    explicit CreateOrder(QWidget *parent = nullptr);
    ~CreateOrder();

signals:
    void dataSubmit(const QString &marka, const QString &model, const QString &rok, const QString &poj, const QString &typ, const QString &vin,
                    const QString &rej, const QString &opis);
private slots:
    void putDataToDb();
    void on_pushButton_clicked();

private:
    Ui::CreateOrder *ui;
};

#endif // CREATEORDER_H
