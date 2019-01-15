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

    Eigen::MatrixXd cloth_eig();
    void cloth_defo( Eigen::MatrixXd );
    void cloth_displ( Eigen::MatrixXd );

    void writeDefo();
    void writeDispl();

private:
    ply_module* _plyModuleR;
    ply_module* _plyModuleT;

    Eigen::MatrixXi faces;
    Eigen::MatrixXd Eig_sq, Defo, Displ;

};

#endif // CLOTH_CALC_H
