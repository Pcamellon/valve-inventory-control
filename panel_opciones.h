#ifndef PANEL_OPCIONES_H
#define PANEL_OPCIONES_H

#include <QDialog>
#include <QSqlDatabase>
#include <interfaz_adiccion.h>
#include <entrada_serial_modificar.h>

namespace Ui {
    class Panel_Opciones;
}

class Panel_Opciones : public QDialog
{
    Q_OBJECT

public:
    explicit Panel_Opciones(QWidget *parent = 0);
    ~Panel_Opciones();
    void Scanner_elementosNecesarios(QString &, QSqlDatabase &); //Función que carga la base de datos con la cual estamos trabajando desde el inicio y el tipo de válvula en específico con el cual el usuario quiere trabajar


private:
    Ui::Panel_Opciones *ui;
    QString tipo_Valvula; //Variable dedicada a guardar el tipo de válvula con el cual el usuario quiere trabajar
    QSqlDatabase *dataBase; //Creación de la base de la variable que va a contener la base de datos
    interfaz_Adiccion *w; //Objeto creado de la clase interfaz_Adiccion
    Entrada_serial_modificar *v; //Objeto creado de la clase Entrada_serial_modificar

private slots:
    void on_add_in_DB_clicked(); //Slot que va a efectuarse cuando el usuario elige adicionar una válvula
    void on_mod_in_DB_clicked(); //Slot que va a efectuarse cuando el usuario elige modificar una válvula existente
    void on_delete_in_DB_clicked(); //Slot que va a efectuarse cuando el usuario elige eliminar una válvula existente

    void on_salida_clicked();
};

#endif // PANEL_OPCIONES_H
