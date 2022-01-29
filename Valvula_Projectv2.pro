#-------------------------------------------------
#
# Project created by QtCreator 2016-12-29T11:48:14
#
#-------------------------------------------------

QT       += core gui sql multimedia

TARGET = Valvula_Projectv2
TEMPLATE = app


SOURCES += main.cpp\
        contrase_a.cpp \
    controladores.cpp \
    agricultores.cpp \
    base_datos.cpp \
    panel_opciones.cpp \
    interfaz_adiccion.cpp \
    entrada_serial_modificar.cpp \
    interfaz_modificacion.cpp

HEADERS  += contrase_a.h \
    controladores.h \
    agricultores.h \
    base_datos.h \
    panel_opciones.h \
    interfaz_adiccion.h \
    entrada_serial_modificar.h \
    interfaz_modificacion.h

FORMS    += contrase_a.ui \
    controladores.ui \
    agricultores.ui \
    base_datos.ui \
    panel_opciones.ui \
    interfaz_adiccion.ui \
    entrada_serial_modificar.ui \
    interfaz_modificacion.ui

RESOURCES += \
    Imagenes.qrc
