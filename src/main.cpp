#include "cloth_window.h"
#include "cloth_glwidget.h"

// #include "ply_Module.h"
#include "ply_defo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cloth_Window w;

    // w.resize(800, 600);
    w.show();

    return a.exec();
}
