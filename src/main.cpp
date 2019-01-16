#include "cloth_window.h"
// #include "cloth_glwidget.h"
#include "cloth_calc.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // close window does not quit all programm
    // QApplication::setQuitOnLastWindowClosed(false);

    Cloth_Window w;

    cloth_calc* test = new cloth_calc();
    test->cloth_eig();
    Eigen::MatrixXd val = test->GetEigval();
    Eigen::MatrixXd vec = test->GetEigvec();
    test->cloth_defo(val, vec);
    // test->cloth_displ();
    // test->GetDefo();
    // test->GetDispl();

    // this is for debug
    // std::cout << test->GetDefo() << std::endl;
    // std::ofstream outfile0("../doc_discussion/debug/Eigvec_sq.txt");
    // outfile0<< vec <<std::endl;
    // outfile0.close();

    w.show();

    return a.exec();


}
