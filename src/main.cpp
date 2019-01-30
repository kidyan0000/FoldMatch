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

    test->cloth_eig_neighbor();
    Eigen::MatrixXd val = test->GetEigval_neighbor();
    test->cloth_vec_normalize(val, 3);

    // std::ofstream outfile("../output/debug/test.txt");
    // outfile<< test->GetEigval_norm_dir3() <<std::endl;
    // outfile.close();

    // test->test();

    w.show();

    return a.exec();


}
