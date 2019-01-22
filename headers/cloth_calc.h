#ifndef CLOTH_CALC_H
#define CLOTH_CALC_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/Eigenvalues>

#include <ply_Module.h>


class cloth_calc
{

public:
    cloth_calc();
    ~cloth_calc();

    // Rohmer et al.
    void cloth_vec();
    void cloth_eig();
    void cloth_defo();
    void cloth_displ();

    void test();
    Eigen::MatrixXd GetVecR();
    Eigen::MatrixXd GetVecT();

    Eigen::MatrixXd GetEigval();
    Eigen::MatrixXd GetEigvec();

    Eigen::MatrixXd GetDefo();
    Eigen::MatrixXd GetDispl();


    //
    void cloth_vec_infl();
    void cloth_eig_infl();
    void cloth_defo_infl();

    Eigen::MatrixXd GetVecR_infl();
    Eigen::MatrixXd GetVecT_infl();

    Eigen::MatrixXd GetEigval_infl();
    Eigen::MatrixXd GetEigvec_infl();

    Eigen::MatrixXd GetDefo_infl();

    //
    void cloth_calc_norm(Eigen::MatrixXd Eigenval, int dim);
    Eigen::MatrixXd GetEig_norm();

    void cloth_calc_Color(Eigen::MatrixXd Eigval, int dim);
    Eigen::MatrixXd GetColor_vert1();
    Eigen::MatrixXd GetColor_vert2();
    Eigen::MatrixXd GetColor_vert3();

    void cloth_WriteColor(Eigen::MatrixXd color);


private:
    ply_module* _plyModuleR;
    ply_module* _plyModuleT;

    Eigen::MatrixXi faces;

    // Rohmer et al.
    Eigen::MatrixXd VecR, VecT;
    Eigen::MatrixXd Eigval_sq, Eigvec_sq;
    Eigen::MatrixXd Defo;
    Eigen::MatrixXd Displ;

    //
    Eigen::MatrixXd VecR_infl, VecT_infl;

    //
    Eigen::MatrixXd Eigval_norm;
    Eigen::MatrixXd Color_vert1, Color_vert2, Color_vert3;
};

#endif // CLOTH_CALC_H
