#include "contrase_a.h"
#include "ui_contrase_a.h"

Contrase_a::Contrase_a(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Contrase_a)
{
    ui->setupUi(this);
    w = new Agricultores; //Inicializaci�n del objeto referenciado al agricultor
    v = new Controladores; //Inicializaci�n del objeto referenciado al controlador
    dataBase_Datos = new QSqlDatabase; //Inicializaci�n de la variable de la base de datos
    connect(w, SIGNAL(senha()), this, SLOT(reinicio())); //Conexi�n para reestablecer el panel de contrase�as para cuando el objeto agricultor se cierre
    connect(v, SIGNAL(senha()), this, SLOT(reinicio())); //Conexi�n para reestablecer el panel de contrase�as para cuando el objeto controlador se cierre
}

Contrase_a::~Contrase_a()
{
    delete ui;
}

void Contrase_a::on_Entrada_clicked()
{
    //////////////////////////////////////////////////////////////////////

    QString ruta = QString ("%1/DataBases/Base_datos_PF.mdb").arg(QDir::currentPath());
    ruta.replace("/","//");
    *dataBase_Datos = QSqlDatabase::addDatabase("QODBC");
    dataBase_Datos->setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ="+ruta);
    dataBase_Datos->open();

    ////////////////////////////////////////////////////////////////////////
    //Lo anterior es para cargar la base de datos
    //En la linea 25 establezco la direcci�n predefiida en la base de datos la cual se encuentra en la carpeta de origen o sea en el built o en el ejcutable

    ////////////////////////////////////////////////////////////////////////
    //ui->comboBox es el combo Box donde el usuario elige su especializaci�n siendo 1(Agricultor) y 2(Controlador)

    QSqlQuery *consultaAgronomo = new QSqlQuery(*dataBase_Datos);
    QSqlQuery *consultaAutomatico = new QSqlQuery(*dataBase_Datos);
    int a=0;
    int opcion = ui->comboBox->currentIndex();
    if(opcion==0 || ui->contrasenha_Line->text()=="" || ui->usuario_Line->text()=="")
    {
        QMessageBox::critical(this,"Error","Datos insuficientes", QMessageBox::Ok);
    }
    if(opcion==1) //Si se eligi� la especialidad de Agronomo en el combo box entonces el query trabajar� con la tabla donde se encuentra las usuarios con sus respectivas contrase�as
    {
        consultaAgronomo->exec("SELECT * FROM Ingenieros_Agr�nomo");
        int nombre_usuario = consultaAgronomo->record().indexOf("Usuario");
        int contrasenha_usuario = consultaAgronomo->record().indexOf("Contrase�a");
        while(consultaAgronomo->next())
        {
            if(ui->usuario_Line->text()==consultaAgronomo->value(nombre_usuario).toString()&&ui->contrasenha_Line->text()==consultaAgronomo->value(contrasenha_usuario).toString())
            {
                a=1; //Si existe coincidencia
                Cargar_Agronomo();
            }
        }
        if(a==0)
        {
            //Al no existir coincidencia
            QMessageBox::warning(this,"Atencion","Datos incorrectos", QMessageBox::Ok);
            ui->contrasenha_Line->setText("");
            ui->usuario_Line->setText("");
            ui->comboBox->setCurrentIndex(0);
        }
    }
    if(opcion==2) //Si se eligi� la especialidad de Controlador en el combo box entonces el query trabajar� con la tabla donde se encuentra las usuarios con sus respectivas contrase�as
    {
        a=0;
        consultaAutomatico->exec("SELECT * FROM Ingenieros_Autom�ticos");
        int nombre_usuario = consultaAutomatico->record().indexOf("Usuario");
        int contrasenha_usuario = consultaAutomatico->record().indexOf("Contrase�a");
        while(consultaAutomatico->next())
        {
            if(ui->usuario_Line->text()==consultaAutomatico->value(nombre_usuario).toString()&&ui->contrasenha_Line->text()==consultaAutomatico->value(contrasenha_usuario).toString())
            {
                a=1; //Si existe coincidencia
                Cargar_Controlador();
            }
        }
        if(a==0)  //Al no existir coincidencia
        {
            QMessageBox::warning(this,"Atencion","Datos incorrectos", QMessageBox::Ok);
            ui->contrasenha_Line->setText("");
            ui->usuario_Line->setText("");
            ui->comboBox->setCurrentIndex(0);
        }
    }
    a=0;
}

void Contrase_a::on_Salida_clicked()
{
    this->close();
}

void Contrase_a::reinicio()
{
    dataBase_Datos->open(); //Abre la base de datos ya anteriormente cargada cuando se vuelva a abrir la interfaz de contrase�as
    this->setEnabled(true); //Habilitar el panel de contrase�as
}

void Contrase_a::Cargar_Agronomo()
{
    w->Cargar_BD(*dataBase_Datos); //Funci�n que permite que Agricultores cargue la base de datos ya cargada inicialmente
    w->setEnabled(true); //Habilita el panel de Agr�nomo
    w->show();
    this->setEnabled(false); //Deshabilita el panel de contrase�a cuando se abre el otro panel

    //Inicializa los elementos de inscripci�n del panel de contrase�a
    ui->contrasenha_Line->clear();
    ui->usuario_Line->clear();
    ui->comboBox->setCurrentIndex(0);
}

void Contrase_a::Cargar_Controlador()
{
    v->Cargar_BD(*dataBase_Datos); //Funci�n que permite que Controladores cargue la base de datos ya cargada inicialmente
    v->Cargar_Limite(); //Permite la creaci�n de un inputDiialog para imponer el consumo diario permitido
    v->Cargar_Panel(); //Permite cargar el panel de control a partir de los elementos que se encuentra en la base de datos (tabla de valvulas)
    v->setEnabled(true); //Habilita el panel de Controlador
    v->showMaximized(); //Muestra el panel de control maximizado
    this->setEnabled(false); //Deshabilita el panel de contrase�a cuando se abre el otro panel

    //Inicializa los elementos de inscripci�n del panel de contrase�a
    ui->contrasenha_Line->clear();
    ui->usuario_Line->clear();
    ui->comboBox->setCurrentIndex(0);
}
