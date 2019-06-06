#include "cloth_window.h"
#include "cloth_glwidget.h"
#include "cloth_calc.h"
#include "cloth_control.h"

#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <string>


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
    // MODE 3: Neighbor3x
    // MODE 4: Neighbor4x
    // MODE 5: KD-Tree
    // MODE 0: TEST
    int MODE = 4;

    // settings writing results
    // CAL 1: lambda
    // CAL 2: wrinkel vector field
    // CAL 3: velocity gradient
    // CAL 4: vertices update
    // CAL 5: lambda assemble Riemann
    // CAL 6: lambda assemble CCM
    // CAL 7: wrinkel vector field assemble CCM
    //
    int CAL = 1;

    double Per = 0.01; // Kd-tree parameters

    double deltaT = 0.006; // time step


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
    if(CAL == 5 || CAL == 6)
    {
        switch(LAMBDA)
        {
            case 1:
            control -> cloth_lambdaAssemble("lambda1_assemble");
            break;
            case 2:
            control -> cloth_lambdaAssemble("lambda2_assemble");
            break;
            case 3:
            control -> cloth_lambdaAssemble("lambda3_assemble");
            break;
        }

    }

    control -> cloth_input("../data/Tshirt/");
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
        case 4:
        {
            control -> cloth_vertsUpdate_output("../output/debug/");
            break;
        }
        case 5:
        {
            control -> cloth_lambdaAssemble_output("../output/debug/");
            break;
        }
        case 6:
        {
            control -> cloth_stretchCCM_output("../output/debug/");
            control -> cloth_stretchFreq_output("../output/debug/");
            control -> cloth_stretchMap_output("../output/debug/");

            control -> cloth_lambdaAssemble_output("../output/debug/");
            break;
        }
        case 7:
        {
            control -> cloth_stretchCCM_output("../output/debug/");
            control -> cloth_stretchFreq_output("../output/debug/");
            control -> cloth_stretchMap_output("../output/debug/");

            control -> cloth_wrinkVecField_output("../output/debug/");
            break;
        }
    }


    // the input and output list can be found in the output folder
    // the label of our cloth is from 1-0001 to 75-0075

    int CT=1;   // cloth template
    int CR=CT+1;   // cloth reference
    int BS=1;   // cloth base
    int FILE=1; // file name

    // create the folder
    std::string dir = "../output/debug/readme";
    if (access(dir.c_str(), 0) == -1)
    {
        mkdir(dir.c_str(), 0777);
    }

    // initialize the map neighbor and first update
    cloth_calc* slot_map = new cloth_calc(control->GetInput(0) , control->GetInput(1), control->GetInput(0));

    cloth_calc* slot_CT = new cloth_calc(control->GetInput(CT) , control->GetInput(CR), control->GetInput(BS));
    cloth_calc* slot_CR = new cloth_calc(control->GetInput(CT) , control->GetInput(CR), control->GetInput(BS));

    if(MODE == 1 || MODE == 2 ||MODE == 3 ||MODE == 4)
    {
        slot_map -> cloth_map_neighbor(MODE);
    }


    /////////////////////////////////////////////////////////////////////
    /// intialize the slot 1
    /////////////////////////////////////////////////////////////////////

    switch(MODE)
    {
        case 1:
        {
            slot_CT -> cloth_eig_neighbor(slot_map->GetMapNeighbor());
            slot_CR -> cloth_eig_neighbor(slot_map->GetMapNeighbor());

            if(CAL == 1)
            {
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighbor(), 3);
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
            if(CAL == 2)
            {
                slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighbor(), slot_CR->GetEigvec_neighbor());
                slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
            }
            if(CAL == 5)
            {
                // calculate the stretch tensor U and U_assemble
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor(), slot_CR->GetEigvec_neighbor());
                slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor());
                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                    break;
                }
            }
            break;
        }
        case 2:
        {
            slot_CT -> cloth_eig_neighbor2x(slot_map->GetMapNeighbor2x());
            slot_CR -> cloth_eig_neighbor2x(slot_map->GetMapNeighbor2x());
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

            if(CAL == 2)
            {

                // setting wrinkel vector field
                slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighbor2x(), slot_CR->GetEigvec_neighbor2x());
                slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
            }
            if(CAL == 5)
            {
                // calculate the stretch tensor U and U_assemble
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor2x(), slot_CR->GetEigvec_neighbor2x());
                slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor2x());
                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                    break;
                }
            }
            if(CAL == 6)
            {
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor2x(), slot_CR->GetEigvec_neighbor2x());
                slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor2x(), slot_map->GetMapNeighbor2x());

                std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                StretchCCM.close();

                std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                StretchFreq.close();

                std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                StretchMap.close();

                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                    break;
                }
            }
            if(CAL == 7)
            {
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor2x(), slot_CR->GetEigvec_neighbor2x());
                slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor2x(), slot_map->GetMapNeighbor2x());

                std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                StretchCCM.close();

                std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                StretchFreq.close();

                std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                StretchMap.close();

                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_assemble(), slot_CR->GetEigvec_assemble());
                slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
            }
            break;
        }
        case 3:
        {
            slot_CT -> cloth_eig_neighbor3x(slot_map->GetMapNeighbor3x());
            slot_CR -> cloth_eig_neighbor3x(slot_map->GetMapNeighbor3x());

            if(CAL == 1)
            {
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighbor3x(), 3);
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


            if(CAL == 2)
            {
                // setting wrinkel vector field
                slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighbor3x(), slot_CR->GetEigvec_neighbor3x());
                slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
            }
            if(CAL == 5)
            {
                // calculate the stretch tensor U and U_assemble
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor3x(), slot_CR->GetEigvec_neighbor3x());
                slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor3x());
                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                    break;
                }
            }
            if(CAL == 6)
            {
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor3x(), slot_CR->GetEigvec_neighbor3x());
                slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor3x(), slot_map->GetMapNeighbor3x());

                std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                StretchCCM.close();

                std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                StretchFreq.close();

                std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                StretchMap.close();

                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                    break;
                }
            }
            if(CAL == 7)
            {
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor3x(), slot_CR->GetEigvec_neighbor3x());
                slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor3x(), slot_map->GetMapNeighbor3x());

                std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                StretchCCM.close();

                std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                StretchFreq.close();

                std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                StretchMap.close();

                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_assemble(), slot_CR->GetEigvec_assemble());
                slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
            }
            // slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
            break;
        }
        case 4:
        {
            slot_CT -> cloth_eig_neighbor4x(slot_map->GetMapNeighbor4x());
            slot_CR -> cloth_eig_neighbor4x(slot_map->GetMapNeighbor4x());

            if(CAL == 1)
            {

                slot_CR -> cloth_ReadTransformationMat(control -> GetInputTransf(FILE));
                slot_CR -> cloth_vec_normalize_text(slot_CR->GetTransformationMat(), 3);
                //slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighbor4x(), 3);
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
            if(CAL == 2)
            {
                slot_CR -> cloth_ReadTransformationMat(control -> GetInputTransf(FILE));
                slot_CR -> cloth_wrink_vec_field_text(slot_CR->GetTransformationMat());
                slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));

                // slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighbor4x(), slot_CR->GetEigvec_neighbor4x());
                // slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
            }

            if(CAL == 3)
            {
                slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
                slot_CR -> cloth_WriteColor(slot_CR->GetVertsUpdate(), control->GetWrinkVecFieldOutput(FILE));
            }
            if(CAL == 4)
            {
                /*
                // calculate the stretch tensor U and U_assemble
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor4x(), slot_CR->GetEigvec_neighbor4x());
                slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor4x());
                // now we do the optimazation
                slot_CR -> cloth_rotationTensor(slot_CR->GetDefoGrad(),slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_translationVec(slot_CR->GetRotationTensor(), slot_map->GetMapNeighbor4x());
                slot_CR -> cloth_transformationMat(slot_CR->GetRotationTensor(), slot_CR->GetTranslationVec());
                slot_CR -> cloth_update(slot_CR->GetRotationTensor(), slot_CR->GetTranslationVec());
                slot_CR -> cloth_WriteVerts(slot_CR->GetVertsUpdate(), control->GetVertsUpdateOutput(CR));
                // we update the vertices and do new calculation
                */
                slot_CR -> cloth_ReadTransformationMat(control -> GetInputTransf(FILE));
                slot_CR -> cloth_Opt(slot_CR->GetTransformationMat());
                // std::cout << slot_CR->GetRotationTensorOpt() << std::endl;
                slot_CR -> cloth_translationVec(slot_CR->GetRotationTensorOpt(), slot_map->GetMapNeighbor4x());
                slot_CR -> cloth_update(slot_CR->GetRotationTensorOpt(), slot_CR->GetTranslationVec());
                slot_CR -> cloth_WriteVerts(slot_CR->GetVertsUpdate(), control->GetVertsUpdateOutput(CR));
            }
            if(CAL == 5)
            {
                // calculate the stretch tensor U and U_assemble
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor4x(), slot_CR->GetEigvec_neighbor4x());
                slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor4x());
                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                    break;
                }
            }
            if(CAL == 6)
            {
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor4x(), slot_CR->GetEigvec_neighbor4x());
                slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor4x(), slot_map->GetMapNeighbor4x());

                std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                StretchCCM.close();

                std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                StretchFreq.close();

                std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                StretchMap.close();

                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                    break;
                }
            }
            if(CAL == 7)
            {
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor4x(), slot_CR->GetEigvec_neighbor4x());
                slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor4x(), slot_map->GetMapNeighbor4x());

                std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                StretchCCM.close();

                std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                StretchFreq.close();

                std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                StretchMap.close();

                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_assemble(), slot_CR->GetEigvec_assemble());
                slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
            }
            break;
        }
        case 5:
        {
            slot_CT -> cloth_eig_kdTree(Per);
            slot_CR -> cloth_eig_kdTree(Per);

            if(CAL == 1)
            {
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighborKdTree(), 3);
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
            if(CAL == 2)
            {
                slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighborKdTree(), slot_CR->GetEigvec_neighborKdTree());
                slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
            }
            if(CAL == 3)
            {
                // settings for velocity gradient
                slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
                // smooth the velocity gradient
                slot_CR -> cloth_velGrad_assemble(slot_CR->GetStrTensor(), Per);
                // normalize the velocity gradient
                slot_CR -> cloth_velGrad_normalize(slot_CR->GetStrTensorAsemmble());
                slot_CR -> cloth_WriteColor(slot_CR->GetStrTensor_norm_dir1(), control->GetStretchOutput(FILE));

            }
            if(CAL == 4)
            {
                // calculate the stretch tensor U and U_assemble
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighborKdTree(), slot_CR->GetEigvec_neighborKdTree());
                slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor());

                // now we do the optimazation
                slot_CR -> cloth_rotationTensor(slot_CR->GetDefoGrad(),slot_CR->GetStretchTensor_3D());
                slot_CR -> cloth_translationVec(slot_CR->GetRotationTensor(), slot_map->GetMapNeighbor());
                slot_CR -> cloth_transformationMat(slot_CR->GetRotationTensor(), slot_CR->GetTranslationVec());
                slot_CR -> cloth_update(slot_CR->GetRotationTensor(), slot_CR->GetTranslationVec());
                slot_CR -> cloth_WriteVerts(slot_CR->GetVertsUpdate(), control->GetVertsUpdateOutput(CR));

            }
            if(CAL == 5)
            {
                // calculate the stretch tensor U and U_assemble
                slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighborKdTree(), slot_CR->GetEigvec_neighborKdTree());
                slot_CR -> cloth_stretchTensor_kdTree(slot_CR->GetStretchTensor_3D(), Per);
                slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                switch(LAMBDA)
                {
                    case 1:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 2:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                    break;
                    case 3:
                    slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                    break;
                }
            }
            break;
        }
    case 0:
        {
            // std::cout << control -> GetInputTransf(FILE) << std::endl;
            // std::cout << control -> GetInputFreq(FILE) << std::endl;

            /*
            slot_map -> cloth_map_neighbor(4);
            slot_CR -> cloth_eig_neighbor4x(slot_map->GetMapNeighbor4x());

            slot_CR->cloth_ReadTransformationMat(control -> GetInputTransf(FILE), control -> GetInputFreq(FILE));
            slot_CR->cloth_Opt(slot_CR->GetTransformationMat(), slot_CR->GetDefoGrad());

            slot_CR -> cloth_translationVec(slot_CR->GetRotationTensorOpt(), slot_map->GetMapNeighbor4x());
            slot_CR -> cloth_update(slot_CR->GetRotationTensorOpt(), slot_CR->GetTranslationVec());
            slot_CR -> cloth_WriteVerts(slot_CR->GetVertsUpdate(), "../test.ply");
            // std::cout << slot_CR->GetRotTensorOpt()<<std::endl;
            */
            slot_CR->cloth_ReadTransformationMat(control -> GetInputTransf(FILE));
            slot_CR -> test(slot_CR->GetTransformationMat());
            slot_CR -> cloth_translationVec(slot_CR->GetRotationTensorOpt(), slot_map->GetMapNeighbor4x());
            slot_CR -> cloth_update(slot_CR->GetRotationTensorOpt(), slot_CR->GetTranslationVec());
            slot_CR -> cloth_WriteVerts(slot_CR->GetVertsUpdate(), control->GetVertsUpdateOutput(CR));
        }
        break;
    }



    std::ofstream outfile(control->Readme(FILE));
    outfile << "Selected mode is: " << MODE << std::endl;
    if(MODE == 5)
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
    if(CAL == 4)
    {
        outfile << "Template update is: "    << control->GetVertsUpdateOutput(CR) << std::endl;
        outfile << "The Transformation txt file is: " << control->GetInputTransf(FILE) << std::endl;
    }
    outfile << "delta T is: "    << deltaT << std::endl;
    outfile.close();

    ////////////////////////////////
    ///// START THE SIMULATION /////
    ////////////////////////////////

    // slot should be chosen from 1 to 74
    for(int slot=2; slot<101; slot++)
    {
        CT = slot;
        CR = slot+1;
        BS = slot;
        FILE = slot;

        cloth_calc* slot_CT = new cloth_calc(control->GetInput(CT) , control->GetInput(CR), control->GetInput(BS));
        // cloth_calc* slot_CR = new cloth_calc(control->GetVertsUpdateOutput(CT) , control->GetInput(CR), control->GetVertsUpdateOutput(BS));
        cloth_calc* slot_CR = new cloth_calc(control->GetInput(CT) , control->GetInput(CR), control->GetInput(BS));

        switch(MODE)
        {
            case 1:
            {
                slot_CT -> cloth_eig_neighbor(slot_map->GetMapNeighbor());
                slot_CR -> cloth_eig_neighbor(slot_map->GetMapNeighbor());

                if(CAL == 1)
                {
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighbor(), 3);
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
                if(CAL == 2)
                {
                    slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighbor(), slot_CR->GetEigvec_neighbor());
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
                }
                if(CAL == 5)
                {
                    // calculate the stretch tensor U and U_assemble
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor(), slot_CR->GetEigvec_neighbor());
                    slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor());
                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                        break;
                    }
                }
                break;
            }
            case 2:
            {
                slot_CT -> cloth_eig_neighbor2x(slot_map->GetMapNeighbor2x());
                slot_CR -> cloth_eig_neighbor2x(slot_map->GetMapNeighbor2x());
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
                if(CAL == 2)
                {

                    // setting wrinkel vector field
                    slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighbor2x(), slot_CR->GetEigvec_neighbor2x());
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
                }
                if(CAL == 5)
                {
                    // calculate the stretch tensor U and U_assemble
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor2x(), slot_CR->GetEigvec_neighbor2x());
                    slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor2x());
                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                        break;
                    }
                }
                if(CAL == 6)
                {
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor2x(), slot_CR->GetEigvec_neighbor2x());
                    slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor2x(), slot_map->GetMapNeighbor2x());

                    std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                    StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                    StretchCCM.close();

                    std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                    StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                    StretchFreq.close();

                    std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                    StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                    StretchMap.close();

                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                        break;
                    }
                }
                if(CAL == 7)
                {
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor2x(), slot_CR->GetEigvec_neighbor2x());
                    slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor2x(), slot_map->GetMapNeighbor2x());

                    std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                    StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                    StretchCCM.close();

                    std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                    StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                    StretchFreq.close();

                    std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                    StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                    StretchMap.close();

                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_assemble(), slot_CR->GetEigvec_assemble());
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
                }
                break;
            }
            case 3:
            {
                slot_CT -> cloth_eig_neighbor3x(slot_map->GetMapNeighbor3x());
                slot_CR -> cloth_eig_neighbor3x(slot_map->GetMapNeighbor3x());

                if(CAL == 1)
                {
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighbor3x(), 3);
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


                if(CAL == 2)
                {
                    // setting wrinkel vector field
                    slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighbor3x(), slot_CR->GetEigvec_neighbor3x());
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(CR));
                }
                if(CAL == 5)
                {
                    // calculate the stretch tensor U and U_assemble
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor3x(), slot_CR->GetEigvec_neighbor3x());
                    slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor3x());
                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                        break;
                    }
                }
                if(CAL == 6)
                {
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor3x(), slot_CR->GetEigvec_neighbor3x());
                    slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor3x(), slot_map->GetMapNeighbor3x());

                    std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                    StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                    StretchCCM.close();

                    std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                    StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                    StretchFreq.close();

                    std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                    StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                    StretchMap.close();

                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                        break;
                    }
                }
                if(CAL == 7)
                {
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor3x(), slot_CR->GetEigvec_neighbor3x());
                    slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor3x(), slot_map->GetMapNeighbor3x());

                    std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                    StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                    StretchCCM.close();

                    std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                    StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                    StretchFreq.close();

                    std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                    StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                    StretchMap.close();

                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_assemble(), slot_CR->GetEigvec_assemble());
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
                }
                // slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
                break;
            }
            case 4:
            {
                slot_CT -> cloth_eig_neighbor4x(slot_map->GetMapNeighbor4x());
                slot_CR -> cloth_eig_neighbor4x(slot_map->GetMapNeighbor4x());

                if(CAL == 1)
                {
                    slot_CR -> cloth_ReadTransformationMat(control -> GetInputTransf(FILE));
                    slot_CR -> cloth_vec_normalize_text(slot_CR->GetTransformationMat(), 3);

                    // slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighbor4x(), 3);

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
                if(CAL == 2)
                {
                    slot_CR -> cloth_ReadTransformationMat(control -> GetInputTransf(FILE));
                    slot_CR -> cloth_wrink_vec_field_text(slot_CR->GetTransformationMat());
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));

                    // slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighbor4x(), slot_CR->GetEigvec_neighbor4x());
                    // slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
                }

                if(CAL == 3)
                {
                    slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
                    slot_CR -> cloth_WriteColor(slot_CR->GetVertsUpdate(), control->GetWrinkVecFieldOutput(FILE));
                }
                if(CAL == 4)
                {
                    /*
                    // calculate the stretch tensor U and U_assemble
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor4x(), slot_CR->GetEigvec_neighbor4x());
                    slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor4x());
                    // now we do the optimazation
                    slot_CR -> cloth_rotationTensor(slot_CR->GetDefoGrad(),slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_translationVec(slot_CR->GetRotationTensor(), slot_map->GetMapNeighbor4x());
                    slot_CR -> cloth_transformationMat(slot_CR->GetRotationTensor(), slot_CR->GetTranslationVec());
                    slot_CR -> cloth_update(slot_CR->GetRotationTensor(), slot_CR->GetTranslationVec());
                    slot_CR -> cloth_WriteVerts(slot_CR->GetVertsUpdate(), control->GetVertsUpdateOutput(CR));
                    // we update the vertices and do new calculation
                    */
                    slot_CR -> cloth_ReadTransformationMat(control -> GetInputTransf(FILE));
                    slot_CR -> cloth_Opt(slot_CR->GetTransformationMat());
                    slot_CR -> cloth_translationVec(slot_CR->GetRotationTensorOpt(), slot_map->GetMapNeighbor4x());
                    slot_CR -> cloth_update(slot_CR->GetRotationTensorOpt(), slot_CR->GetTranslationVec());
                    slot_CR -> cloth_WriteVerts(slot_CR->GetVertsUpdate(), control->GetVertsUpdateOutput(CR));
                }
                if(CAL == 5)
                {
                    // calculate the stretch tensor U and U_assemble
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor4x(), slot_CR->GetEigvec_neighbor4x());
                    slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor4x());
                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                        break;
                    }
                }
                if(CAL == 6)
                {
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor4x(), slot_CR->GetEigvec_neighbor4x());
                    slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor4x(), slot_map->GetMapNeighbor4x());

                    std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                    StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                    StretchCCM.close();

                    std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                    StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                    StretchFreq.close();

                    std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                    StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                    StretchMap.close();

                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                        break;
                    }
                }
                if(CAL == 7)
                {
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighbor4x(), slot_CR->GetEigvec_neighbor4x());
                    slot_CR -> cloth_stretchTensor_CCM(slot_CR->GetStretchTensor_3D(), slot_CR->GetEigval_neighbor4x(), slot_map->GetMapNeighbor4x());

                    std::ofstream StretchCCM(control->GetStretchCCMOutput(FILE));
                    StretchCCM << slot_CR->GetStretchTensorAsemmble()<< std::endl;
                    StretchCCM.close();

                    std::ofstream StretchFreq(control->GetStretchFreqOutput(FILE));
                    StretchFreq << slot_CR->GetStretchTensorFreq()<< std::endl;
                    StretchFreq.close();

                    std::ofstream StretchMap(control->GetStretchMapOutput(FILE));
                    StretchMap << slot_CR->GetStretchTensorMap()<< std::endl;
                    StretchMap.close();

                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_assemble(), slot_CR->GetEigvec_assemble());
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
                }
                break;
            }
            case 5:
            {
                slot_CT -> cloth_eig_kdTree(Per);
                slot_CR -> cloth_eig_kdTree(Per);

                if(CAL == 1)
                {
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_neighborKdTree(), 3);
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
                if(CAL == 2)
                {
                    slot_CR -> cloth_wrink_vec_field(slot_CR->GetEigval_neighborKdTree(), slot_CR->GetEigvec_neighborKdTree());
                    slot_CR -> cloth_WriteColor(slot_CR->GetWrinkVecField_norm(), control->GetWrinkVecFieldOutput(FILE));
                }
                if(CAL == 3)
                {
                    // settings for velocity gradient
                    slot_CR -> cloth_velGrad_3D(slot_CT->GetDefoGrad(), slot_CR->GetDefoGrad(), deltaT);
                    // smooth the velocity gradient
                    slot_CR -> cloth_velGrad_assemble(slot_CR->GetStrTensor(), Per);
                    // normalize the velocity gradient
                    slot_CR -> cloth_velGrad_normalize(slot_CR->GetStrTensorAsemmble());
                    slot_CR -> cloth_WriteColor(slot_CR->GetStrTensor_norm_dir1(), control->GetStretchOutput(FILE));

                }
                if(CAL == 4)
                {
                    // calculate the stretch tensor U and U_assemble
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighborKdTree(), slot_CR->GetEigvec_neighborKdTree());
                    slot_CR -> cloth_stretchTensor_assemble(slot_CR->GetStretchTensor_3D(), slot_map->GetMapNeighbor());

                    // now we do the optimazation
                    slot_CR -> cloth_rotationTensor(slot_CR->GetDefoGrad(),slot_CR->GetStretchTensor_3D());
                    slot_CR -> cloth_translationVec(slot_CR->GetRotationTensor(), slot_map->GetMapNeighbor());
                    slot_CR -> cloth_transformationMat(slot_CR->GetRotationTensor(), slot_CR->GetTranslationVec());
                    slot_CR -> cloth_update(slot_CR->GetRotationTensor(), slot_CR->GetTranslationVec());
                    slot_CR -> cloth_WriteVerts(slot_CR->GetVertsUpdate(), control->GetVertsUpdateOutput(CT));
                }
                if(CAL == 5)
                {
                    // calculate the stretch tensor U and U_assemble
                    slot_CR -> cloth_stretchTensor_3D(slot_CR->GetEigval_neighborKdTree(), slot_CR->GetEigvec_neighborKdTree());
                    slot_CR -> cloth_stretchTensor_kdTree(slot_CR->GetStretchTensor_3D(), Per);
                    slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
                    slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
                    switch(LAMBDA)
                    {
                        case 1:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 2:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir2(), control->GetLambdaAssembleOutput(FILE));
                        break;
                        case 3:
                        slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir3(), control->GetLambdaAssembleOutput(FILE));
                        break;
                    }
                }
                break;
            }
        case 0:
            {
                slot_CR->cloth_map_neighbor(MODE);
                break;
            }
        }

        std::ofstream outfile(control->Readme(FILE));
        outfile << "Selected mode is: " << MODE << std::endl;
        if(MODE == 5)
        {
            outfile << "Kd-Tree parameter is: "    << Per << std::endl;
        }
        outfile << "We calculate for: " << CAL << std::endl;
        if(CAL == 4)
        {
            outfile << "Template is: "  << control->GetVertsUpdateOutput(CT) << std::endl;
            outfile << "Reference is: " << control->GetInput(CR) << std::endl;
            outfile << "Base is: "      << control->GetVertsUpdateOutput(BS) << std::endl;
        }
        else
        {
            outfile << "Template is: "  << control->GetInput(CT) << std::endl;
            outfile << "Reference is: " << control->GetInput(CR) << std::endl;
            outfile << "Base is: "      << control->GetInput(BS) << std::endl;
        }

        if(CAL == 1)
        {
            outfile << "Lambda is: "    << control->GetLambda() << std::endl;
        }
        if(CAL == 4)
        {
            outfile << "Template update is: "    << control->GetVertsUpdateOutput(CR) << std::endl;
            outfile << "The Transformation txt file is: " << control->GetInputTransf(FILE) << std::endl;
        }
        outfile << "delta T is: "    << deltaT << std::endl;
        outfile.close();


        ///////////////////////////////
        ////// THIS IS FOR DEBUG //////
        ///////////////////////////////


        // Eigen::MatrixXd test;
        // test.resize(3,3);
        //test << 3,4,5,1,2,0,0,0,1;
        // test.row(0) << 2,2,2;
        // std::cout << test.row(0).norm()  << std::endl;
        // Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv(test);
        // std::cout << solv.eigenvectors() << std::endl;

    }
    // Eigen::MatrixXd test = Eigen::MatrixXd::Identity(3,3);

    // test.row(0) << 2,2,2;
    // std::cout << test  << std::endl;
    // Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv(test);
    // std::cout << solv.eigenvectors() << std::endl;
    // slot_CR -> cloth_eig_assemble(slot_CR->GetStretchTensorAsemmble());
    // slot_CR -> cloth_vec_normalize(slot_CR->GetEigval_assemble(), 3);
    // for(int i = 1;i<75;i++)
    // {
    //     std::cout << control->GetLambdaAssembleOutput(i) << std::endl;
    // }

    // std::ofstream Test("../output/R_opt.txt");
    // Test << slot_CR->GetRotationTensorOpt()<< std::endl;
    // Test.close();

    // slot_CR -> cloth_WriteColor(slot_CR->GetEigval_norm_dir1(), "../output/debug/lambda1_assemble.ply");


    w.show();

    return a.exec();


}
