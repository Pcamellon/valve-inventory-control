#ifndef ENTRADA_SERIAL_MODIFICAR_H
#define ENTRADA_SERIAL_MODIFICAR_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <interfaz_modificacion.h>

namespace Ui {
    class Entrada_serial_modificar;
}

class Entrada_serial_modificar : public QDialog
{
    Q_OBJECT

public:
    explicit Entrada_serial_modificar(QWidget *parent = 0);
    ~Entrada_serial_modificar();
    int a; //Variable que contirne el modo en que se va a activar la interfaz (eliminación o modificación)
    QSqlDatabase db; //Variable que contiene la base de datos
    QString tValvula; //Varable que contiene el tipo de valvula con la cual se va a trabajar
    void actualizar_Listado(); //Variable para actualizar los elementos contenidos en el textEdit con relación a los elementos contenidos en la base de datos
    Interfaz_modificacion *w; //Objeto de la clase interfaz_Modificación (se utilizará para introducir los parámetros que se desea modificar)


private:
    Ui::Entrada_serial_modificar *ui;

private slots:
    void on_Ejecutar_clicked();
    void on_Cerrar_clicked();
    void modificacion_Implementacion(); //Acá se implementa el algoritmo que usamos para modificar los elementos de la base de datos
};

#endif // ENTRADA_SERIAL_MODIFICAR_H
