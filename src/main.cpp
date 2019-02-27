#include "cloth_window.h"
// #include "cloth_glwidget.h"
#include "cloth_calc.h"
#include "cloth_control.h"

#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <dirent.h>


#include <sys/types.h>
#include <sys/stat.h>

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

    int CT;   // cloth template
    int CR;   // cloth reference
    int BS;   // cloth base
    int FILE; // file name

    // create the folder
    std::string dir = "../output/debug/readme";
    if (access(dir.c_str(), 0) == -1)
    {
        mkdir(dir.c_str(), 0777);
    }

    ////////////////////////////////
    ///// START THE SIMULATION /////
    ////////////////////////////////

    for(CR=1; CR<75; CR++)
    {
        CT = 0;
        BS = 0;
        FILE = CR-1;

        cloth_calc* cloth = new cloth_calc(control->GetInput(CT) , control->GetInput(CR), control->GetInput(BS));

        // cloth -> cloth_eig_neighbor();
        // Eigen::MatrixXd val = cloth->GetEigval_neighbor();

        cloth -> cloth_eig_neighbor2x();
        Eigen::MatrixXd val = cloth->GetEigval_neighbor2x();

        cloth -> cloth_vec_normalize(val, 3);

        cloth -> cloth_WriteColor(cloth->GetEigval_norm_dir3(), control->GetOutput(FILE));

        std::ofstream outfile(control->Readme(FILE));
        outfile << "Template is: "  << control->GetInput(CT) << std::endl;
        outfile << "Reference is: " << control->GetInput(CR) << std::endl;
        outfile << "Base is: "      << control->GetInput(BS) << std::endl;
        outfile << "Lambda is: "    << control->GetLambda() << std::endl;
        outfile.close();

        ///////////////////////////////
        ////// THIS IS FOR DEBUG //////
        ///////////////////////////////

        // std::cout << control->Readme(i) << std::endl;
        // std::ofstream Test("../output/Test.txt");
        // Test<< val << std::endl;
        // Test.close();
    }


    w.show();

    return a.exec();


}
