#include "cloth_window.h"
#include "cloth_glwidget.h"

#include "ply_Module.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // close window does not quit all programm
    // QApplication::setQuitOnLastWindowClosed(false);

    Cloth_Window w;

    w.show();

    // get vertices
    ply_module* _plyModule = new ply_module();
    _plyModule->readPLY("../data/bunny.ply", true, true, true, true, true);

    Eigen::MatrixXd verts;
    verts = _plyModule->getVertices();

    // print the vertices
    std::cout << verts << std::endl;

    // delete _plyModule;

    return a.exec();


}
