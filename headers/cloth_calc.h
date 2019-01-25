#ifndef CLOTH_CALC_H
#define CLOTH_CALC_H

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <TriMesh/TriMesh.h>
// #include <libmesh/node_elem.h>

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
    void cloth_eig_neighbor();
    void cloth_defo_neighbor();

    Eigen::MatrixXd GetEigval_neighbor();
    Eigen::MatrixXd GetEigvec_neighbor();

    Eigen::MatrixXd GetDefo_neighbor();

    //
    void cloth_vec_normalize(Eigen::MatrixXd Eigenval, int dim);
    Eigen::MatrixXd GetEigval_norm_dir1();
    Eigen::MatrixXd GetEigval_norm_dir2();
    Eigen::MatrixXd GetEigval_norm_dir3();

    void cloth_WriteColor(Eigen::MatrixXd color, const std::string &  ifileName);


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
    Eigen::MatrixXd Eigval_sq_neighbor, Eigvec_sq_neighbor;
    Eigen::MatrixXd Defo_neighbor;
    Eigen::MatrixXd Displ_neighbor;

    //
    Eigen::MatrixXd Eigval_norm_dir1, Eigval_norm_dir2, Eigval_norm_dir3;
};

#endif // CLOTH_CALC_H
