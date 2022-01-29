#ifndef CONTROLADORES_H
#define CONTROLADORES_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QPushButton>
#include <QProgressBar>
#include <QDial>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QListWidget>
#include <QTimeEdit>
#include <QWidget>
#include <QScrollArea>
#include <QComboBox>
#include <QStateMachine>
#include <QHash>
#include <QTimer>
#include <QInputDialog>
#include <QMessageBox>
#include <QSound>
#include <QDir>

namespace Ui {
    class Controladores;
}

class Controladores : public QMainWindow
{
    Q_OBJECT

public:
    explicit Controladores(QWidget *parent = 0);
    ~Controladores();
    void Cargar_BD(QSqlDatabase &); //Funci�n encargada de cargar la base de datos
    void Cargar_Panel();//Funci�n utilizada para cargar el panel de control tomando en cuenta los elementos existentes en la base de datos
    void Cargar_Limite();//Funci�n encargada de cargar el consumo diario establecido como l�mite

private:
    Ui::Controladores *ui;
    QTime hora_Actualizada;
    QSqlDatabase *dataBase_Valvulas;//Variable que contiene la base de datos con la cual se va a trabajar

    ///////////////////////////////////////////////////////////////////////////
    //Elementos visuales del panel de control
    //Ac� es importante poner todos los elementos en QHash y Arreglos para no perder la referencia de los mismos (los que se encuentran creados din�micamente) para poder efectuar todos los cambios necesarios en el panel
    QScrollArea *contenido;
    QWidget *final;
    QHash <int, QPushButton *>Efectuar;
    QHash <int, QDial *> Dial_Porcentaje;
    QHash <int, QProgressBar *> Progreso_Porcentaje;
    QHash <int, QTimeEdit *> Tiempooo;
    QHash <int, QComboBox *>condicion_Activacion;
    QHash <int, QLabel *> flujo_Activado;
    QHash <int, QLabel *> condicion_Valvula;
    QHash <int, QPushButton *> Stop;
    QHash <int, QLabel *> cuenta_Regresiva;
    QWidget *widgetSupremo[0];
    QLabel *cartelito_Porcentaje[0];
    QLabel *cartelito_Tiempo[0];
    QLabel *infoValvulas[0];
    QHBoxLayout *boxDialProgreso[0];
    QVBoxLayout *boxBoxInfo[0];
    QVBoxLayout *boxTime[0];
    QHBoxLayout *todoAll[0];
    QVBoxLayout *betweenFlujo_Reverse[0];
    //////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////
    //Variables internas utilizadas en la elaboraci�n del algoritmo de controladores

    QString abc; //String para poner la unidad de medida en la interfaz
    QStringList a; //String utilizada para poner todo lo referente a la informaci�n de la v�lvula escanesda de la base de datos

    //Strings para establecer el comentario de los label cartelito_Porcentaje y cartelito_Tiempo
    QStringList b;
    QStringList c;
    ///////////////////////////////////////////////////////

    int cValvulas;//Variable para establecer la cantidad de V�lvulas existentes en la base de datos
    int ah; //Variable utilizada para el parpadeo cuando la v�lvula se cierra autom�ticamente
    double total_M3Consumidos; //Variable utilizada para guardar la cantidad de metros c�bicos consumidos por la empresa
    double limite_Diario; //Variable encargada de guardar el limite diario permitido para la empresa
    double consumo_en_Tiempo; //Variable utilizada para calcular el flujo de agua que se est� consumiendo en tiempo real (ser�a una sumatoria de las atribuciones de todas las v�lvulas y en el c�digo est� implementada para modificarse cuando ocurre una modificaci�n en el flujo total)
    QHash <int, int> max_Flujo; //Variable utilizada para guardar el flujo m�ximo de cada una de las v�lvulas de la base de datos
    QHash <int, double> consumo_Parcial; //Variable utilizada para guardar el consumo de agua (metros c�bicos) que est� teniendo la v�lvula (se puede variar e influye directamente a la variable consumo_en_Tiempo)
    QHash <int, int > tempo; //Variable utilizada para guardar la cantidad de segundos que le falta a la v�lvula para que se desactive (solo en modo autom�tico)
    int segundos_restantes_Total; //Segundos que faltan para que se acbe el d�a real
    int segundos_restantes_Real; //Segundos que faltan para que en tiempo real y al flujo correspondiente a ese tiempo se acabe el consumo diario establecido

    /////////////////////////////////////////////////////////////////////////////
    //Qtimers utilizados en la elaboraci�n del algoritmo

    QHash <int, QTimer *> contador; //Utilizado para contar la cantidad de agua que se ha consumido
    QHash <int, QTimer *> temporizador; //Utilizado para hacer la para autom�tica de la v�lvula (solo en modo autom�tico)
    QTimer *calculo_MinutosRestantes; //Utilizado para determinar si a el ritmo de consumo existente en la empresa el consumo diario se puede agotar antes de lo previsto
    QTimer *empresa_Acueducto; //Utilizado para detreminar si el consumo diario establecido ya se ha agotado
    QHash <int, QTimer *> timer_Alarma; //Utilizado para parar la alarma del paro autom�tico de una valvula (solo en modo autom�tico)
    QHash <int, QTimer *> timer_Parpadeo; //Utilizado para hacer la alarma visual del paro autom�tico de una v�lvula (solo en modo autom�tico)
    QHash <int, QTimer *> culpa_Reverse; //Utilizado para hacer la cuenta regresiva del funcionamiento de la v�lvula activada en modo autom�tico

    ///////////////////////////////////////////////////////////////////////////

    QHash <int, QSound *> sound_Alarma; //Variable para inicializar el sonido que se va a utilizar para realizar la alarama sonora del paro autom�tico de una v�lvula de forma autom�tica
    QTime *consumo_automatizado; //Es un QTime inicializado al inicio de un d�a

private slots:
    void on_salida_clicked();
    void aver_q(int); //Slot utilizada para realizar algunas modificaciones en el flujo de la valvula cuando se var�a el valor de del dial
    void evaluar_Enable(int a); //Slot encargada para determinar el modo de activaci�n de la v�lvula
    void activacion_Manual(); //Slot para activar manualmente la v�lvula
    void parar_Manual(); //Slot para parar manualmente la v�lvula
    void tomando_Cuenta(); //Slot para contar la cantidad de agua en metros c�bicos que se ha consumido en tiempo real
    void activar_Automatico(); //Slot para activar autom�ticamente la v�lvula
    void stop_Automatizado(); //Slot para parar autom�ticamente la v�lvula
    void emitir_senha(); //Slot para emitir la se�al para dar a conocer a la interfaz de contrase�a que el panel de control est� a punto de cerrarse
    void establecer_Cuenta(); //Slot para inicializar la cuenta regresiva para en la activaci�n autom�tica
    void Establecer_Horarios(); //Slot para trabajar con los segundos que faltan para que se acabe el d�a y la comparaci�n con los que faltan para que se acabe el consumo diario establecido, este determina si mostrar el aviso o no del posible agotamiento
    void Establecer_Resto(); //Slot para establecer cuantos segundos faltan para que se acabe el consumo diarioestablecido con respecto al consumo existente en tiempo real
    void inspeccion();//Slot para determinar si el consumo diario establecido ya se ha agotado
    void cerrar_Panel();//Slot para establecer las condiciones para cerrar el panel de control
    void parar_Alarma();//Slot para parar la alarma impuesta cuando una v�lvula se desactiva autom�ticamente
    void parpadea_CFlujo();//Slot para hacer la alarma visual cuando la v�lvula se desactiva autom�ticamente
    void parar_Dial(int &);//Slot para si la v�lvula est� activada manualmente y el dial obtiene el valor de 0 entonces la v�lvula se desactiva

signals:
    void senha(); //Se�al para preparar la interfaz de contrase�as
    void cambio_ConsumoTiempo();//Se�al para dar a conocer alguna modificaci�n del flujo resultante entre el flujo hecho por todas las v�lvulas (afectando as� el tiempo para que se termine el consumo diario establecido)
};

#endif // CONTROLADORES_H
