#include "kurs.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    kurs w;
    w.show();
    w.resize(2000, 1300);
    w.setWindowTitle("AIS Persons");

    return a.exec();
}
