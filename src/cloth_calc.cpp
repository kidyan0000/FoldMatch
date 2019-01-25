#include "cloth_calc.h"

cloth_calc::cloth_calc()
{
    _plyModuleT = new ply_module();
    _plyModuleR = new ply_module();
    _plyModuleR->readPLY("../data/Template-1_0001.ply", true, true, true, true, true);
    _plyModuleT->readPLY("../data/Template-5_0005.ply", true, true, true, true, true);
}

void cloth_calc::cloth_vec()
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
    // here is ColMajor
    this -> VecR.resize(6,faces.rows()*3);
    this -> VecT.resize(6,faces.rows()*3);
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
    int Vec_num = faces.rows();

    for(int i=0; i<Vec_num; i++)
    {
            // FOR REFERENCE
            this -> VecR.col(Vec_index).transpose()   << (vertsR.row(faces(i,0)) - vertsR.row(faces(i,1))), // vector at vertex 0
                                     (vertsR.row(faces(i,0)) - vertsR.row(faces(i,2)));
            this -> VecR.col(Vec_index+1).transpose() << (vertsR.row(faces(i,1)) - vertsR.row(faces(i,0))), // vector at vertex 1
                                     (vertsR.row(faces(i,1)) - vertsR.row(faces(i,2)));
            this -> VecR.col(Vec_index+2).transpose() << (vertsR.row(faces(i,2)) - vertsR.row(faces(i,0))), // vector at vertex 2
                                     (vertsR.row(faces(i,2)) - vertsR.row(faces(i,1)));

            // FOR TEMPLATE
            this -> VecT.col(Vec_index).transpose()   << (vertsT.row(faces(i,0)) - vertsT.row(faces(i,1))), // vector at vertex 0
                                     (vertsT.row(faces(i,0)) - vertsT.row(faces(i,2)));
            this -> VecT.col(Vec_index+1).transpose() << (vertsT.row(faces(i,1)) - vertsT.row(faces(i,0))), // vector at vertex 1
                                     (vertsT.row(faces(i,1)) - vertsT.row(faces(i,2)));
            this -> VecT.col(Vec_index+2).transpose() << (vertsT.row(faces(i,2)) - vertsT.row(faces(i,0))), // vector at vertex 2
                                     (vertsT.row(faces(i,2)) - vertsT.row(faces(i,1)));


            Vec_index = Vec_index+3;
    }
}

void cloth_calc::cloth_eig()
{
    cloth_vec();

    // initialize the matrix to store the eigenvalue and eigenvector
    this -> Eigval_sq.resize(faces.rows()*6,1);
    this -> Eigvec_sq.resize(faces.rows()*6,2);

    int Eig_index = 0;
    int Eig_num = faces.rows()*3;

    for(int i=0; i<Eig_num; i++)
    {
        // compute the transformation matrix (Transl)
        // T = [u1, u2] * [u1_, u2_]^-1
        // we have here 3D triangles, thus we need to transform at first to 2D triangles using affine transformation
        Eigen::MatrixXd Transf((Eigen::Map<Eigen::Matrix<double,3,2> >(this -> VecT.col(i).data())).block<2,2>(0,0) * ((Eigen::Map<Eigen::Matrix<double,3,2> >(this -> VecR.col(i).data())).block<2,2>(0,0)).inverse());

        // compute the eigenvalues and eigenvectors of transformation matrix and save it to eigval and eigvec
        // U^2 = T^transpose * T
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix2d> solv(Transf.transpose() * Transf);

        // we save the vector and matrix
        this -> Eigval_sq.block(Eig_index,0,2,1) << solv.eigenvalues();
        this -> Eigvec_sq.block(Eig_index,0,2,2) << solv.eigenvectors();

        Eig_index = Eig_index+2;
    }


}

void cloth_calc::cloth_defo()
{
    cloth_eig();

    // initialize the matrix to store the stretch tensor
    this -> Defo.resize(faces.rows()*6,2);

    int Defo_num = faces.rows()*6;

    for(int i=0; i<Defo_num; i=i+2)
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
        // U = sqrt(/lamdba_1)*v1*v1^T + sqrt(/lamdba_2)*v2*v2^T
        Defo.block(i,0,2,2) << ( sqrt(this -> Eigval_sq(i,0)) * (this -> Eigvec_sq.block(i,0,2,2).col(0)) * this -> Eigvec_sq.block(i,0,2,2).col(0).transpose() ) + ( sqrt(this -> Eigval_sq(i+1,0)) * (this -> Eigvec_sq.block(i,0,2,2).col(1)) * this -> Eigvec_sq.block(i,0,2,2).col(1).transpose());
    }
}

void cloth_calc::cloth_displ()
{
    cloth_vec();

    // initialize the matrix to store the transformation matrix
    this -> Displ.resize(faces.rows()*6,2);

    int Displ_index = 0;
    int Displ_num = faces.rows()*3;

    for(int i=0; i<Displ_num; i++)
    {
        // compute the transformation matrix (Transf)
        // T = [u1, u2] * [u1_, u2_]^-1
        // we have here 3D triangles, thus we need to transform at first to 2D triangles using affine transformation
        Eigen::MatrixXd Transf((Eigen::Map<Eigen::Matrix<double,3,2> >(this -> VecT.col(i).data())).block<2,2>(0,0) * ((Eigen::Map<Eigen::Matrix<double,3,2> >(this -> VecR.col(i).data())).block<2,2>(0,0)).inverse());

        // H = F-1
        this -> Displ.block(Displ_index,0,2,2) << Transf - Eigen::MatrixXd::Identity(2,2);
        Displ_index = Displ_index+2;
    }
}


void cloth_calc::cloth_eig_neighbor()
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

    // initialize the points set P(reference) and Q(template)
    Eigen::MatrixXd P, Q;
    Eigen::MatrixXd H;

    // initialize the eigenvalues and eigenvectors
    this -> Eigval_sq_neighbor.resize(faces.rows()*9,1);
    this -> Eigvec_sq_neighbor.resize(faces.rows()*9,3);

    // read for all vertex
    int Vec_num = faces.rows();
    for(int i=0; i<Vec_num; i++)
    {
        // for reference
        vertsR.row(faces(i,0)); // the first vertex of i-th element
        // need to get the neighborred this vertex

        vertsR.row(faces(i,1)); // the second vertex of i-th element

        vertsR.row(faces(i,2)); // the third vertex of i-th element

        // for template
        vertsT.row(faces(i,0)); // the first vertex of i-th element
        // need to get the neighborred this vertex

        vertsT.row(faces(i,1)); // the second vertex of i-th element

        vertsT.row(faces(i,2)); // the third vertex of i-th element

        // we consider here the covariance matrix H as the approximated deformation gradient F
        // KABSCH ALGORITHM
        //  1.Translation to origin
        //  2.Computation of the covariance matrix
        //      H = P^T * Q
        //  3.Computation of the singular value decomposition
        //      H = U * S * V^T
        //      def: U, V - rotation informations
        //      def:    S - stretch informations
    }

    // now we get all the information and to calculate the eigenvalues and eigenvector


    int Eig_index = 0;
    int Eig_num = faces.rows()*3;

    for(int i=0; i<Eig_num; i++)
    {
        // compute the eigenvalues and eigenvectors of transformation matrix and save it to eigval and eigvec
        // U^2 = T^transpose * T
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv(H.transpose() * H);

        // we save the vector and matrix
        this -> Eigval_sq_neighbor.block(Eig_index,0,3,1) << solv.eigenvalues();
        this -> Eigvec_sq_neighbor.block(Eig_index,0,3,3) << solv.eigenvectors();

        Eig_index = Eig_index+3;
    }

}

void cloth_calc::cloth_defo_neighbor()
{
    cloth_eig_neighbor();

    this -> Defo_neighbor.resize(faces.rows()*9,3);

    int Defo_num = faces.rows()*9;

    for(int i=0; i<Defo_num; i=i+3)
    {
        // it is important to get the squart root before calculate the stretch tensor
        // U = sqrt(/lamdba_1)*v1*v1^T + sqrt(/lamdba_2)*v2*v2^T + sqrt(/lamdba_3)*v3*v3^T
        Defo_neighbor.block(i,0,3,3) << ( sqrt(this -> Eigval_sq_neighbor(i,0)) * (this -> Eigvec_sq_neighbor.block(i,0,3,3).col(0)) * this -> Eigvec_sq_neighbor.block(i,0,3,3).col(0).transpose() ) + ( sqrt(this -> Eigval_sq_neighbor(i+1,0)) * (this -> Eigvec_sq_neighbor.block(i,0,3,3).col(1)) * this -> Eigvec_sq_neighbor.block(i,0,3,3).col(1).transpose()) + ( sqrt(this -> Eigval_sq_neighbor(i+2,0)) * (this -> Eigvec_sq_neighbor.block(i,0,3,3).col(2)) * this -> Eigvec_sq_neighbor.block(i,0,3,3).col(2).transpose());
    }
}



void cloth_calc::cloth_vec_normalize(Eigen::MatrixXd Eigval, int dim)
{
    int Eig_num = Eigval.size();
    int Eig_index = 0;
    Eigval_norm_dir1.resize(faces.size(),1);
    Eigval_norm_dir2.resize(faces.size(),1);
    Eigval_norm_dir3.resize(faces.size(),1);

    if(dim==2)
    {
        // this is to normlize the eigenvalue in order to smooth the solution space
        Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<2> >Eigval_norm_dir1(Eigval.data()  ,faces.size(),1);
        Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<2> >Eigval_norm_dir2(Eigval.data()+1,faces.size(),1);

        this -> Eigval_norm_dir1 = Eigval_norm_dir1 / (Eigval_norm_dir1.maxCoeff() - Eigval_norm_dir1.minCoeff());
        this -> Eigval_norm_dir2 = Eigval_norm_dir2 / (Eigval_norm_dir2.maxCoeff() - Eigval_norm_dir2.minCoeff());
    }

    if(dim==3)
    {
        // this is to normlize the eigenvalue in order to smooth the solution space
        Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<3> >Eigval_norm_dir1(Eigval.data()  ,faces.size(),1);
        Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<3> >Eigval_norm_dir2(Eigval.data()+1,faces.size(),1);
        Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<3> >Eigval_norm_dir3(Eigval.data()+2,faces.size(),1);

        this -> Eigval_norm_dir1 = Eigval_norm_dir1 / (Eigval_norm_dir1.maxCoeff() - Eigval_norm_dir1.minCoeff());
        this -> Eigval_norm_dir2 = Eigval_norm_dir2 / (Eigval_norm_dir2.maxCoeff() - Eigval_norm_dir2.minCoeff());
        this -> Eigval_norm_dir3 = Eigval_norm_dir3 / (Eigval_norm_dir3.maxCoeff() - Eigval_norm_dir3.minCoeff());
    }
}


void cloth_calc::cloth_WriteColor(Eigen::MatrixXd Eigval_norm, const std::string &  ifileName)
{

    // set the color and write as ply files
    ply_module* plyColor;
    plyColor = new ply_module();

    plyColor -> setColors(Eigval_norm.cast<int>());
    plyColor -> writePLY(ifileName, true, false, false, false, false);

    // std::cout << color*256 << std::endl;
    // std::cout << (color*256).cast<int>() << std::endl;

}


Eigen::MatrixXd cloth_calc::GetVecR()
{
    return this -> VecR;
}

Eigen::MatrixXd cloth_calc::GetVecT()
{
    return this -> VecT;
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

Eigen::MatrixXd cloth_calc::GetEigval_norm_dir1()
{
    return this -> Eigval_norm_dir1;
}

Eigen::MatrixXd cloth_calc::GetEigval_norm_dir2()
{
    return this -> Eigval_norm_dir2;
}

Eigen::MatrixXd cloth_calc::GetEigval_norm_dir3()
{
    return this -> Eigval_norm_dir3;
}


void cloth_calc::test()
{
    const char *filename = "../data/bunny_bi.ply";
    trimesh::TriMesh *mymesh = trimesh::TriMesh::read(filename);
    if (!mymesh) {
        printf("Couldn't read mesh!\n");
        exit(1);
    }
    mymesh -> trimesh::TriMesh::need_neighbors();

}

cloth_calc::~cloth_calc()
{

}

