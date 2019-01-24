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

    test->cloth_calc_norm(val, 2);
    test->test();
    // test->GetDefo();
    // test->GetDispl();
    Eigen::MatrixXd val_norm = test->GetEig_norm();

    test->cloth_calc_Color(val_norm, 2);
    test->cloth_WriteColor(test->GetColor_vert1(), "../output/debug/Color_vert1.ply");
    test->cloth_WriteColor(test->GetColor_vert2(), "../output/debug/Color_vert2.ply");
    test->cloth_WriteColor(test->GetColor_vert3(), "../output/debug/Color_vert3.ply");

    // this is for debug
    // std::cout << test->GetEig_norm() << std::endl;
    /*
    std::ofstream outfile1("../output/debug/Eigen_norm.txt");
    outfile1<< test->GetEig_norm() <<std::endl;
    outfile1.close();
    std::ofstream outfile("../output/debug/Color_vec1.txt");
    outfile<< test->GetColor_vert1() <<std::endl;
    outfile.close();
    std::ofstream outfile2("../output/debug/Color_vec2.txt");
    outfile2<< test->GetColor_vert2() <<std::endl;
    outfile2.close();
    std::ofstream outfile3("../output/debug/Color_vec3.txt");
    outfile3<< test->GetColor_vert3() <<std::endl;
    outfile3.close();
    */
    // test->test();

    w.show();

    return a.exec();


}
