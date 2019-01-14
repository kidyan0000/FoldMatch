#include "cloth_calc.h"

cloth_calc::cloth_calc()
{
    _plyModuleT = new ply_module();
    _plyModuleR = new ply_module();
    _plyModuleR->readPLY("../data/Template-1_0001.ply", true, true, true, true, true);
    _plyModuleT->readPLY("../data/Template-5_0005.ply", true, true, true, true, true);
}

void cloth_calc::cloth_eig()
{
    Eigen::MatrixXd vertsR, vertsT;

    if (_plyModuleT->getVertices().rows() != 0)
    {
        vertsT = _plyModuleT->getVertices();
    }
    if (_plyModuleR->getVertices().rows() != 0)
    {
        vertsR = _plyModuleR->getVertices();
    }
    if (_plyModuleR->getFaces().rows() != 0)
    {
        faces = _plyModuleR->getFaces();
    }

    // initialize the matrix to store the vector values of each triangles
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> VecR(faces.rows()*3,2);
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> VecT(faces.rows()*3,2);
    /*
     *Vec = [ index_e1_v1 index_e1_v2
     *        index_e1_v1 index_e1_v3
     *        index_e1_v2 index_e1_v3
     *                                ]
     */

    // FOR REFERENCE
    // get the verts index of each triangles i

    int k = 0;
    for(int i=0; i<faces.rows(); i++)
    {
            VecR.row(k) <<   (vertsR(faces(i,0)) - vertsR(faces(i,1))), // vector at vertex 0
                             (vertsR(faces(i,0)) - vertsR(faces(i,2)));
            VecR.row(k+1) << (vertsR(faces(i,1)) - vertsR(faces(i,0))), // vector at vertex 1
                             (vertsR(faces(i,1)) - vertsR(faces(i,2)));
            VecR.row(k+2) << (vertsR(faces(i,2)) - vertsR(faces(i,0))), // vector at vertex 2
                             (vertsR(faces(i,2)) - vertsR(faces(i,1)));
            k = k+3;
    }

    // FOR TEMPLATE
    int j = 0;
    for(int i=0; i<faces.rows(); i++)
    {
            VecT.row(j) <<   (vertsT(faces(i,0)) - vertsT(faces(i,1))), // vector at vertex 0
                             (vertsT(faces(i,0)) - vertsT(faces(i,2)));
            VecT.row(j+1) << (vertsT(faces(i,1)) - vertsT(faces(i,0))), // vector at vertex 1
                             (vertsT(faces(i,1)) - vertsT(faces(i,2)));
            VecT.row(j+2) << (vertsT(faces(i,2)) - vertsT(faces(i,0))), // vector at vertex 2
                             (vertsT(faces(i,2)) - vertsT(faces(i,1)));
            j = j+3;
    }


    // initialize the matrix to store the eigenvalue and eigenvector
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> eigval(faces.rows()*3,2);
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> eigvec(faces.rows()*3,4);

    int l = 0;
    for(int i=0; i<faces.rows()*3; i++)
    {
        // compute the transformation matrix
        // T = [u1, u2] * [u1_, u2_]^-1
        Eigen::MatrixXd Transl(Eigen::Map<Eigen::RowVectorXd>(VecT.row(i).data(), 2).transpose() * Eigen::Map<Eigen::RowVectorXd>(VecR.row(i).data(), 2));

        // compute the eigenvalues and eigenvectors of transformation matrix and save it to eigval and eigvec
        // U^2 = T^transpose * T
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix2d> solv(Transl.transpose() * Transl);

        eigval.row(l) << (solv.eigenvalues()).transpose();
        eigvec.row(l) << ((solv.eigenvectors()).col(0)).transpose(), ((solv.eigenvectors()).col(1)).transpose();
    }
}

void cloth_calc::cloth_defo()
{

}

cloth_calc::~cloth_calc()
{

}
