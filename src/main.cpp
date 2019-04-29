#include "cloth_window.h"
#include "cloth_glwidget.h"
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

    // control the OpengL settings
    // Cloth_GLWidget *GLWidget = new Cloth_GLWidget();

    // setting calculation mode
    // MODE 1: Neighbor1x
    // MODE 2: Neighbor2x
    // MODE 3: KD-TREE
    int MODE = 1;

    // settings writing results
    // CAL 1: lambda
    // CAL 2: wrinkel vector field
    int CAL = 2;

    // setting calculation lambda
    int LAMBDA = 1;
    if(CAL == 1)
    {
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

    }

    control -> cloth_input("../data/");
    switch(CAL)
    {
        case 1:
        {
            control -> cloth_lambda_output("../output/debug/");
            break;
        }
        case 2:
        {
            control -> cloth_wrinkVecField_output("../output/debug/");
            break;
        }
        case 3:
        {
            control -> cloth_stretch_output("../output/debug/");
            break;
        }
    }


    // the input and output list can be found in the output folder
    // the label of our cloth is from 1-0001 to 75-0075

    int CT;   // cloth template
    int CR;   // cloth reference
    int BS;   // cloth base
    int FILE; // file name
    double Per; // Kd-tree parameters

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
    for(int slot=1; slot<75; slot++)
    {
        CT = slot;
        CR = slot+3;
        BS = slot;
        FILE = slot;

        Per = 0.03;

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
                if(CAL == 1)
                {
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaOutput(FILE));
                        break;
                    }
                }

                // setting wrinkel vector field
                slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighbor(), slot_CR->GetEigvec_neighbor());
                if(CAL == 2)
                {
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
                }
                // slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
                break;
            }
            case 2:
            {
                slot_CT -> cloth_eig_neighbor2x();

                slot_CR -> cloth_eig_neighbor2x();
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighbor2x(), 3);
                if(CAL == 1)
                {
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaOutput(FILE));
                        break;
                    }
                }

                // setting wrinkel vector field
                slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighbor2x(), slot_CR->GetEigvec_neighbor2x());
                if(CAL == 2)
                {
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
                }

                // slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
                break;
            }
            case 3:
            {
                slot_CT -> cloth_eig_kdTree(Per);

                slot_CR -> cloth_eig_kdTree(Per);
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighborKdTree(), 3);
                if(CAL == 1)
                {
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaOutput(FILE));
                        break;
                    }
                }

                // setting wrinkel vector field
                slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighborKdTree(), slot_CR->GetEigvec_neighborKdTree());
                if(CAL == 2)
                {
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
                }

                // settings for velocity gradient
                slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
                // smooth the velocity gradient
                slot_CR -> cloth_velGrad_assemble(slot_CR->GetStrTensor());
                // normalize the velocity gradient
                slot_CR -> cloth_velGrad_normalize(slot_CR->GetStrTensor());
                // slot_CR -> cloth_WriteColor(slot_CR->GetStrTensor_norm_dir1(), control->GetStretch(FILE));


                break;


            }
        }



        std::ofstream outfile(control->Readme(FILE));
        outfile << "Selected mode is: " << MODE << std::endl;
        if(MODE == 3)
        {
            outfile << "Kd-Tree parameter is: "    << Per << std::endl;
        }
        outfile << "We calculate for: " << CAL << std::endl;
        outfile << "Template is: "  << control->GetInput(CT) << std::endl;
        outfile << "Reference is: " << control->GetInput(CR) << std::endl;
        outfile << "Base is: "      << control->GetInput(BS) << std::endl;
        if(CAL == 1)
        {
            outfile << "Lambda is: "    << control->GetLambda() << std::endl;
        }
        outfile << "delta T is: "    << deltaT << std::endl;
        outfile.close();


        ///////////////////////////////
        ////// THIS IS FOR DEBUG //////
        ///////////////////////////////

        // std::cout << "a" << std::endl;
        // std::ofstream Test("../output/v_norm.txt");
        // Test<< slot_CR->GetWrinkVecField_norm()<< std::endl;
        // Test.close();

        // Eigen::MatrixXd test;
        // test.resize(3,3);
        // test << 1,1,0,1,2,0,0,0,1;
        // std::cout << test << std::endl;
        // Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv(test);
        // std::cout << solv.eigenvectors() << std::endl;

    }


    w.show();

    return a.exec();


}
