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
    int Vec_index = 0;
    for(int i=0; i<faces.rows(); i++)
    {
            // FOR REFERENCE
            VecR.row(Vec_index)   << (vertsR.row(faces(i,0)) - vertsR.row(faces(i,1))), // vector at vertex 0
                                     (vertsR.row(faces(i,0)) - vertsR.row(faces(i,2)));
            VecR.row(Vec_index+1) << (vertsR.row(faces(i,1)) - vertsR.row(faces(i,0))), // vector at vertex 1
                                     (vertsR.row(faces(i,1)) - vertsR.row(faces(i,2)));
            VecR.row(Vec_index+2) << (vertsR.row(faces(i,2)) - vertsR.row(faces(i,0))), // vector at vertex 2
                                     (vertsR.row(faces(i,2)) - vertsR.row(faces(i,1)));

            // FOR TEMPLATE
            VecT.row(Vec_index)   << (vertsT.row(faces(i,0)) - vertsT.row(faces(i,1))), // vector at vertex 0
                                     (vertsT.row(faces(i,0)) - vertsT.row(faces(i,2)));
            VecT.row(Vec_index+1) << (vertsT.row(faces(i,1)) - vertsT.row(faces(i,0))), // vector at vertex 1
                                     (vertsT.row(faces(i,1)) - vertsT.row(faces(i,2)));
            VecT.row(Vec_index+2) << (vertsT.row(faces(i,2)) - vertsT.row(faces(i,0))), // vector at vertex 2
                                     (vertsT.row(faces(i,2)) - vertsT.row(faces(i,1)));

            Vec_index = Vec_index+3;
    }

    // initialize the matrix to store the eigenvalue and eigenvector
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> Eigval_sq(faces.rows()*3,2);
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> Eigvec_sq(faces.rows()*6,2);

    int Eigval_index = 0;
    int Eigvec_index = 0;
    for(int i=0; i<faces.rows()*3; i++)
    {
        // compute the transformation matrix (Transl)
        // T = [u1, u2] * [u1_, u2_]^-1
        // we have here 3D triangles, thus we need to transform at first to 2D triangles using affine transformation
        Eigen::MatrixXd Transf((Eigen::Map<Eigen::Matrix<double,3,2> >(VecT.row(i).data())).block<2,2>(0,0) * ((Eigen::Map<Eigen::Matrix<double,3,2> >(VecR.row(i).data())).block<2,2>(0,0)).inverse());

        // compute the eigenvalues and eigenvectors of transformation matrix and save it to eigval and eigvec
        // U^2 = T^transpose * T
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix2d> solv(Transf.transpose() * Transf);

        // we transpose and save the vector and matrix
        Eigval_sq.row(Eigval_index)   << (solv.eigenvalues()).transpose();
        Eigvec_sq.block(Eigvec_index,0,2,2) << (solv.eigenvectors()).transpose();

        Eigval_index = Eigval_index+1;
        Eigvec_index = Eigvec_index+2;
    }

    // this is for debug
    // std::ofstream outfile0("../doc_discussion/debug/Eigval_sq.txt");
    // outfile0<< Eigval_sq <<std::endl;
    // outfile0.close();
    // std::ofstream outfile1("../doc_discussion/debug/Eigvec_sq.txt");
    // outfile1<< Eigvec_sq <<std::endl;
    // outfile1.close();

}

void cloth_calc::cloth_defo(Eigen::MatrixXd Eigval_sq, Eigen::MatrixXd Eigvec_sq)
{
    // initialize the matrix to store the stretch tensor
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> Defo(faces.rows()*3*2,2);
    int Defo_index = 0;
    int Eigval_index = 0;
    int Eigvec_index = 0;

    for(int i=0; i<faces.rows()*3*3; i=i+3)
    {
        /*
           Defo = [
                    defo_el1_vert1 defo_el1_vert1
                    defo_el1_vert1 defo_el1_vert1

                    defo_el1_vert2 defo_el1_vert2
                    defo_el1_vert2 defo_el1_vert2
                    ]
        */
        // it is important to get the squart root before calculate the stretch tensor
        Defo.block(Defo_index,0,2,2) << (sqrt(Eigval_sq(Eigval_index,0))*(Eigvec_sq.row(Eigvec_index)).transpose()*Eigvec_sq.row(Eigvec_index)) + (sqrt(Eigval_sq(Eigval_index,1))*(Eigvec_sq.row(Eigvec_index+1)).transpose()*Eigvec_sq.row(Eigvec_index+1));

        Defo_index = Defo_index+2;
        Eigval_index = Eigval_index+1;
        Eigvec_index = Eigvec_index+2;
    }

    // for debug
    // std::ofstream outfile6("../doc_discussion/debug/Defo.txt");
    // outfile6<< Defo <<std::endl;
    // outfile6.close();
    // std::cout <<Defo.rows() << std::endl;
}

void cloth_calc::cloth_displ()
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
     *        el1_vet1_v1_x el1_vet1_v1_y el1_vet1_v1_z  el1_vet1_v2_x el1_vet1_v2_y el1_vet1_v2_z
     *        el1_vet2_v1_x el1_vet2_v1_y el1_vet2_v1_z  el1_vet2_v2_x el1_vet2_v2_y el1_vet2_v2_z
     *        el1_vet3_v1_x el1_vet3_v1_y el1_vet3_v1_z  el1_vet3_v2_x el1_vet3_v2_y el1_vet3_v2_z
     *      ]
     */

    // get the verts index of each triangles el=i
    // faces(i,j) is the ith triangle of the vertex j

    int Vec_index = 0;
    for(int i=0; i<faces.rows(); i++)
    {
            // FOR REFERENCE
            VecR.row(Vec_index)   << (vertsR.row(faces(i,0)) - vertsR.row(faces(i,1))), // vector at vertex 0
                                     (vertsR.row(faces(i,0)) - vertsR.row(faces(i,2)));
            VecR.row(Vec_index+1) << (vertsR.row(faces(i,1)) - vertsR.row(faces(i,0))), // vector at vertex 1
                                     (vertsR.row(faces(i,1)) - vertsR.row(faces(i,2)));
            VecR.row(Vec_index+2) << (vertsR.row(faces(i,2)) - vertsR.row(faces(i,0))), // vector at vertex 2
                                     (vertsR.row(faces(i,2)) - vertsR.row(faces(i,1)));

            // FOR TEMPLATE
            VecT.row(Vec_index)   << (vertsT.row(faces(i,0)) - vertsT.row(faces(i,1))), // vector at vertex 0
                                     (vertsT.row(faces(i,0)) - vertsT.row(faces(i,2)));
            VecT.row(Vec_index+1) << (vertsT.row(faces(i,1)) - vertsT.row(faces(i,0))), // vector at vertex 1
                                     (vertsT.row(faces(i,1)) - vertsT.row(faces(i,2)));
            VecT.row(Vec_index+2) << (vertsT.row(faces(i,2)) - vertsT.row(faces(i,0))), // vector at vertex 2
                                     (vertsT.row(faces(i,2)) - vertsT.row(faces(i,1)));


            Vec_index = Vec_index+3;
    }

    // initialize the matrix to store the transformation matrix
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> Displ(faces.rows()*6,2);
    int Displ_index = 0;

    for(int i=0; i<faces.rows()*3; i++)
    {
        // compute the transformation matrix (Transl)
        // T = [u1, u2] * [u1_, u2_]^-1
        // we have here 3D triangles, thus we need to transform at first to 2D triangles using affine transformation
        Eigen::MatrixXd Transf((Eigen::Map<Eigen::Matrix<double,3,2> >(VecT.row(i).data())).block<2,2>(0,0) * ((Eigen::Map<Eigen::Matrix<double,3,2> >(VecR.row(i).data())).block<2,2>(0,0)).inverse());
        Displ.block(Displ_index,0,2,2) << Transf;

        Displ_index = Displ_index+2;
    }

    // this is for debug
    // std::ofstream outfile0("../doc_discussion/debug/Displ.txt");
    // outfile0<< Displ <<std::endl;
    // outfile0.close();

}

Eigen::MatrixXd cloth_calc::GetEigval()
{
    return this -> Eigval_sq;
}

Eigen::MatrixXd cloth_calc::GetEigvec()
{
    return this -> Eigvec_sq;
}

Eigen::MatrixXd cloth_calc::GetDefo()
{
    return this -> Defo;
}

Eigen::MatrixXd cloth_calc::GetDispl()
{
    return this -> Displ;
}


cloth_calc::~cloth_calc()
{

}
