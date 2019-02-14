#include "cloth_window.h"
// #include "cloth_glwidget.h"
#include "cloth_calc.h"
#include "cloth_control.h"

#include <stdio.h>

#include <QTextStream>
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

    control -> cloth_lambda("lambda3");

    control -> cloth_input("../data/");
    control -> cloth_output("../output/debug/");

    // the input and output list can be found in the output folder
    // the label of our cloth is from 1-0001 to 75-0075

    int CT=0;
    int CR=74;

    int FILE=CR-1;

    ////////////////////////////////
    ///// START THE SIMULATION /////
    ////////////////////////////////

    cloth_calc* cloth = new cloth_calc(control->GetInput(CT) , control->GetInput(CR));

    cloth -> cloth_eig_neighbor();
    Eigen::MatrixXd val = cloth->GetEigval_neighbor();
    cloth -> cloth_vec_normalize(val, 3);

    cloth -> cloth_WriteColor(cloth->GetEigval_norm_dir3(), control->GetOutput(FILE));

    std::ofstream outfile(control->Readme(FILE));
    outfile << "Template is: "  << control->GetInput(CT) << std::endl;
    outfile << "Reference is: " << control->GetInput(CR) << std::endl;
    outfile.close();

    ///////////////////////////////
    ////// THIS IS FOR DEBUG //////
    ///////////////////////////////

    // std::cout << control->Readme(i) << std::endl;
    // std::ofstream Test("../output/val40.txt");
    // Test<< val  << std::endl;
    // Test.close();

    w.show();

    return a.exec();


}
