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
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> VecR(faces.rows()*3,6);
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> VecT(faces.rows()*3,6);
    /*
     *Vec = [
     *        el1_vet1_v1_x el1_vet1_v1_y el1_vet1_v1_z el1_vet1_v2_x el1_vet1_v2_y el1_vet1_v2_z
     *        el1_vet2_v1_x el1_vet2_v1_y el1_vet2_v1_z el1_vet2_v2_x el1_vet2_v2_y el1_vet2_v2_z
     *        el1_vet3_v1_x el1_vet3_v1_y el1_vet3_v1_z el1_vet3_v2_x el1_vet3_v2_y el1_vet3_v2_z
     *      ]
     */

    // get the verts index of each triangles el=i
    // faces(i,j) is the ith triangle of the vertex j
    // FOR REFERENCE
    int k = 0;
    for(int i=0; i<faces.rows(); i++)
    {
            VecR.row(k) <<   (vertsR.row(faces(i,0)) - vertsR.row(faces(i,1))), // vector at vertex 0
                             (vertsR.row(faces(i,0)) - vertsR.row(faces(i,2)));
            VecR.row(k+1) << (vertsR.row(faces(i,1)) - vertsR.row(faces(i,0))), // vector at vertex 1
                             (vertsR.row(faces(i,1)) - vertsR.row(faces(i,2)));
            VecR.row(k+2) << (vertsR.row(faces(i,2)) - vertsR.row(faces(i,0))), // vector at vertex 2
                             (vertsR.row(faces(i,2)) - vertsR.row(faces(i,1)));
            k = k+3;
    }
    // FOR TEMPLATE
    int j = 0;
    for(int i=0; i<faces.rows(); i++)
    {
            VecT.row(j) <<   (vertsT.row(faces(i,0)) - vertsT.row(faces(i,1))), // vector at vertex 0
                             (vertsT.row(faces(i,0)) - vertsT.row(faces(i,2)));
            VecT.row(j+1) << (vertsT.row(faces(i,1)) - vertsT.row(faces(i,0))), // vector at vertex 1
                             (vertsT.row(faces(i,1)) - vertsT.row(faces(i,2)));
            VecT.row(j+2) << (vertsT.row(faces(i,2)) - vertsT.row(faces(i,0))), // vector at vertex 2
                             (vertsT.row(faces(i,2)) - vertsT.row(faces(i,1)));
            j = j+3;
    }

    // this is for debug

    // initialize the matrix to store the eigenvalue and eigenvector
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> eigval(faces.rows()*3,2);
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> eigvec(faces.rows()*3,4);

    int l = 0;
    for(int i=0; i<faces.rows()*3; i++)
    {
        // compute the transformation matrix (Transl)
        // T = [u1, u2] * [u1_, u2_]^-1
        // we have here 3D triangles, thus we need to transform at first to 2D triangles using projecting
        Eigen::MatrixXd Transl((Eigen::Map<Eigen::Matrix<double,3,2> >(VecT.row(i).data())).block<2,2>(0,0) * ((Eigen::Map<Eigen::Matrix<double,3,2> >(VecR.row(i).data())).block<2,2>(0,0)).inverse());

        // compute the eigenvalues and eigenvectors of transformation matrix and save it to eigval and eigvec
        // U^2 = T^transpose * T
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix2d> solv(Transl.transpose() * Transl);

        eigval.row(l) << (solv.eigenvalues()).transpose();
        eigvec.row(l) << ((solv.eigenvectors()).col(0)).transpose(), ((solv.eigenvectors()).col(1)).transpose();
        l = l+1;
    }
    // this is for debug
    std::cout << eigvec << std::endl;

}

void cloth_calc::cloth_defo()
{

}

cloth_calc::~cloth_calc()
{

}
