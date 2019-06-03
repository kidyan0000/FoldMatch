#ifndef CLOTH_CALC_H
#define CLOTH_CALC_H

#include <DivergingColorMaps.hpp>
#include <nanoflann.hpp>

#include <iostream>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <map>

#include <TriMesh/TriMesh.h>
// #include <libmesh/node_elem.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/Eigenvalues>

#include <unsupported/Eigen/NonLinearOptimization>
#include <unsupported/Eigen/NumericalDiff>

#include <ply_Module.h>
using namespace Eigen;

// Generic functor
template<typename _Scalar, int NX = Eigen::Dynamic, int NY = Eigen::Dynamic>
struct Functor
{
    typedef _Scalar Scalar;
    enum {
        InputsAtCompileTime = NX,
        ValuesAtCompileTime = NY
};
typedef Eigen::Matrix<Scalar,InputsAtCompileTime,1> InputType;
typedef Eigen::Matrix<Scalar,ValuesAtCompileTime,1> ValueType;
typedef Eigen::Matrix<Scalar,ValuesAtCompileTime,InputsAtCompileTime> JacobianType;

int m_inputs, m_values;

Functor() : m_inputs(InputsAtCompileTime), m_values(ValuesAtCompileTime) {}
Functor(int inputs, int values) : m_inputs(inputs), m_values(values) {}

int inputs() const { return m_inputs; }
int values() const { return m_values; }

};

struct my_functor : Functor<double>
{
 // private:
    Eigen::MatrixXd     U_input, R_input, F_input, T_input;
    int                 Vert_num;
 // public:
    /*
    void setValues(Eigen::MatrixXd T)
    {
        this -> F_input = T.block(0,0,3,3);
        this -> R_input = T.block(0,3,3,3);
        this -> U_input = T.block(0,8,3,3);

        std::cout << R_input << std::endl;
    }
    */
    my_functor(int Vert_num, Eigen::MatrixXd T): Functor<double>(18,Vert_num)
    {
        /*
        this -> R_input = Matrix<double, Dynamic, Dynamic>::Zero(3, 3);
        this -> U_input = Matrix<double, Dynamic, Dynamic>::Zero(3, 3);
        this -> F_input = Matrix<double, Dynamic, Dynamic>::Zero(3, 3);
        */
        this -> R_input = T.block(0,3,3,3);
        this -> U_input = T.block(0,8,3,3);
        this -> F_input = T.block(0,0,3,3);
        this -> Vert_num = Vert_num;
    }
    int operator()(const Eigen::VectorXd &X, Eigen::VectorXd &f_val) const
    {
        Eigen::MatrixXd R_opt, U_opt;
        Eigen::Matrix<double, Dynamic , 1> F_tmp(18);


        F_tmp = X;

        R_opt.resize(3,3);
        U_opt.resize(3,3);

        // set R_opt
        R_opt.row(0) << F_tmp(0), F_tmp(1), F_tmp(2);
        R_opt.row(1) << F_tmp(3), F_tmp(4), F_tmp(5);
        R_opt.row(2) << F_tmp(6), F_tmp(7), F_tmp(8);

        // set U_opt
        U_opt.row(0) << F_tmp(9), F_tmp(10), F_tmp(11);
        U_opt.row(1) << F_tmp(12), F_tmp(13), F_tmp(14);
        U_opt.row(2) << F_tmp(15), F_tmp(16), F_tmp(17);

        // std::cout << R_opt << std::endl;

        f_val(0) = (R_opt - this-> R_input).norm() + (U_opt - this-> U_input).norm() + (R_opt*U_opt - this-> F_input).norm();
        for(int i=1;i<Vert_num;i++)
        {
            f_val(i) = 0;
        }

        return 0;
    }
    /*
    int df(const Eigen::MatrixXd &X, Eigen::MatrixXd &f_der) const
    {
        // jacobian matrix
        return 0;
    }

    float getError(const Eigen::VectorXf &x, float &maxError)
    {

        return result;
    }
    */
};


class cloth_calc
{

public:
    cloth_calc(std::string Cloth_Template, std::string Cloth_Reference, std::string Cloth_Base );
    ~cloth_calc();

    void cloth_init_vert();
    void cloth_init_neighbor();
    void cloth_map_neighbor(int MODE);
    void cloth_map_adjacent();
    void cloth_map_neighbor_kdTree(double Per);

    void cloth_vec();
    void cloth_eig_2D();
    void cloth_eig_3D();
    void cloth_eig_assemble(Eigen::MatrixXd U);
    void cloth_stretchTensor_2D();
    void cloth_stretchTensor_3D(Eigen::MatrixXd Eigenval, Eigen::MatrixXd Eigenvec);
    void cloth_stretchTensor_neighbor();
    void cloth_stretchTensor_assemble(Eigen::MatrixXd U, std::map<int, std::vector<int>> MapNeighbor);
    void cloth_stretchTensor_CCM(Eigen::MatrixXd U, Eigen::MatrixXd Eigenval, std::map<int, std::vector<int>> MapNeighbor);
    void cloth_stretchTensor_kdTree(Eigen::MatrixXd U, double Per);
    void cloth_displGrad_2D();

    // USING NEIGHBOR
    void cloth_eig_neighbor(std::map<int, std::vector<int>> MapNeighbor);
    void cloth_eig_neighbor2x(std::map<int, std::vector<int>> MapNeighbor);
    void cloth_eig_neighbor3x(std::map<int, std::vector<int>> MapNeighbor);
    void cloth_eig_neighbor4x(std::map<int, std::vector<int>> MapNeighbor);

    // USING KD-TREE
    void cloth_eig_kdTree(double Per);

    // Wrinkel vector field v
    void cloth_wrink_vec_field(Eigen::MatrixXd Eigenval, Eigen::MatrixXd Eigenvec);

    // Stretching Tensor
    void cloth_velGrad_3D(Eigen::MatrixXd F_CT, Eigen::MatrixXd F_CR, double deltaT);
    void cloth_velGrad_assemble(Eigen::MatrixXd VelGrad, double Per);
    void cloth_velGrad_normalize(Eigen::MatrixXd VelGrad);

    // Optimation Process
    void cloth_rotationTensor(Eigen::MatrixXd F, Eigen::MatrixXd U);
    void cloth_translationVec(Eigen::MatrixXd R, std::map<int, std::vector<int>> MapNeighbor);
    void cloth_transformationMat(Eigen::MatrixXd R, Eigen::MatrixXd t);
    void cloth_update(Eigen::MatrixXd R, Eigen::MatrixXd t);
    void cloth_WriteVerts(Eigen::MatrixXd verts, const std::string &  ifileName);

    void cloth_ReadTransformationMat(std::string Transformation, std::string Frequency);
    void cloth_Opt(Eigen::MatrixXd T);

    // COLLORMAP
    void cloth_vec_normalize(Eigen::MatrixXd Eigenval, int dim);
    void cloth_WriteColor(Eigen::MatrixXd color, const std::string &  ifileName);



    void test(Eigen::MatrixXd T);

    std::map<int, std::vector<int>> GetMapNeighbor();
    std::map<int, std::vector<int>> GetMapNeighbor2x();
    std::map<int, std::vector<int>> GetMapNeighbor3x();
    std::map<int, std::vector<int>> GetMapNeighbor4x();
    std::map<int, std::vector<size_t>> GetMapNeighborKdTree();
    std::map<int, std::vector<int>> GetMapAdjacent();

    Eigen::MatrixXd GetEigval();
    Eigen::MatrixXd GetEigvec();
    Eigen::MatrixXd GetStretchTensor_2D();
    Eigen::MatrixXd GetStretchTensor_3D();
    Eigen::MatrixXd GetStretchTensorAsemmble();
    Eigen::MatrixXd GetStretchTensorMap();
    Eigen::MatrixXd GetStretchTensorFreq();
    Eigen::MatrixXd GetDefoGrad();
    Eigen::MatrixXd GetEigval_3D();
    Eigen::MatrixXd GetEigvec_3D();
    Eigen::MatrixXd GetEigval_assemble();
    Eigen::MatrixXd GetEigvec_assemble();
    Eigen::MatrixXd GetEigval_neighbor();
    Eigen::MatrixXd GetEigvec_neighbor();
    Eigen::MatrixXd GetEigval_neighbor2x();
    Eigen::MatrixXd GetEigvec_neighbor2x();
    Eigen::MatrixXd GetEigval_neighbor3x();
    Eigen::MatrixXd GetEigvec_neighbor3x();
    Eigen::MatrixXd GetEigval_neighbor4x();
    Eigen::MatrixXd GetEigvec_neighbor4x();
    Eigen::MatrixXd GetEigval_neighborKdTree();
    Eigen::MatrixXd GetEigvec_neighborKdTree();
    Eigen::MatrixXd GetEigval_norm_dir1();
    Eigen::MatrixXd GetEigval_norm_dir2();
    Eigen::MatrixXd GetEigval_norm_dir3();
    Eigen::MatrixXd GetWrinkVecField();
    Eigen::MatrixXd GetWrinkVecField_norm();
    Eigen::MatrixXd GetRotationTensor();
    Eigen::MatrixXd GetRotationTensorOpt();
    Eigen::MatrixXd GetTranslationVec();
    Eigen::MatrixXd GetTransformationMat();
    Eigen::MatrixXd GetVertsUpdate();
    Eigen::MatrixXd GetStrTensor();
    Eigen::MatrixXd GetRotTensor();
    Eigen::MatrixXd GetStrTensorAsemmble();
    Eigen::MatrixXd GetStrTensor_norm_dir1();


private:
    std::string CT;
    std::string CR;
    std::string BS;
    ply_module* _plyModuleT;
    ply_module* _plyModuleR;
    ply_module* _plyModule;
    ply_module* plyUpdate;

    std::map<int, std::vector<int>> mapNeighbor;
    std::map<int, std::vector<int>> mapNeighbor2x;
    std::map<int, std::vector<int>> mapNeighbor3x;
    std::map<int, std::vector<int>> mapNeighbor4x;
    std::map<int, std::vector<int>> mapAdjacent;
    std::map<int, std::vector<size_t>> mapNeighborKdTree;

    trimesh::TriMesh *_plyMeshT;
    trimesh::TriMesh *_plyMeshR;
    trimesh::TriMesh *_plyMesh;


    Eigen::MatrixXi faces;
    Eigen::MatrixXd vertsT, vertsR, verts;
    Eigen::MatrixXd vertsT_cog;
    Eigen::MatrixXd vertsR_cog;
    Eigen::MatrixXd vertsUpdate;

    // Rohmer et al.
    Eigen::MatrixXd VecT, VecR;
    Eigen::MatrixXd F;
    Eigen::MatrixXd Eigval_2D, Eigvec_2D;
    Eigen::MatrixXd Eigval_3D, Eigvec_3D;
    Eigen::MatrixXd Eigval_assemble, Eigvec_assemble;
    Eigen::MatrixXd U_2D;
    Eigen::MatrixXd H_2D;
    Eigen::MatrixXd U_3D;
    Eigen::MatrixXd U_3D_assem;
    Eigen::MatrixXd U_map;
    Eigen::MatrixXd Freq;

    Eigen::MatrixXd F_der;
    Eigen::MatrixXd F_inv;
    Eigen::MatrixXd L;
    Eigen::MatrixXd D;
    Eigen::MatrixXd W;
    Eigen::MatrixXd D_assem;
    Eigen::MatrixXd D_norm_dir1;
    Eigen::MatrixXd D_norm_dir2;
    Eigen::MatrixXd D_norm_dir3;

    //
    Eigen::MatrixXd Eigval_neighbor, Eigvec_neighbor;
    Eigen::MatrixXd Eigval_neighbor2x, Eigvec_neighbor2x;
    Eigen::MatrixXd Eigval_neighbor3x, Eigvec_neighbor3x;
    Eigen::MatrixXd Eigval_neighbor4x, Eigvec_neighbor4x;
    Eigen::MatrixXd Eigval_neighborKdTree, Eigvec_neighborKdTree;
    Eigen::MatrixXd U_neighbor;
    Eigen::MatrixXd H_neighbor;

    //
    Eigen::MatrixXd v;
    Eigen::MatrixXd v_norm;

    //
    Eigen::MatrixXd R;
    Eigen::MatrixXd t;
    Eigen::MatrixXd T;
    Eigen::MatrixXd R_opt;

    //
    Eigen::MatrixXd Eigval_norm_dir1, Eigval_norm_dir2, Eigval_norm_dir3;
};

#endif // CLOTH_CALC_H
