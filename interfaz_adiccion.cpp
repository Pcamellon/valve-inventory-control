#include "interfaz_adiccion.h"
#include "ui_interfaz_adiccion.h"

interfaz_Adiccion::interfaz_Adiccion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::interfaz_Adiccion)
{
    ui->setupUi(this);
}

interfaz_Adiccion::~interfaz_Adiccion()
{
    delete ui;
}

void interfaz_Adiccion::on_cancelar_clicked()
{
    db.close();
    tipo_Valvula="";
    this->close();
}

void interfaz_Adiccion::on_insertar_clicked()
{
    db.open();

    ////////////////////////////////////////////////////
    //Si todos los lineEdit tienen información escrita entonces se va a introducir la información dentro de la Base de Datos

    if(ui->flujo_Line_2->text()!="" && ui->modelo_Line_2->text()!="" && ui->serial_Line->text()!="" && ui->plantacion_Line_2->text()!="")
    {
        QString serial, model, plantacion, flujo_maximo;
        flujo_maximo=flujo_maximo.number(ui->flujo_Line_2->text().toInt());
        serial=ui->serial_Line->text();
        model=ui->modelo_Line_2->text();
        plantacion=ui->plantacion_Line_2->text();

        //Aquí se usa el Query para introducir los elementos dentro de la Base de Datos

        QSqlQuery encuesta;
        encuesta.prepare("INSERT INTO BaseDatosValvulas (Serial, Tipo, Modelo, Plantacion, Flujo_maximo)"
                         "Values (?, ?, ?, ?, ?)");
        encuesta.addBindValue(serial);
        encuesta.addBindValue(tipo_Valvula);
        encuesta.addBindValue(model);
        encuesta.addBindValue(plantacion);
        encuesta.addBindValue(flujo_maximo);
        encuesta.exec();
        ui->flujo_Line_2->clear();
        ui->modelo_Line_2->clear();
        ui->plantacion_Line_2->clear();
        ui->serial_Line->clear();
    }
    else
    {
        QMessageBox::warning(this, "Error", "No ha introducido todos\nlos datos necesarios para realizar la accion\nVuelva a intentarlo", QMessageBox::Ok);
        ui->flujo_Line_2->clear();
        ui->modelo_Line_2->clear();
        ui->plantacion_Line_2->clear();
        ui->serial_Line->clear();
    }
}
