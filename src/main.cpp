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
    _plyModule->readPLY("../data/Reference-1180330141717.ply", true, true, true, true, true);

    Eigen::MatrixXd verts, normals;
    Eigen::MatrixXi colors;
    verts = _plyModule->getVertices();
    normals = _plyModule->getNormals();
    colors = _plyModule->getColors();

    // change matrix to row-based vector
    Eigen::MatrixXd verts_t = verts.transpose();
    Eigen::VectorXd verts_row = Eigen::Map<Eigen::VectorXd>(verts_t.data(), verts_t.size());
    Eigen::MatrixXd normals_t = normals.transpose();
    Eigen::VectorXd normals_row = Eigen::Map<Eigen::VectorXd>(normals_t.data(), normals_t.size());
    Eigen::MatrixXi colors_t = colors.transpose();
    Eigen::VectorXi colors_row = Eigen::Map<Eigen::VectorXi>(colors_t.data(),colors_t.size());

    /*
     * use method resize
     * verts.resize(1,verts.size());
    */


    // debug
    std::cout << colors_row << std::endl;

    return a.exec();


}
