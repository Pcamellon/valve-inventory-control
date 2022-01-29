#include "panel_opciones.h"
#include "ui_panel_opciones.h"

Panel_Opciones::Panel_Opciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Panel_Opciones)
{
    ui->setupUi(this);
    dataBase=new QSqlDatabase;
    w = new interfaz_Adiccion;
    v = new Entrada_serial_modificar;
}

Panel_Opciones::~Panel_Opciones()
{
    delete ui;
}

void Panel_Opciones::on_salida_clicked()
{
    tipo_Valvula="";
    this->close();
}

void Panel_Opciones::on_delete_in_DB_clicked()
{
    v->a=1; //Esta es una variable de Entrada_serial_modificar con la cual me va a permitir inicializar esa interfaz en modo de eliminación de válvulas
    v->db=*dataBase; //Esta es una variable de Entrada...... con la cual se cargará la base de datos con la cual estamos trabajando para la siguiente interfaz
    v->tValvula=tipo_Valvula; //Esta es una variable de Entrada..... con la cual se cargará en la siguiente interfaz el tipo de válvula con el cual estamos trabajando
    v->actualizar_Listado(); //Función encargada de Actualizar el QTextEdit con los elementos existentes en la base de datos en la interfaz Entrada....
    v->exec();
}

void Panel_Opciones::on_mod_in_DB_clicked()
{
    v->a=2; //Esta es una variable de Entrada_serial_modificar con la cual me va a permitir inicializar esa interfaz en modo de modificación de válvulas
    v->db=*dataBase; //Esta es una variable de Entrada...... con la cual se cargará la base de datos con la cual estamos trabajando para la siguiente interfaz
    v->tValvula=tipo_Valvula; //Esta es una variable de Entrada..... con la cual se cargará en la siguiente interfaz el tipo de válvula con el cual estamos trabajando
    v->actualizar_Listado(); //Función encargada de Actualizar el QTextEdit con los elementos existentes en la base de datos en la interfaz Entrada....
    v->exec();
}

void Panel_Opciones::on_add_in_DB_clicked()
{
    w->tipo_Valvula=tipo_Valvula; //Esta es una variable de interfaz_Adiccion con la cual se cargará en la siguiente interfaz el tipo de válvula con el cual estamos trabajando
    w->db=*dataBase; //Esta es una variable de interfaz_Adicción con la cual se cargará la base de datos con la cual estamos trabajando para la siguiente interfaz
    w->exec();
}
void Panel_Opciones::Scanner_elementosNecesarios(QString &a, QSqlDatabase &b) //Función que carga a variables locales de la interfaz tanto la base de datos como el tipo de válvulas a trabajar
{
    tipo_Valvula=a;
    *dataBase=b;
}
