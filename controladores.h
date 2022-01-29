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
    void Cargar_BD(QSqlDatabase &); //Función encargada de cargar la base de datos
    void Cargar_Panel();//Función utilizada para cargar el panel de control tomando en cuenta los elementos existentes en la base de datos
    void Cargar_Limite();//Función encargada de cargar el consumo diario establecido como límite

private:
    Ui::Controladores *ui;
    QTime hora_Actualizada;
    QSqlDatabase *dataBase_Valvulas;//Variable que contiene la base de datos con la cual se va a trabajar

    ///////////////////////////////////////////////////////////////////////////
    //Elementos visuales del panel de control
    //Acá es importante poner todos los elementos en QHash y Arreglos para no perder la referencia de los mismos (los que se encuentran creados dinámicamente) para poder efectuar todos los cambios necesarios en el panel
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
    //Variables internas utilizadas en la elaboración del algoritmo de controladores

    QString abc; //String para poner la unidad de medida en la interfaz
    QStringList a; //String utilizada para poner todo lo referente a la información de la válvula escanesda de la base de datos

    //Strings para establecer el comentario de los label cartelito_Porcentaje y cartelito_Tiempo
    QStringList b;
    QStringList c;
    ///////////////////////////////////////////////////////

    int cValvulas;//Variable para establecer la cantidad de Válvulas existentes en la base de datos
    int ah; //Variable utilizada para el parpadeo cuando la válvula se cierra automáticamente
    double total_M3Consumidos; //Variable utilizada para guardar la cantidad de metros cúbicos consumidos por la empresa
    double limite_Diario; //Variable encargada de guardar el limite diario permitido para la empresa
    double consumo_en_Tiempo; //Variable utilizada para calcular el flujo de agua que se está consumiendo en tiempo real (sería una sumatoria de las atribuciones de todas las válvulas y en el código está implementada para modificarse cuando ocurre una modificación en el flujo total)
    QHash <int, int> max_Flujo; //Variable utilizada para guardar el flujo máximo de cada una de las válvulas de la base de datos
    QHash <int, double> consumo_Parcial; //Variable utilizada para guardar el consumo de agua (metros cúbicos) que está teniendo la válvula (se puede variar e influye directamente a la variable consumo_en_Tiempo)
    QHash <int, int > tempo; //Variable utilizada para guardar la cantidad de segundos que le falta a la válvula para que se desactive (solo en modo automático)
    int segundos_restantes_Total; //Segundos que faltan para que se acbe el día real
    int segundos_restantes_Real; //Segundos que faltan para que en tiempo real y al flujo correspondiente a ese tiempo se acabe el consumo diario establecido

    /////////////////////////////////////////////////////////////////////////////
    //Qtimers utilizados en la elaboración del algoritmo

    QHash <int, QTimer *> contador; //Utilizado para contar la cantidad de agua que se ha consumido
    QHash <int, QTimer *> temporizador; //Utilizado para hacer la para automática de la válvula (solo en modo automático)
    QTimer *calculo_MinutosRestantes; //Utilizado para determinar si a el ritmo de consumo existente en la empresa el consumo diario se puede agotar antes de lo previsto
    QTimer *empresa_Acueducto; //Utilizado para detreminar si el consumo diario establecido ya se ha agotado
    QHash <int, QTimer *> timer_Alarma; //Utilizado para parar la alarma del paro automático de una valvula (solo en modo automático)
    QHash <int, QTimer *> timer_Parpadeo; //Utilizado para hacer la alarma visual del paro automático de una válvula (solo en modo automático)
    QHash <int, QTimer *> culpa_Reverse; //Utilizado para hacer la cuenta regresiva del funcionamiento de la válvula activada en modo automático

    ///////////////////////////////////////////////////////////////////////////

    QHash <int, QSound *> sound_Alarma; //Variable para inicializar el sonido que se va a utilizar para realizar la alarama sonora del paro automático de una válvula de forma automática
    QTime *consumo_automatizado; //Es un QTime inicializado al inicio de un día

private slots:
    void on_salida_clicked();
    void aver_q(int); //Slot utilizada para realizar algunas modificaciones en el flujo de la valvula cuando se varía el valor de del dial
    void evaluar_Enable(int a); //Slot encargada para determinar el modo de activación de la válvula
    void activacion_Manual(); //Slot para activar manualmente la válvula
    void parar_Manual(); //Slot para parar manualmente la válvula
    void tomando_Cuenta(); //Slot para contar la cantidad de agua en metros cúbicos que se ha consumido en tiempo real
    void activar_Automatico(); //Slot para activar automáticamente la válvula
    void stop_Automatizado(); //Slot para parar automáticamente la válvula
    void emitir_senha(); //Slot para emitir la señal para dar a conocer a la interfaz de contraseña que el panel de control está a punto de cerrarse
    void establecer_Cuenta(); //Slot para inicializar la cuenta regresiva para en la activación automática
    void Establecer_Horarios(); //Slot para trabajar con los segundos que faltan para que se acabe el día y la comparación con los que faltan para que se acabe el consumo diario establecido, este determina si mostrar el aviso o no del posible agotamiento
    void Establecer_Resto(); //Slot para establecer cuantos segundos faltan para que se acabe el consumo diarioestablecido con respecto al consumo existente en tiempo real
    void inspeccion();//Slot para determinar si el consumo diario establecido ya se ha agotado
    void cerrar_Panel();//Slot para establecer las condiciones para cerrar el panel de control
    void parar_Alarma();//Slot para parar la alarma impuesta cuando una válvula se desactiva automáticamente
    void parpadea_CFlujo();//Slot para hacer la alarma visual cuando la válvula se desactiva automáticamente
    void parar_Dial(int &);//Slot para si la válvula está activada manualmente y el dial obtiene el valor de 0 entonces la válvula se desactiva

signals:
    void senha(); //Señal para preparar la interfaz de contraseñas
    void cambio_ConsumoTiempo();//Señal para dar a conocer alguna modificación del flujo resultante entre el flujo hecho por todas las válvulas (afectando así el tiempo para que se termine el consumo diario establecido)
};

#endif // CONTROLADORES_H
