#include "agricultores.h"
#include "ui_agricultores.h"

Agricultores::Agricultores(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Agricultores)
{
    ui->setupUi(this);

    //Inicializaci�n del objeto referenciado a Panel_Opciones
    v = new Panel_Opciones;
    
    //Inicializaci�n del objeto referenciado a  Base_Datos
    w = new Base_Datos;
    
    //Inicializaci�n de la variable de la base de datos
    dataBase_Valvulas= new QSqlDatabase;
}

Agricultores::~Agricultores()
{
    delete ui;
}

//Aca estoy cargando por referencia la base de datos
void Agricultores::Cargar_BD(QSqlDatabase &a)
{
    *dataBase_Valvulas=a;
}

void Agricultores::on_salida_clicked()
{
    dataBase_Valvulas->close();
    this->setEnabled(false);
    this->emitir_senha();
    this->close();
}

void Agricultores::on_VBasica_clicked()
{
    tipo_valvula="Valvula Basica";
    v->Scanner_elementosNecesarios(tipo_valvula, *dataBase_Valvulas); //Funci�n que le permite a panel de opciones cargar la base de datos y el tipo de v�lvula con el cual se est� trabajando
    tipo_valvula="";
    v->exec();
}

void Agricultores::on_VCSolenoide_clicked()
{
    tipo_valvula="Valvula de Control de Solenoide";
    v->Scanner_elementosNecesarios(tipo_valvula, *dataBase_Valvulas); //Iden....
    tipo_valvula="";
    v->exec();
}

void Agricultores::on_VRPresion_clicked()
{
    tipo_valvula="Valvula Reductora de Presion";
    v->Scanner_elementosNecesarios(tipo_valvula, *dataBase_Valvulas); //Iden....
    tipo_valvula="";
    v->exec();
}

void Agricultores::on_VSAlivio_clicked()
{
    tipo_valvula="Valvula de Seguridad de Alivio Rapido";
    v->Scanner_elementosNecesarios(tipo_valvula, *dataBase_Valvulas); //Iden....
    tipo_valvula="";
    v->exec();
}

void Agricultores::on_VERPresion_clicked()
{
    tipo_valvula="Valvula Electrica Reductora de Presion";
    v->Scanner_elementosNecesarios(tipo_valvula, *dataBase_Valvulas); //Iden....
    tipo_valvula="";
    v->exec();
}

void Agricultores::on_actionVer_Base_de_Datos_triggered()
{
    w->Scanner_BD(*dataBase_Valvulas); //Funcion que permite cargar la base de datos para la interfaz Base_Datos
    w->load_simpleData(); //Funci�n encargada a cargar el TableView de la interfaz Base_Datos
    w->exec();
}

void Agricultores::emitir_senha()
{
    emit senha();
}
