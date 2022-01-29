#ifndef INTERFAZ_ADICCION_H
#define INTERFAZ_ADICCION_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
    class interfaz_Adiccion;
}

class interfaz_Adiccion : public QDialog
{
    Q_OBJECT

public:
    explicit interfaz_Adiccion(QWidget *parent = 0);
    ~interfaz_Adiccion();
    QString tipo_Valvula; //Variable que va a contener el tipo de Válvula que se va a introducir
    QSqlDatabase db; //Variable que va a contener la Base de Datos

private:
    Ui::interfaz_Adiccion *ui;

private slots:
    void on_insertar_clicked(); //Slot para incertar la nueva válvula
    void on_cancelar_clicked();
};

#endif // INTERFAZ_ADICCION_H
