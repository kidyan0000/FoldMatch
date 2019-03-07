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

    control -> cloth_lambda("lambda1");
    control -> cloth_input("../data/");
    control -> cloth_output("../output/debug/");

    // the input and output list can be found in the output folder
    // the label of our cloth is from 1-0001 to 75-0075

    int CT;   // cloth template
    int CR;   // cloth reference
    int BS;   // cloth base
    int FILE; // file name

    double deltaT;

    // create the folder
    std::string dir = "../output/debug/readme";
    if (access(dir.c_str(), 0) == -1)
    {
        mkdir(dir.c_str(), 0777);
    }

    ////////////////////////////////
    ///// START THE SIMULATION /////
    ////////////////////////////////

    for(int slot=0; slot<72; slot++)
    {
        CT = slot;
        CR = slot+3;
        BS = slot;
        FILE = slot;
        deltaT = 0.005;


        // cloth_calc* slot_CT = new cloth_calc(control->GetInput(CT-1) , control->GetInput(CR-1), control->GetInput(BS-1));
        cloth_calc* slot_CR = new cloth_calc(control->GetInput(CT) , control->GetInput(CR), control->GetInput(BS));

        /*
        slot_CT -> cloth_eig_neighbor2x();
        slot_CR -> cloth_eig_neighbor2x();

        Eigen::MatrixXd val = slot_CR->GetEigval_neighbor2x();
        slot_CR -> cloth_vec_normalize(val, 3);
        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetOutput(FILE));
        slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
        */

        slot_CR -> cloth_eig_kdTree();
        Eigen::MatrixXd val = slot_CR->GetEigval_neighborKdTree();
        slot_CR -> cloth_vec_normalize(val, 3);
        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetOutput(FILE));

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
        // std::ofstream Test("../output/eigval_kdtree.txt");
        // Test<< slot_CR->GetEigval_neighborKdTree() << std::endl;
        // Test.close();
    }


    w.show();

    return a.exec();


}
