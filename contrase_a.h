#ifndef CONTRASE_A_H
#define CONTRASE_A_H

#include <QMainWindow>
#include <agricultores.h>
#include <controladores.h>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDir>

namespace Ui {
    class Contrase_a;
}

class Contrase_a : public QMainWindow
{
    Q_OBJECT

public:
    explicit Contrase_a(QWidget *parent = 0);
    ~Contrase_a();

private:
    Ui::Contrase_a *ui;
    Agricultores *w; //Objeto creado de la clase Agricultores
    Controladores *v; //Objeto creado de la clase Controladores
    QSqlDatabase *dataBase_Datos; //Creación de la base de la variable que va a contener la base de datos

private slots:
    void on_Salida_clicked();
    void on_Entrada_clicked();
    void reinicio(); //Slot para reiniciar la interfaz cuando se vuelva a la portada de contraseñas
    void Cargar_Agronomo(); //Slot creado para cargar el panel de agrónomo usando las funciones públicas de la clase Controladores
    void Cargar_Controlador(); //Slot creado para cargar el panel de controlador usando las funciones públicas de la clase Controladores

};

#endif // CONTRASE_A_H
