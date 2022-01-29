#include "entrada_serial_modificar.h"
#include "ui_entrada_serial_modificar.h"

Entrada_serial_modificar::Entrada_serial_modificar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Entrada_serial_modificar)
{
    ui->setupUi(this);
    w = new Interfaz_modificacion;

    //A partir de una se�al que me va a emitir la interfaz donde se escriben los elementos a modificar se va a aplicar el algoritmo de modificaci�n
    connect(w, SIGNAL(Senha()), this, SLOT(modificacion_Implementacion())); 
}

Entrada_serial_modificar::~Entrada_serial_modificar()
{
    delete ui;
}

void Entrada_serial_modificar::on_Cerrar_clicked()
{
    db.close();
    ui->Contenedor->clear();
    a=0;
    tValvula="";
    this->close();
}
void Entrada_serial_modificar::actualizar_Listado()
{
    ///////////////////////////////////////////////////////////////
    //Aqu� se diferencia en lo visual la interfaz de modificaci�n y la de eliminaci�n

    db.open();
    if(a==1)
    {
        ui->label->setText("Introduzca el serial que desea eliminar");
        this->setWindowTitle("Eliminacion de Valvulas");

    }
    if(a==2)
    {
        ui->label->setText("Introduzca el serial que desea modificar");
        this->setWindowTitle("Modificacion de Valvulas");
    }

    //////////////////////////////////////////////////////////////////

    //Posteriormente el siguiente algritmo mostrado me permite poner los elementos de la base de datos denro del TextEdit de acorde al tipo de valvula con l cual se est� trabajando

    QString a, b, c, d, e;
    a="Serial: ";
    b="Tipo: ";
    c="Modelo: ";
    d="Plantacion: ";
    e="Flujo maximo: ";
    QSqlQuery consulta("SELECT * FROM BaseDatosValvulas");
    int field0 = consulta.record().indexOf("Serial");
    int field1 = consulta.record().indexOf("Tipo");
    int field2 = consulta.record().indexOf("Modelo");
    int field3 = consulta.record().indexOf("Plantacion");
    int field4 = consulta.record().indexOf("Flujo_maximo");
    while(consulta.next())
        {
            if(consulta.value(field1).toString()==tValvula)
            {
                ui->Contenedor->append(a+" "+consulta.value(field0).toString());
                ui->Contenedor->append(b+" "+consulta.value(field1).toString());
                ui->Contenedor->append(c+" "+consulta.value(field2).toString());
                ui->Contenedor->append(d+" "+consulta.value(field3).toString());
                ui->Contenedor->append(e+" "+QString ("%1").arg(consulta.value(field4).toDouble()));
                ui->Contenedor->append("\n");
            }
        }

}

void Entrada_serial_modificar::on_Ejecutar_clicked()
{
    //////////////////////////////////////////////////////////////
    //Ac� se establece como que va a realizar el bot�n efectuar dependiendo si est� en modo de eliminaci�n(1) o modificaci�n(2)

    QString modificacion="";
    if(a==1)
    {
        QSqlQuery encuesta;
        QSqlQuery encuesta1;
        int i=0;
        if(ui->Serial_cuestion->text()!="")
        {
            encuesta.exec("Select * From BaseDatosValvulas");
            int fieldO = encuesta.record().indexOf("Serial");
            int fieldB = encuesta.record().indexOf("Id");
            while(encuesta.next())
            {
                if(encuesta.value(fieldO).toString()==ui->Serial_cuestion->text())
                {
                    modificacion=encuesta.value(fieldB).toString();
                    i=1;
                }
            }
            if(i==1)
            {
                encuesta1.exec("DELETE * FROM BaseDatosValvulas WHERE Id="+modificacion);
                ui->Contenedor->clear();
                modificacion="";
                ui->Serial_cuestion->clear();
                actualizar_Listado();
            }
            else
            {
                QMessageBox::warning(this, "Error", "No se ha encontrado ningun elemento\n que coincida con el serial introducido\nVuelva a intentarlo", QMessageBox::Ok);
                ui->Serial_cuestion->clear();
            }
        }
        else
            QMessageBox::warning(this, "Error", "No ha introducido ningun serial\nVuelva a intentarlo:-)", QMessageBox::Ok);
    }
    if(a==2)
    {
        if(ui->Serial_cuestion->text()!="")
        {
            int i=0;
            QSqlQuery encuesta;
            encuesta.exec("Select * From BaseDatosValvulas");
            int field1 = encuesta.record().indexOf("Serial");
            while(encuesta.next())
            {
                if(encuesta.value(field1).toString()==ui->Serial_cuestion->text())
                {
                    i=1;
                }
            }
            if(i==1)
            {
                w->exec(); //Aqu� se manda a abrir la interfaz donde se van a introducir los elemenos a modificar
                ui->Serial_cuestion->clear();
            }
            else
            {
                QMessageBox::warning(this, "Error", "No se ha encontrado ningun elemento\n que coincida con el serial introducido\nVuelva a intentarlo", QMessageBox::Ok);
                ui->Serial_cuestion->clear();
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "No ha introducido ningun serial\nVuelva a intentarlo:-)", QMessageBox::Ok);
        }
    }

    /////////////////////////////////////////////////////////////////////
}
void Entrada_serial_modificar::modificacion_Implementacion()
{
    ///////////////////////////////////////////////////////////////////////
    //Est� funci�n lo que va a ser es los siguiente
    //1-Copiar todo los elementos que se encuentra en la base de datos que corresponden con el serial introducido en el lineEdit (serial) y auxiliandome del id en variables temporalmente creadas en el presente slot
    //2-Eliminar los elementos que se encuentra en la base de datos......excepto el id para posteriormente mantenerlo en la misma posici�n de la v�lvula sin ser modificada
    //3-Copiar los elementos temporales si est�n vacios sus correspondientes lineEdit en la otra interfaz, de no ser as� se introduce el elemnto escaneado del lineedit de la otra interfaz

    QSqlQuery encuesta;
    QString modificacion;

    QString serial_temporal;
    QString plantacion_temporal;
    QString flujo_temporal;
    QString modelo_temporal;
    QString tipo_temporal;

    ////////////////////////////////////////////////////////////////////
    //1

    encuesta.exec("Select * From BaseDatosValvulas");
    int field1 = encuesta.record().indexOf("Serial");
    int field0 = encuesta.record().indexOf("Id");
    int field4 = encuesta.record().indexOf("Plantacion");
    int field5 = encuesta.record().indexOf("Flujo_maximo");
    int field2 = encuesta.record().indexOf("Tipo");
    int field3 = encuesta.record().indexOf("Modelo");
    while(encuesta.next())
    {
        if(encuesta.value(field1).toString()==ui->Serial_cuestion->text())
        {
            modificacion=encuesta.value(field0).toString();
            serial_temporal=encuesta.value(field1).toString();
            plantacion_temporal=encuesta.value(field4).toString();
            flujo_temporal=encuesta.value(field5).toString();
            modelo_temporal=encuesta.value(field3).toString();
            tipo_temporal=encuesta.value(field2).toString();
        }
    }

    //////////////////////////////////////////////////////////////////////
    //2
    encuesta.exec("Delete * FROM BaseDatosValvulas WHERE Id="+modificacion);

    ///////////////////////////////////////////////////////////////////
    //3
    encuesta.prepare("INSERT INTO BaseDatosValvulas (Id, Serial, Tipo, Modelo, Plantacion, Flujo_maximo)"
                      "Values (?, ?, ?, ?, ?, ?)");
    encuesta.addBindValue(modificacion.toInt());
    if(w->Scanner_Serial()=="")
        encuesta.addBindValue(serial_temporal);
    else
        encuesta.addBindValue(w->Scanner_Serial());
    encuesta.addBindValue(tipo_temporal);
    if(w->Scanner_modelo()=="")
        encuesta.addBindValue(modelo_temporal);
    else
        encuesta.addBindValue(w->Scanner_modelo());
    if(w->Scanner_Plantacion()=="")
        encuesta.addBindValue(plantacion_temporal);
    else
        encuesta.addBindValue(w->Scanner_Plantacion());
    if(w->Sacnner_Flujo()=="")
        encuesta.addBindValue(flujo_temporal.toInt());
    else
        encuesta.addBindValue(w->Sacnner_Flujo().toInt());
    encuesta.exec();
    ui->Contenedor->clear();
    modificacion="";
    actualizar_Listado();
}
