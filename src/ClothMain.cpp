#include "clothwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClothWindow w;
    w.show();

    return a.exec();
}
