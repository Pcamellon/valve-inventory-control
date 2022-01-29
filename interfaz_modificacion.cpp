#include "interfaz_modificacion.h"
#include "ui_interfaz_modificacion.h"

Interfaz_modificacion::Interfaz_modificacion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Interfaz_modificacion)
{
    ui->setupUi(this);
}

Interfaz_modificacion::~Interfaz_modificacion()
{
    delete ui;
}

void Interfaz_modificacion::on_modificar_clicked()
{
    this->emitir_Senha();
    ui->Flujo_line->clear();
    ui->Plantacion_line->clear();
    ui->Serial_line->clear();
    this->close();
}

void Interfaz_modificacion::on_salir_clicked()
{
    ui->Flujo_line->clear();
    ui->Plantacion_line->clear();
    ui->Serial_line->clear();
    this->close();
}

QString Interfaz_modificacion::Sacnner_Flujo()
{
    return ui->Flujo_line->text();
}

QString Interfaz_modificacion::Scanner_modelo()
{
    return ui->Modelo_line->text();
}

QString Interfaz_modificacion::Scanner_Plantacion()
{
    return ui->Plantacion_line->text();
}

QString Interfaz_modificacion::Scanner_Serial()
{
    return ui->Serial_line->text();
}

void Interfaz_modificacion::emitir_Senha()
{
    emit Senha();
}
