#ifndef INTERFAZ_MODIFICACION_H
#define INTERFAZ_MODIFICACION_H

#include <QDialog>

namespace Ui {
    class Interfaz_modificacion;
}

class Interfaz_modificacion : public QDialog
{
    Q_OBJECT

public:
    explicit Interfaz_modificacion(QWidget *parent = 0);
    ~Interfaz_modificacion();

    ///////////////////////////////////////////////
    //Funciones que van a entregarle a la interfaz padre los elementos que se desean modificar envíando un QString no nulo
    QString Scanner_Plantacion ();
    QString Scanner_Serial ();
    QString Sacnner_Flujo ();
    QString Scanner_modelo ();


private:
    Ui::Interfaz_modificacion *ui;

private slots:
    void on_salir_clicked();
    void on_modificar_clicked();
    void emitir_Senha();

signals:
    void Senha();
};

#endif // INTERFAZ_MODIFICACION_H
