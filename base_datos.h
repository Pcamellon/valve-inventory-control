#ifndef BASE_DATOS_H
#define BASE_DATOS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
    class Base_Datos;
}

class Base_Datos : public QDialog
{
    Q_OBJECT

public:
    explicit Base_Datos(QWidget *parent = 0);
    ~Base_Datos();
    void Scanner_BD(QSqlDatabase &); //Función encargada de escanear la base de datos con la cual se está trabajando
    void load_simpleData(); //Función que cargará el TableView de la interfaz de Base_Datos con los elementos de la base de datos ya anteriormente cargada

private:
    Ui::Base_Datos *ui;
    QSqlDatabase *data_BaseVisual; //Creación de la variable que va a contener la base de datos
    QSqlTableModel *modelo; //Creación de variable que va a cargar los elementos de la base de datos para introducirlos en el TableView

private slots:
    void on_salida_clicked();
};

#endif // BASE_DATOS_H
