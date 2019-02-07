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

    control -> cloth_lambda("lambda3.ply");
    control -> cloth_input("../data/");
    control -> cloth_output("../output/debug/regulation-");

    // the input and output list can be found in the output folder
    // from 0 to 73
    int i=73;

    ////////////////////////////////
    /// NOW START THE SIMULATION ///
    ////////////////////////////////

    cloth_calc* cloth = new cloth_calc(control->GetInput(i) , control->GetInput(i+1));

    cloth -> cloth_eig_neighbor();
    Eigen::MatrixXd val = cloth->GetEigval_neighbor();
    cloth -> cloth_vec_normalize(val, 3);

    cloth -> cloth_WriteColor(cloth->GetEigval_norm_dir3(), control->GetOutput(i));

    std::ofstream outfile(control->Readme(i));
    outfile << "Template is: " << control->GetInput(i) << std::endl;
    outfile << "Reference is: " << control->GetInput(i+1) << std::endl;
    outfile.close();

    ///////////////////////////////
    ////// THIS IS FOR DEBUG //////
    ///////////////////////////////

    // std::cout << control->GetInput(0) << std::endl;
    // std::ofstream outfile("../output/Test.txt");
    // outfile<< Eigval_norm_dir1 << std::endl;
    // outfile.close();

    w.show();

    return a.exec();


}
