#include "about.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    about w;
    w.show();

    return a.exec();
}
