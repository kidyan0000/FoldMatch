#include "clothwindow.h"
#include "ply_Module.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClothWindow w;
    w.show();
    
    // read ply Module
    ply_module* _plyModuleT = new ply_module();
    ply_module* _plyModuleR = new ply_module();
    _plyModuleT->readPLY("../data/elephant.ply", true, true, true, false, true);
    _plyModuleR->readPLY("../data/elephant.ply", true, true, true, false, true);

    return a.exec();
}
