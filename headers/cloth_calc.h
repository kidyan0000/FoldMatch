#ifndef CLOTH_CALC_H
#define CLOTH_CALC_H

#include <DivergingColorMaps.hpp>

#include <iostream>
#include <math.h>
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
    cloth_calc(std::string Cloth_Template, std::string Cloth_Reference, std::string Cloth_Base );
    ~cloth_calc();

    void cloth_init_vert();
    void cloth_init_neighbor();

    // Rhomer et al.
    void cloth_vec();
    void cloth_eig_2D();
    void cloth_eig_3D();
    void cloth_U_2D();
    void cloth_H_2D();
    void cloth_U_3D();
    void cloth_U_assemble();
    // USING NEIGHBOR
    void cloth_eig_neighbor();
    void cloth_eig_neighbor2x();
    void cloth_U_neighbor();

    void cloth_L_3D(Eigen::MatrixXd F_CT, Eigen::MatrixXd F_CR, double deltaT);

    // COLLORMAP
    void cloth_vec_normalize(Eigen::MatrixXd Eigenval, int dim);
    void cloth_WriteColor(Eigen::MatrixXd color, const std::string &  ifileName);

    void test();

    Eigen::MatrixXd GetEigval();
    Eigen::MatrixXd GetEigvec();
    Eigen::MatrixXd GetU_2D();
    Eigen::MatrixXd GetU_3D();
    Eigen::MatrixXd GetF_3D();
    Eigen::MatrixXd GetEigval_3D();
    Eigen::MatrixXd GetEigvec_3D();
    Eigen::MatrixXd GetEigval_neighbor();
    Eigen::MatrixXd GetEigvec_neighbor();
    Eigen::MatrixXd GetEigval_neighbor2x();
    Eigen::MatrixXd GetEigvec_neighbor2x();
    Eigen::MatrixXd GetEigval_norm_dir1();
    Eigen::MatrixXd GetEigval_norm_dir2();
    Eigen::MatrixXd GetEigval_norm_dir3();
    Eigen::MatrixXd GetD_3D();
    Eigen::MatrixXd GetW_3D();


private:
    std::string CT;
    std::string CR;
    std::string BS;
    ply_module* _plyModuleT;
    ply_module* _plyModuleR;
    ply_module* _plyModule;

    trimesh::TriMesh *_plyMeshT;
    trimesh::TriMesh *_plyMeshR;
    trimesh::TriMesh *_plyMesh;


    Eigen::MatrixXi faces;
    Eigen::MatrixXd vertsT, vertsR, verts;

    // Rohmer et al.
    Eigen::MatrixXd VecT, VecR;
    Eigen::MatrixXd F;
    Eigen::MatrixXd Eigval_2D, Eigvec_2D;
    Eigen::MatrixXd Eigval_3D, Eigvec_3D;
    Eigen::MatrixXd U_2D;
    Eigen::MatrixXd H_2D;
    Eigen::MatrixXd U_3D;
    Eigen::MatrixXd U_3D_assem;

    Eigen::MatrixXd F_3D_der;
    Eigen::MatrixXd F_3D_inv;
    Eigen::MatrixXd L_3D;
    Eigen::MatrixXd D_3D;
    Eigen::MatrixXd W_3D;

    //
    Eigen::MatrixXd Eigval_neighbor, Eigvec_neighbor;
    Eigen::MatrixXd Eigval_neighbor2x, Eigvec_neighbor2x;
    Eigen::MatrixXd U_neighbor;
    Eigen::MatrixXd H_neighbor;

    //
    Eigen::MatrixXd Eigval_norm_dir1, Eigval_norm_dir2, Eigval_norm_dir3;
};

#endif // CLOTH_CALC_H
