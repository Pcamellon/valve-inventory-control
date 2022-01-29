#include "base_datos.h"
#include "ui_base_datos.h"

Base_Datos::Base_Datos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Base_Datos)
{
    ui->setupUi(this);
    data_BaseVisual = new QSqlDatabase; //Inicialización de la variable que contendrá la base de datos
}

Base_Datos::~Base_Datos()
{
    delete ui;
}

void Base_Datos::on_salida_clicked()
{
    data_BaseVisual->close();
    this->close();
}

void Base_Datos::Scanner_BD(QSqlDatabase &a)
{
    *data_BaseVisual=a; //Acá estoy cargando la base de datos y posteriormente la abro
    data_BaseVisual->open();
}

void Base_Datos::load_simpleData()
{
    if(!data_BaseVisual->isOpen())
    {
        data_BaseVisual->open();
    }

    ////////////////////////////////////////////////
    //Acá estoy trabajando con el QSqlTableModel para cargar la base de datos e introducirlas dentro del TableView

    modelo=new QSqlTableModel(this);
    ui->tableView->setModel(modelo);
    ui->tableView->resizeColumnsToContents();
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelo->setTable("BaseDatosValvulas");
    modelo->removeColumn(0);
    modelo->select();
    modelo->sort(1,Qt::AscendingOrder);
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
}
