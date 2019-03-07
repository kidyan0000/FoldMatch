#ifndef CLOTH_CALC_H
#define CLOTH_CALC_H

#include <DivergingColorMaps.hpp>
#include <nanoflann.hpp>

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
    void cloth_stretchTensor_2D();
    void cloth_stretchTensor_3D();
    void cloth_stretchTensor_assemble();
    void cloth_displGrad_2D();

    // USING NEIGHBOR
    void cloth_eig_neighbor();
    void cloth_eig_neighbor2x();
    void cloth_stretchTensor_neighbor();

    // USING KD-TREE
    void cloth_eig_kdTree();

    void cloth_velGrad_3D(Eigen::MatrixXd F_CT, Eigen::MatrixXd F_CR, double deltaT);

    // COLLORMAP
    void cloth_vec_normalize(Eigen::MatrixXd Eigenval, int dim);
    void cloth_WriteColor(Eigen::MatrixXd color, const std::string &  ifileName);

    void test();

    Eigen::MatrixXd GetEigval();
    Eigen::MatrixXd GetEigvec();
    Eigen::MatrixXd GetStretchTensor_2D();
    Eigen::MatrixXd GetStretchTensor_3D();
    Eigen::MatrixXd GetDefoGrad();
    Eigen::MatrixXd GetEigval_3D();
    Eigen::MatrixXd GetEigvec_3D();
    Eigen::MatrixXd GetEigval_neighbor();
    Eigen::MatrixXd GetEigvec_neighbor();
    Eigen::MatrixXd GetEigval_neighbor2x();
    Eigen::MatrixXd GetEigvec_neighbor2x();
    Eigen::MatrixXd GetEigval_neighborKdTree();
    Eigen::MatrixXd GetEigvec_neighborKdTree();
    Eigen::MatrixXd GetEigval_norm_dir1();
    Eigen::MatrixXd GetEigval_norm_dir2();
    Eigen::MatrixXd GetEigval_norm_dir3();
    Eigen::MatrixXd GetVelTensor();
    Eigen::MatrixXd GetRotTensor();


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

    Eigen::MatrixXd F_der;
    Eigen::MatrixXd F_inv;
    Eigen::MatrixXd L;
    Eigen::MatrixXd D;
    Eigen::MatrixXd W;

    //
    Eigen::MatrixXd Eigval_neighbor, Eigvec_neighbor;
    Eigen::MatrixXd Eigval_neighbor2x, Eigvec_neighbor2x;
    Eigen::MatrixXd Eigval_neighborKdTree, Eigvec_neighborKdTree;
    Eigen::MatrixXd U_neighbor;
    Eigen::MatrixXd H_neighbor;

    //
    Eigen::MatrixXd Eigval_norm_dir1, Eigval_norm_dir2, Eigval_norm_dir3;
};

#endif // CLOTH_CALC_H
