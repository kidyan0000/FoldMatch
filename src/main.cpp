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
    Eigen::MatrixXd val = test->GetEigvec();
    Eigen::MatrixXd vec = test->GetEigvec();
    // test.cloth_defo(val, vec);
    // test.GetDefo();

    std::cout << val.data() << std::endl;

    w.show();

    return a.exec();


}
