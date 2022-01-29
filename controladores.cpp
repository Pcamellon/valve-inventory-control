#include "controladores.h"
#include "ui_controladores.h"


Controladores::Controladores(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Controladores)
{
    ui->setupUi(this);
    cValvulas=0;
    consumo_en_Tiempo=0;
    segundos_restantes_Total=0;
    total_M3Consumidos=0;
    limite_Diario=0;
    abc = "  (m^3/s)";
    ui->icono->hide();
    ui->icono_2->hide();
    ah=0;
}

Controladores::~Controladores()
{
    delete ui;
}

void Controladores::Cargar_BD(QSqlDatabase &aa)
{
    dataBase_Valvulas = new QSqlDatabase;
    *dataBase_Valvulas=aa;
    segundos_restantes_Real=86401;
    calculo_MinutosRestantes= new QTimer;
    calculo_MinutosRestantes->start(1000);
    empresa_Acueducto = new QTimer;
    empresa_Acueducto->start(1);
    connect(calculo_MinutosRestantes, SIGNAL(timeout()), this, SLOT(Establecer_Horarios()));
    connect(empresa_Acueducto, SIGNAL(timeout()), this, SLOT(inspeccion()));
}

void Controladores::Cargar_Limite() //Cera un InputDialog que permite escanear el consumo diario establecido para la empresa
{
    QString txt=QInputDialog::getText(this,"Consumo diario","Introduzca el consumo diario permitido");
    limite_Diario=txt.toInt();
    ui->consumido_Total->setRange(0, limite_Diario);
}

void Controladores::on_salida_clicked()
{
    cerrar_Panel();
    contenido->close();
    ui->consumido_Parcial->clear();
    ui->consumido_Total->setValue(0);
    total_M3Consumidos=0;
    calculo_MinutosRestantes->stop();
    this->setEnabled(false);
    this->emitir_senha();
    this->close();
}

void Controladores::Cargar_Panel()
{
    contenido = new QScrollArea;
    final = new QWidget;
    consumo_automatizado = new QTime(0,0,0,0);
    QVBoxLayout *organizador_Widget = new QVBoxLayout;

    /////////////////////////////////////////////////////////
    //Aquí abro la base de datos y posteriormente escaneo cada uno de las valvulas que se encuentran en la Base de Datos y le determino la cantidad de valvulas registradas
    dataBase_Valvulas->open();
    QSqlQuery *consulta = new QSqlQuery;
    consulta->exec("Select * from BaseDatosValvulas");
    int fieldSerial = consulta->record().indexOf("Serial");
    int fieldTipo = consulta->record().indexOf("Tipo");
    int fieldModelo = consulta->record().indexOf("Modelo");
    int fieldPlantacion = consulta->record().indexOf("Plantacion");
    int fieldFlujo = consulta->record().indexOf("Flujo_maximo");
    int iterator=0;
    while(consulta->next())
    {
        a << "Info Valvula:\nSerial: " + consulta->value(fieldSerial).toString() + "\nTipo: "+consulta->value(fieldTipo).toString()+"\nModelo: "+consulta->value(fieldModelo).toString()+"\nPlantacion: "+consulta->value(fieldPlantacion).toString()+"\nFlujo maximo: "+consulta->value(fieldFlujo).toString() + "  (m^3/s)";
        max_Flujo.insert(iterator, consulta->value(fieldFlujo).toInt());
        iterator++;
        cValvulas++;
    }
    /////////////////////////////////////////////////////////////////////////////

    QWidget *widgetSupremo[cValvulas];
    QLabel *cartelito_Porcentaje[cValvulas];
    QLabel *cartelito_Tiempo[cValvulas];
    QLabel *infoValvulas[cValvulas];
    QHBoxLayout *boxDialProgreso[cValvulas];
    QVBoxLayout *boxBoxInfo[cValvulas];
    QVBoxLayout *boxTime[cValvulas];
    QHBoxLayout *todoAll[cValvulas];
    QVBoxLayout *betweenFlujo_Reverse[cValvulas];
    for(int i=0; i<cValvulas; ++i)
    {
        /////////////////////////////////////////////////////////////////////
        //Aquí estoy creando las tiras de válvulas, todo a base de código debido a que tiene que ser creado dinámicamente
        //También establesco las conexiones necesarias para que la interfaz haga las acciones que se piden

        b<<"Modifique el flujo de la valvula";
        c<<"Introduzca el tiempo que desea activar\nautomaticamente la valvula";
        Efectuar[i] = new QPushButton("Efectuar");
        Dial_Porcentaje.insert(i, new QDial);
        Dial_Porcentaje[i]->setNotchesVisible(true);
        Dial_Porcentaje[i]->setRange(0,100);
        Progreso_Porcentaje.insert(i, new QProgressBar);
        Progreso_Porcentaje[i]->setRange(0,100);
        Tiempooo.insert(i, new QTimeEdit);
        Tiempooo[i]->setEnabled(false);
        Tiempooo[i]->setDisplayFormat("H:mm:ss");
        cuenta_Regresiva.insert(i, new QLabel);
        cuenta_Regresiva[i]->hide();
        widgetSupremo[i] = new QWidget;
        cartelito_Porcentaje[i] = new QLabel(b.value(i));
        cartelito_Tiempo[i] = new QLabel(c.value(i));
        infoValvulas[i] = new QLabel(a.value(i));
        boxDialProgreso[i] = new QHBoxLayout;
        boxBoxInfo[i] = new QVBoxLayout;
        boxTime[i] = new QVBoxLayout;
        todoAll[i] = new QHBoxLayout;
        betweenFlujo_Reverse[i] = new QVBoxLayout;
        condicion_Activacion.insert(i, new QComboBox);
        condicion_Activacion[i]->addItem("Seleccione");
        condicion_Activacion[i]->addItem("Activacion Manual");
        condicion_Activacion[i]->addItem("Activacion Automatica");
        flujo_Activado.insert(i, new QLabel("Flujo"));
        flujo_Activado[i]->hide();
        condicion_Valvula.insert(i, new QLabel("Valvula desactivada"));
        Stop.insert(i, new QPushButton("Stop"));
        Stop[i]->hide();
        contador.insert(i, new QTimer);
        temporizador.insert(i, new QTimer);
        culpa_Reverse.insert(i, new QTimer);
        connect(Dial_Porcentaje[i], SIGNAL(valueChanged(int)), this, SLOT(aver_q(int)));
        connect(condicion_Activacion[i], SIGNAL(currentIndexChanged(int)), this, SLOT(evaluar_Enable(int)));
        connect(Efectuar[i], SIGNAL(clicked()), this, SLOT(activacion_Manual()));
        connect(Stop[i], SIGNAL(clicked()), this, SLOT(parar_Manual()));
        connect(contador[i], SIGNAL(timeout()), this, SLOT(tomando_Cuenta()));
        connect(Efectuar[i], SIGNAL(clicked()), this, SLOT(activar_Automatico()));
        connect(temporizador[i], SIGNAL(timeout()), this, SLOT(stop_Automatizado()));
        connect(culpa_Reverse[i], SIGNAL(timeout()), this, SLOT(establecer_Cuenta()));
        connect(this, SIGNAL(cambio_ConsumoTiempo()), this, SLOT(Establecer_Resto()));
        boxDialProgreso[i]->addWidget(Dial_Porcentaje[i]);
        boxDialProgreso[i]->addWidget(Progreso_Porcentaje[i]);
        boxBoxInfo[i]->addWidget(cartelito_Porcentaje[i]);
        boxBoxInfo[i]->addLayout(boxDialProgreso[i]);
        boxTime[i]->addWidget(cartelito_Tiempo[i]);
        boxTime[i]->addWidget(Tiempooo[i]);
        todoAll[i]->addWidget(infoValvulas[i]);
        todoAll[i]->addLayout(boxBoxInfo[i]);
        todoAll[i]->addLayout(boxTime[i]);
        todoAll[i]->addWidget(condicion_Activacion[i]);
        todoAll[i]->addWidget(Efectuar[i]);
        todoAll[i]->addWidget(condicion_Valvula[i]);
        todoAll[i]->addWidget(Stop[i]);
        betweenFlujo_Reverse[i]->addWidget(flujo_Activado[i]);
        betweenFlujo_Reverse[i]->addWidget(cuenta_Regresiva[i]);
        todoAll[i]->addLayout(betweenFlujo_Reverse[i]);
        widgetSupremo[i]->setLayout(todoAll[i]);
        organizador_Widget->addWidget(widgetSupremo[i]);

        //////////////////////////////////////////////////////////////////////
    }
    final->setLayout(organizador_Widget);
    contenido->setWidget(final);
    ui->contenedor_Contenido->addWidget(contenido);
    dataBase_Valvulas->close();
    cValvulas=0;
}

void Controladores::aver_q(int a)
{
    int key = Dial_Porcentaje.key((QDial *)(sender()));//Esto se encuentra en todo el algoritmo y me permite poder determinar que elemento está mandando la señal para poder tener una referecia de que tira de válvula tiene que hacer sus modificaciones
    Progreso_Porcentaje.value(key)->setValue(a);
    if(!flujo_Activado.value(key)->isHidden()) //Significa si la valvula se encuentra activada pues el label tomado como referencia solo se muestra cuando se encuentra activada la valvula
    {
        consumo_en_Tiempo-=consumo_Parcial.value(key);//Variación del flujo total que se está consumiendo en tiempo real (acá le estoy restando el valor antiguo de la valvula que está siendo su flujo modificado)
        emit cambio_ConsumoTiempo(); //Cambio del fluo total señal que lo señala
        consumo_Parcial[key]=(double)max_Flujo.value(key)*a/100;
        consumo_en_Tiempo+=consumo_Parcial.value(key);//Variación del flujo total que se está consumiendo en tiempo real (acá le estoy sumando el nuevo valor de la valvula que está siendo su flujo modificado)
        emit cambio_ConsumoTiempo();
        flujo_Activado.value(key)->setText("Flujo activado: "+QString ("%1").arg((consumo_Parcial.value(key)))+abc);
    }
    if(!Stop.value(key)->isHidden())
    {
        if(a==0)
        {
            parar_Dial(key);
        }
    }
}

void Controladores::evaluar_Enable(int a)
{
    int key = condicion_Activacion.key((QComboBox *)(sender()));
    if(a==2)
    {
        Tiempooo.value(key)->setEnabled(true);
    }
    else
        Tiempooo.value(key)->setEnabled(false);
}

void Controladores::activacion_Manual()
{
    int key = Efectuar.key((QPushButton *)(sender()));
    if(condicion_Activacion.value(key)->currentIndex()==1)
    {
        if(Dial_Porcentaje.value(key)->value()!=0)
        {
            consumo_Parcial[key]=(double)max_Flujo.value(key)*Dial_Porcentaje.value(key)->value()/100; //Acá le estoy asignando un valor al flujo que tiene la valvula que se está activando
            consumo_en_Tiempo+=consumo_Parcial.value(key);//Variación del flujo total que se está consumiendo en tiempo real
            contador.value(key)->start(1000);
            emit cambio_ConsumoTiempo();
            flujo_Activado.value(key)->setText("Flujo activado: "+QString ("%1").arg((consumo_Parcial.value(key)))+abc);
            flujo_Activado.value(key)->show();
            condicion_Valvula.value(key)->setText("Valvula activada");
            Stop.value(key)->show();
            Efectuar.value(key)->setEnabled(false);
            condicion_Activacion.value(key)->setEnabled(false);
        }
    }
}

void Controladores::parar_Manual()
{
    int key = Stop.key((QPushButton *)(sender()));
    flujo_Activado.value(key)->setText("");
    flujo_Activado.value(key)->hide();
    condicion_Valvula.value(key)->setText("Valvula desactivada");
    Stop.value(key)->hide();
    Efectuar.value(key)->setEnabled(true);
    consumo_en_Tiempo-=consumo_Parcial.value(key);
    contador.value(key)->stop();
    emit cambio_ConsumoTiempo();
    consumo_Parcial[key]=0;
    Dial_Porcentaje.value(key)->setValue(0);
    Progreso_Porcentaje.value(key)->setValue(0);
    condicion_Activacion.value(key)->setEnabled(true);
}

void Controladores::parar_Dial(int &key)
{
    flujo_Activado.value(key)->setText("");
    flujo_Activado.value(key)->hide();
    condicion_Valvula.value(key)->setText("Valvula desactivada");
    Stop.value(key)->hide();
    Efectuar.value(key)->setEnabled(true);
    consumo_en_Tiempo-=consumo_Parcial.value(key);
    contador.value(key)->stop();
    emit cambio_ConsumoTiempo();
    consumo_Parcial[key]=0;
    Dial_Porcentaje.value(key)->setValue(0);
    Progreso_Porcentaje.value(key)->setValue(0);
    condicion_Activacion.value(key)->setEnabled(true);
}

void Controladores::tomando_Cuenta()
{
    int key = contador.key((QTimer *)(sender()));
    total_M3Consumidos += ((double)(max_Flujo.value(key))*(Dial_Porcentaje.value(key)->value())/100);
    ui->consumido_Parcial->setText(QString ("%1").arg((double)total_M3Consumidos));
    ui->consumido_Total->setValue((double)total_M3Consumidos);
}

void Controladores::activar_Automatico()
{
    int key = Efectuar.key((QPushButton *)(sender()));
    if(condicion_Activacion.value(key)->currentIndex()==2)
    {
        if(Dial_Porcentaje.value(key)->value()!=0)
        {
            QTime tempor;//Esta variable a copiar el tiempo en que se quiere activar la valvula automáticamente
            tempor=Tiempooo.value(key)->time();
            if(tempor!=*consumo_automatizado)
            {    
                tempo.insert(key, consumo_automatizado->secsTo(tempor));
                consumo_Parcial[key]=(double)max_Flujo.value(key)*Dial_Porcentaje.value(key)->value()/100;
                consumo_en_Tiempo+=consumo_Parcial.value(key);
                contador.value(key)->start(1000);
                culpa_Reverse.value(key)->start(1000);
                temporizador.value(key)->start(tempo.value(key)*1000);
                emit cambio_ConsumoTiempo();
                flujo_Activado.value(key)->setText("Flujo activado: "+(QString ("%1").arg((consumo_Parcial.value(key))))+ abc);
                flujo_Activado.value(key)->show();
                condicion_Valvula.value(key)->setText("Valvula activada");
                Efectuar.value(key)->setEnabled(false);
                Tiempooo.value(key)->setEnabled(false);
                condicion_Activacion.value(key)->setEnabled(false);
            }
        }
    }
}

void Controladores::stop_Automatizado()
{
    int key = temporizador.key((QTimer *)(sender()));
    flujo_Activado.value(key)->setText("");
    flujo_Activado.value(key)->hide();
    condicion_Valvula.value(key)->setText("Valvula desactivada");
    Efectuar.value(key)->setEnabled(true);
    Tiempooo.value(key)->setEnabled(true);
    Tiempooo.value(key)->setTime(*consumo_automatizado);
    Dial_Porcentaje.value(key)->setValue(0);
    Progreso_Porcentaje.value(key)->setValue(0);
    consumo_en_Tiempo-=consumo_Parcial.value(key);
    contador.value(key)->stop();
    temporizador.value(key)->stop();
    culpa_Reverse.value(key)->stop();
    emit cambio_ConsumoTiempo();
    consumo_Parcial[key]=0;
    cuenta_Regresiva.value(key)->setText("");
    cuenta_Regresiva.value(key)->hide();
    tempo[key]=0;
    condicion_Activacion.value(key)->setEnabled(true);

    ////////////////////////////////////////////////////////
    //Acá estoy trabajando con la alarma sonora; primero le establesco una ruta predeterminada y posteriormente inicializao la variable de sonido a la ruta escrita y se reproduce
    QString ruta = QString ("%1/Sound/11 pista 11.wav").arg(QDir::currentPath());
    ruta.replace("/","//");
    sound_Alarma.insert(key, new QSound(ruta));
    sound_Alarma[key]->play();
    ////////////////////////////////////////////////////////////

    timer_Alarma.insert(key, new QTimer);
    timer_Alarma[key]->start(4500);
    timer_Parpadeo.insert(key, new QTimer);
    timer_Parpadeo[key]->start(500);

    connect(timer_Parpadeo[key], SIGNAL(timeout()), this, SLOT(parpadea_CFlujo()));
    connect(timer_Alarma[key], SIGNAL(timeout()), this, SLOT(parar_Alarma()));

}

void Controladores::parpadea_CFlujo()
{
    int key = timer_Parpadeo.key((QTimer *)(sender ()));
    ++ah;
    if(ah%2==0)
    {
        condicion_Valvula.value(key)->setText("_._._._._._._._._._");
    }
    else
        condicion_Valvula.value(key)->setText("Valvula desactivada");
}

void Controladores::parar_Alarma()
{
    int key = timer_Alarma.key((QTimer *)(sender()));
    timer_Alarma.value(key)->stop();
    condicion_Valvula.value(key)->setText("Valvula desactivada");
    sound_Alarma.value(key)->stop();
    timer_Parpadeo.value(key)->stop();
}

void Controladores::emitir_senha()
{
    emit senha();
}

void Controladores::establecer_Cuenta()
{
    int key = culpa_Reverse.key((QTimer *)(sender()));
    QTime j;
    j.setHMS((int)(tempo.value(key)/3600), (int)((tempo.value(key)%3600)/60), (int)((tempo.value(key)%36000)%60));
    cuenta_Regresiva.value(key)->setText("La valvula parara en: "+ j.toString("hh:mm:ss"));
    cuenta_Regresiva.value(key)->show();
    int temporary=tempo.value(key)-1;
    tempo.insert(key, temporary);
}

void Controladores::Establecer_Horarios()
{
    if(segundos_restantes_Real==86401)
    {
        ui->icono->hide();
        ui->icono_2->hide();
        ui->tiempo_REAL->hide();
    }
    else
    {
        hora_Actualizada=QTime::currentTime(); //Aca tomo el tiempo de la máquina
        int hora_Actual=hora_Actualizada.hour();
        int minuto_Actual=hora_Actualizada.minute();
        int segundos_Actual=hora_Actualizada.second();
        int segundos_SiguienteDia = 86400; //Total de segundos que tienen un día
        segundos_Actual+=60*(hora_Actual*60+minuto_Actual); //Acá determino el total de segundos que ha pasado en el día (tiempo real)
        segundos_restantes_Total=segundos_SiguienteDia-segundos_Actual;
        if(segundos_restantes_Real<segundos_restantes_Total)
        {
            //////////////////////////////////////
            //Aca estoy dandole formato al tiempo en que se va a gastar el consumo diario establecido
            int hora, minuto, segundos;
            hora=(int)segundos_restantes_Real/3600;
            minuto=(int)(segundos_restantes_Real%3600)/60;
            segundos=(segundos_restantes_Real%3600)%60;
            QString h, m, s;
            h=(QString ("%1").arg(hora));
            if(minuto<10)
            {
                m=("0"+QString("%1").arg(minuto));
            }
            else
                m=(QString ("%1").arg(minuto));
            if(segundos<10)
            {
                s=("0"+QString("%1").arg(segundos));
            }
            else
                s=(QString ("%1").arg(segundos));

            ui->tiempo_REAL->setText("A este ritmo la empresa\nla empresa va a consumir el agua asignada en:\n"+h+":"+m+":"+s);
            ui->tiempo_REAL->show();
            ui->icono->show();
            ui->icono_2->show();
        }
        else
        {
            ui->icono->hide();
            ui->icono_2->hide();
            ui->tiempo_REAL->hide();
        }
    }
    if(segundos_restantes_Real>0)
    {
            segundos_restantes_Real--;
    }
}

void Controladores::Establecer_Resto()
{
    segundos_restantes_Real=86401;
    if(consumo_en_Tiempo!=0)
    {
       segundos_restantes_Real=(double)((limite_Diario-ui->consumido_Total->value())/consumo_en_Tiempo);
    }
    else
    {
        segundos_restantes_Real=86401;
    }
}

void Controladores::inspeccion()
{
    if(total_M3Consumidos>=ui->consumido_Total->maximum())
    {
        cerrar_Panel();
        ui->consumido_Parcial->setText(QString ("%1").arg(limite_Diario));
        ui->consumido_Total->setValue(limite_Diario);
        contenido->setEnabled(false);
        QMessageBox::warning(this, "Atencion", "Ha sobrepasado el consumo diario\npor lo que se desabilitira el panel de control", QMessageBox::Ok);
    }
}
void Controladores::cerrar_Panel()
{
    empresa_Acueducto->stop();
    calculo_MinutosRestantes->stop();
    ui->icono->hide();
    ui->icono_2->hide();
    ui->tiempo_REAL->clear();
    ui->tiempo_REAL->hide();
    ah=0;

    //Es necesario usar iterator debido a que los QHash no trabajan igual que los arreglos y con el iterator se tiene la posibilidad de poder recorrer un QHash entero
    QHash <int, QTimer *>::iterator it;
    for(it=contador.begin(); it != contador.end(); ++it)
    {
       contador.value(it.key())->stop();
    }
    for(it=temporizador.begin(); it!=temporizador.end(); ++it)
    {
        temporizador.value(it.key())->stop();
    }
    for(it=culpa_Reverse.begin(); it!=culpa_Reverse.end(); ++it)
    {
        culpa_Reverse.value(it.key())->stop();
    }
    for(it=timer_Alarma.begin(); it!=timer_Alarma.end(); ++it)
    {
        timer_Alarma.value(it.key())->stop();
    }
    for(it=timer_Parpadeo.begin(); it!=timer_Parpadeo.end(); ++it)
    {
        timer_Parpadeo.value(it.key())->stop();
    }
    QHash <int, QDial*>::iterator it1;
    for(it1=Dial_Porcentaje.begin(); it1!=Dial_Porcentaje.end(); ++it1)
    {
        Dial_Porcentaje[it1.key()]->setValue(0);;
    }
    QHash <int, QProgressBar *>::iterator it2;
    for(it2=Progreso_Porcentaje.begin(); it2!=Progreso_Porcentaje.end(); ++it2)
    {
        Progreso_Porcentaje.value(it2.key())->setValue(0);
    }
    QHash <int, QTimeEdit *>::iterator it3;
    for(it3=Tiempooo.begin(); it3!=Tiempooo.end(); ++it3)
    {
        Tiempooo.value(it3.key())->setTime(*consumo_automatizado);
    }
    QHash <int, QComboBox *>::iterator it4;
    for(it4=condicion_Activacion.begin(); it4!=condicion_Activacion.end(); ++it4)
    {
        condicion_Activacion.value(it4.key())->setCurrentIndex(0);
    }
    QHash <int, QPushButton *>::iterator it5;
    for(it5=Stop.begin(); it5!=Stop.end(); ++it5)
    {
        Stop.value(it5.key())->hide();
    }
    QHash <int, QLabel *>::iterator it6;
    for(it6=cuenta_Regresiva.begin(); it6!=cuenta_Regresiva.end(); ++it6)
    {
        cuenta_Regresiva.value(it6.key())->setText("");
        cuenta_Regresiva.value(it6.key())->hide();
    }
    for(it6=flujo_Activado.begin(); it6!=flujo_Activado.end(); ++it6)
    {
        flujo_Activado.value(it6.key())->setText("");
        flujo_Activado.value(it6.key())->hide();

    }
    for(it6=condicion_Valvula.begin(); it6!=condicion_Valvula.end(); ++it6)
    {
        condicion_Valvula.value(it6.key())->setText("Valvula desactivada");
    }
    QHash <int, int >::iterator it7;
    for(it7=tempo.begin(); it7!=tempo.end(); ++it7)
    {
        tempo[it7.key()]=0;
    }
    QHash <int, double>::iterator it8;
    for(it8=consumo_Parcial.begin(); it8!=consumo_Parcial.end(); ++it8)
    {
        consumo_Parcial[it8.key()]=0;
    }
}
