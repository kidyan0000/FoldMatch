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
    // test->cloth_vec();
    test->cloth_eig();
    Eigen::MatrixXd val = test->GetEigval();
    // Eigen::MatrixXd vec = test->GetEigvec();
    // test->cloth_defo();

    test->cloth_vec_normalize(val, 2);

    // test->GetDefo();
    // test->GetDispl();
    // Eigen::MatrixXd val_norm = test->GetEigval_norm_direc1();

    // this is for debug
    // std::cout << test->GetEig_norm() << std::endl;

    // std::ofstream outfile1("../output/debug/test.txt");
    // outfile1<< test->GetEigval_norm_dir1() <<std::endl;
    // outfile1.close();
    std::ofstream outfile("../output/debug/GetEigval_norm_direc1.txt");
    outfile<< test->GetEigval_norm_dir1() <<std::endl;
    outfile.close();
    std::ofstream outfile3("../output/debug/GetEigval_norm_direc2.txt");
    outfile3<< test->GetEigval_norm_dir2() <<std::endl;
    outfile3.close();
    // std::ofstream outfile3("../output/debug/Color_vec3.txt");
    // outfile3<< test->GetColor_vert3() <<std::endl;
    // outfile3.close();

    test->test();

    w.show();

    return a.exec();


}
