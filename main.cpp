#include <QApplication>
#include "contrase_a.h"
#include <QSound>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Contrase_a w;
    w.show();
    return a.exec();
}
