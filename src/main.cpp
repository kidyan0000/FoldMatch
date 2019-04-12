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

    // setting calculation mode
    // MODE 1: Neighbor1x
    // MODE 2: Neighbor2x
    // MODE 3: KD-TREE
    int MODE = 3;

    // setting calculation lambda
    int LAMBDA = 1;

    switch(LAMBDA)
    {
        case 1:
        control -> cloth_lambda("lambda1");
        break;
        case 2:
        control -> cloth_lambda("lambda2");
        break;
        case 3:
        control -> cloth_lambda("lambda3");
        break;
    }
    control -> cloth_input("../data/");
    control -> cloth_output("../output/debug/");

    // the input and output list can be found in the output folder
    // the label of our cloth is from 1-0001 to 75-0075

    int CT;   // cloth template
    int CR;   // cloth reference
    int BS;   // cloth base
    int FILE; // file name

    double deltaT; // time step

    // create the folder
    std::string dir = "../output/debug/readme";
    if (access(dir.c_str(), 0) == -1)
    {
        mkdir(dir.c_str(), 0777);
    }

    ////////////////////////////////
    ///// START THE SIMULATION /////
    ////////////////////////////////

    // slot should be chosen from 1 to 74
    for(int slot=1; slot<3; slot++)
    {
        CT = slot;
        CR = slot+3;
        BS = slot;
        FILE = slot;
        deltaT = 0.003;


        cloth_calc* slot_CT = new cloth_calc(control->GetInput(CT-1) , control->GetInput(CR-1), control->GetInput(BS-1));
        cloth_calc* slot_CR = new cloth_calc(control->GetInput(CT) , control->GetInput(CR), control->GetInput(BS));

        switch(MODE)
        {
            case 1:
            {
                slot_CT -> cloth_eig_neighbor();

                slot_CR -> cloth_eig_neighbor();
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighbor(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetOutput(FILE));
                    break;
                }
                slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
                break;
            }
            case 2:
            {
                slot_CT -> cloth_eig_neighbor2x();

                slot_CR -> cloth_eig_neighbor2x();
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighbor2x(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetOutput(FILE));
                    break;
                }
                slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
                break;
            }
            case 3:
            {
                slot_CT -> cloth_eig_kdTree();

                slot_CR -> cloth_eig_kdTree();
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighborKdTree(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetOutput(FILE));
                    break;
                }
                slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);

                slot_CR -> cloth_velGrad_assemble(slot_CR->GetStrTensor());
                break;


            }
        }



        std::ofstream outfile(control->Readme(FILE));
        outfile << "Selected mode is: " << MODE << std::endl;
        outfile << "Template is: "  << control->GetInput(CT) << std::endl;
        outfile << "Reference is: " << control->GetInput(CR) << std::endl;
        outfile << "Base is: "      << control->GetInput(BS) << std::endl;
        outfile << "Lambda is: "    << control->GetLambda() << std::endl;
        outfile.close();


        ///////////////////////////////
        ////// THIS IS FOR DEBUG //////
        ///////////////////////////////

        // std::cout << control->Readme(FILE) << std::endl;
        // std::ofstream Test("../output/D_KdTree.txt");
        // Test<< slot_CR->GetStrTensor() << std::endl;
        // Test.close();

    }


    w.show();

    return a.exec();


}
