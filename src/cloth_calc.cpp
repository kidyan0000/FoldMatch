#include "cloth_calc.h"

cloth_calc::cloth_calc()
{
    _plyModuleT = new ply_module();
    _plyModuleR = new ply_module();
    _plyModuleR->readPLY("../data/Template-1_0001.ply", true, true, true, true, true);
    _plyModuleT->readPLY("../data/Template-5_0005.ply", true, true, true, true, true);
}

Eigen::MatrixXd cloth_calc::cloth_eig()
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

    // initialize the matrix to store the transformation matrix
    Eigen::MatrixXd Transl;
    // initialize the matrix to store the eigenvalue and eigenvector
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> Eig_sq(faces.rows()*9,2);

    int Eig_index = 0;
    for(int i=0; i<faces.rows()*3; i++)
    {
        // compute the transformation matrix (Transl)
        // T = [u1, u2] * [u1_, u2_]^-1
        // we have here 3D triangles, thus we need to transform at first to 2D triangles using affine transformation
        Eigen::MatrixXd Transl((Eigen::Map<Eigen::Matrix<double,3,2> >(VecT.row(i).data())).block<2,2>(0,0) * ((Eigen::Map<Eigen::Matrix<double,3,2> >(VecR.row(i).data())).block<2,2>(0,0)).inverse());

        // compute the eigenvalues and eigenvectors of transformation matrix and save it to eigval and eigvec
        // U^2 = T^transpose * T
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix2d> solv(Transl.transpose() * Transl);

        /*
           Eig_sq = [
                    eig_val1  eig_val2;
                    eig_vec1  eig_vec2;
                    eig_vec3  eig_vec4;
                    ]
        */
        Eig_sq.row(Eig_index)   << (solv.eigenvalues()).transpose();
        Eig_sq.row(Eig_index+1) << ((solv.eigenvectors()).col(0)).transpose();
        Eig_sq.row(Eig_index+2) << ((solv.eigenvectors()).col(1)).transpose();

        Eig_index = Eig_index+3;
    }


    // this is for debug
    std::ofstream outfile0("../doc_discussion/debug/vertsT.txt");
    outfile0<< vertsT <<std::endl;
    outfile0.close();
    std::ofstream outfile1("../doc_discussion/debug/vertsR.txt");
    outfile1<< vertsR <<std::endl;
    outfile1.close();
    std::ofstream outfile2("../doc_discussion/debug/VecT.txt");
    outfile2<< VecT <<std::endl;
    outfile2.close();
    std::ofstream outfile3("../doc_discussion/debug/VecR.txt");
    outfile3<< VecT <<std::endl;
    outfile3.close();
    std::ofstream outfile4("../doc_discussion/debug/Transl.txt");
    outfile4<< Transl.data() <<std::endl;
    outfile4.close();
    std::ofstream outfile5("../doc_discussion/debug/Eig_sq.txt");
    outfile5<< Eig_sq <<std::endl;
    outfile5.close();
    // std::cout << "read the element 1 " << (faces.row(0)) << std::endl;
    // std::cout << "read the vertex 1 for reference " << vertsR.row(faces(0,0)) << std::endl;
    // std::cout << "read the vertex 2 for reference " << vertsR.row(faces(0,1)) << std::endl;
    // std::cout << "read the vertex 3 for reference " << vertsR.row(faces(0,2)) << std::endl;
    // std::cout << "read the vertex 1 for template " << vertsT.row(faces(0,0)) << std::endl;
    // std::cout << "read the vertex 2 for template " << vertsT.row(faces(0,1)) << std::endl;
    // std::cout << "read the vertex 3 for template " << vertsT.row(faces(0,2)) << std::endl;
    // std::cout << "read the vector for reference " << VecR.row(1) << std::endl;
    // std::cout << "read the vector for template " << VecT.row(1) << std::endl;
    // std::cout << "read the transformation matrix " << (Eigen::Map<Eigen::Matrix<double,3,2> >(VecT.row(1).data())).block<2,2>(0,0) * ((Eigen::Map<Eigen::Matrix<double,3,2> >(VecR.row(1).data())).block<2,2>(0,0)).inverse() << std::endl;
    // std::cout << "read the eigenvalues " << Eig_sq.row(0) << std::endl;
    // std::cout << "read the eigenvectors " << Eig_sq.row(1) << Eig_sq.row(2) << std::endl;

    return Eig_sq;

}

void cloth_calc::cloth_defo(Eigen::MatrixXd Eig_sq)
{
    /*
       Eig_sq = [
                eig_val1  eig_val2;
                eig_vec1  eig_vec2;
                eig_vec3  eig_vec4;
                ]
    */

    // initialize the matrix to store the Green deformation tensor
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> Defo(faces.rows()*3*2,2);
    int Defo_index = 0;
    for(int i=0; i<faces.rows(); i++)
    {
        /*
           Defo = [
                    defo_el1_vert1 defo_el1_vert1
                    defo_el1_vert1 defo_el1_vert1

                    defo_el1_vert2 defo_el1_vert2
                    defo_el1_vert2 defo_el1_vert2
                    ]
        */

        Defo.block(Defo_index,0,2,2) << sqrt(Eig_sq(i,0))*(Eig_sq.row(i+1)).transpose()*Eig_sq.row(i+1) + sqrt(Eig_sq(i,1))*(Eig_sq.row(i+2)).transpose()*Eig_sq.row(i+2);
        Defo_index = Defo_index+2;
    }

    // for debug
    std::ofstream outfile0("../doc_discussion/debug/Defo.txt");
    outfile0<< Eig_sq <<std::endl;
    outfile0.close();
    std::cout << Defo.rows() << std::endl;
}

void cloth_calc::cloth_displ(Eigen::MatrixXd Eig_sq)
{
    /*
       Eig_sq = [
                eig_val1  eig_val2;
                eig_vec1  eig_vec2;
                eig_vec3  eig_vec4;
                ]
    */

    // initialize the matrix to store the displacement gradient
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic,Eigen::RowMajor> Defo(faces.rows()*3*2,2);
    int Defo_index = 0;
    for(int i=0; i<faces.rows(); i++)
    {
        /*
           Defo = [
                    defo_el1_vert1 defo_el1_vert1
                    defo_el1_vert1 defo_el1_vert1

                    defo_el1_vert2 defo_el1_vert2
                    defo_el1_vert2 defo_el1_vert2
                    ]
        */

        Defo.block(Defo_index,0,2,2) << sqrt(Eig_sq(i,0))*(Eig_sq.row(i+1)).transpose()*Eig_sq.row(i+1) + sqrt(Eig_sq(i,1))*(Eig_sq.row(i+2)).transpose()*Eig_sq.row(i+2);
        Defo_index = Defo_index+2;
    }

    // for debug
    std::ofstream outfile0("../doc_discussion/debug/Defo.txt");
    outfile0<< Eig_sq <<std::endl;
    outfile0.close();
    std::cout << Defo.rows() << std::endl;

}

cloth_calc::~cloth_calc()
{

}
