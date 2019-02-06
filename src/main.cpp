#include "cloth_window.h"
// #include "cloth_glwidget.h"
#include "cloth_calc.h"
#include "cloth_control.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // close window does not quit all programm
    // QApplication::setQuitOnLastWindowClosed(false);

    Cloth_Window w;

    //////////////////////////////
    ///// SET THE SIMULATION /////
    //////////////////////////////

    cloth_control *control = new cloth_control;

    control -> cloth_lambda("/lambda1.ply");
    control -> cloth_input("../data/");
    control -> cloth_output("../output/debug/regulation-");

    ////////////////////////////////
    /// NOW START THE SIMULATION ///
    ////////////////////////////////

    cloth_calc* cloth = new cloth_calc(control->GetInput(0) , control->GetInput(2));

    cloth -> cloth_eig_neighbor();
    Eigen::MatrixXd val = cloth->GetEigval_neighbor();
    cloth -> cloth_vec_normalize(val, 3);

     Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<3> >Eigval_norm_dir1(val.data()  ,val.size()/3,1);

    // cloth -> cloth_WriteColor(cloth->GetEigval_norm_dir1(), control->GetOutput(0));

    ///////////////////////////////
    ////// THIS IS FOR DEBUG //////
    ///////////////////////////////

    // std::cout << control->GetInput(0) << std::endl;
    std::ofstream outfile("../output/Test.txt");
    outfile<< Eigval_norm_dir1 << std::endl;
    outfile.close();

    w.show();

    return a.exec();


}
