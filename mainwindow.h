#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QSqlDatabase db, int id, QString rola, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createOrder_clicked();
    void openCreateDialog();
    void getDataDialogUpploadDb(const QString &marka, const QString &model, const QString &rok, const QString &poj, const QString &typ, const QString &vin,
                       const QString &rej, const QString &opis);

    void on_checkOrder_clicked();

    void on_checkData_clicked();

    void on_deleteAcount_clicked();

    void on_logout_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase m_db;
    int m_id;
    QString m_rola;

};
#endif // MAINWINDOW_H
