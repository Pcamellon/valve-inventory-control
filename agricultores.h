#ifndef AGRICULTORES_H
#define AGRICULTORES_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <base_datos.h>
#include <panel_opciones.h>

namespace Ui {
    class Agricultores;
}

class Agricultores : public QMainWindow
{
    Q_OBJECT

public:
    explicit Agricultores(QWidget *parent = 0);
    ~Agricultores();
    void Cargar_BD(QSqlDatabase &); //Funcion encargada de cargar la base de datos con la cual se cargó en el primer panel


private:
    Ui::Agricultores *ui;
    QSqlDatabase *dataBase_Valvulas; //Creación de la base de la variable que va a contener la base de datos
    Base_Datos *w; //Objeto creado de la clase Base_Datos
    Panel_Opciones *v; //Objeto creado de la clase Panel_Opciones
    QString tipo_valvula; //Variable que me va a permitir escanear el tipo de válvulas donde el Agricultor quiere realzar alguna acción

private slots:
    void on_actionVer_Base_de_Datos_triggered();
    void on_salida_clicked();

    /////////////////////////////////////////////
    //Todos estos Slots me van a permitir darle un valor a la variable tipo_Valvula a partir del boton que pulse el usuario (Agricultor)
    void on_VERPresion_clicked();
    void on_VSAlivio_clicked();
    void on_VRPresion_clicked();
    void on_VCSolenoide_clicked();
    void on_VBasica_clicked();
    //////////////////////////////////////////////////////////
    void emitir_senha (); //Slot que me va a permitir emitir la señal que le indica al panel contraseñas que el panel Agricultor está a punto de cerrar

signals:
    void senha(); //Señal propia del panel Agricultor
};

#endif // AGRICULTORES_H
