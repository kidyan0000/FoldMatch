#include "cloth_calc.h"

cloth_calc::cloth_calc(std::string Cloth_Template, std::string Cloth_Reference, std::string Cloth_Base )
{
    this -> CT = Cloth_Template;
    this -> CR = Cloth_Reference;
    this -> BS = Cloth_Base;

    _plyModuleT = new ply_module();
    _plyModuleR = new ply_module();
    _plyModule  = new ply_module();

    _plyModuleT->readPLY(CT, true, true, true, true, true);
    _plyModuleR->readPLY(CR, true, true, true, true, true);
    _plyModule->readPLY(BS, true, true, true, true, true);
}

void cloth_calc::cloth_init_vert()
{
    // this is just for vertice
    if (_plyModule->getFaces().rows() != 0)
    {
        faces = _plyModule->getFaces();
    }
    if (_plyModule->getVertices().rows() != 0)
    {
        verts = _plyModule->getVertices();
    }


    if (_plyModuleT->getVertices().rows() != 0)
    {
        vertsT = _plyModuleT->getVertices();
    }
    if (_plyModuleR->getVertices().rows() != 0)
    {
        vertsR = _plyModuleR->getVertices();
    }
}

void cloth_calc::cloth_init_neighbor()
{
    _plyMeshT = trimesh::TriMesh::read(CT);
    _plyMeshR = trimesh::TriMesh::read(CR);
    _plyMesh  = trimesh::TriMesh::read(BS);

    _plyMeshT -> trimesh::TriMesh::need_neighbors();
    _plyMeshR -> trimesh::TriMesh::need_neighbors();   
    _plyMesh  -> trimesh::TriMesh::need_neighbors();

}

void cloth_calc::cloth_vec()
{
    cloth_init_vert();

    // initialize the matrix to store the vector values of each triangles
    // here is ColMajor
    this -> VecT.resize(6,faces.rows()*3);
    this -> VecR.resize(6,faces.rows()*3);
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
            // FOR TEMPLATE
            this -> VecT.col(Vec_index).transpose()   << (vertsT.row(faces(i,1)) - vertsT.row(faces(i,0))), // vector at vertex 0
                                                         (vertsT.row(faces(i,2)) - vertsT.row(faces(i,0)));

            this -> VecT.col(Vec_index+1).transpose() << (vertsT.row(faces(i,0)) - vertsT.row(faces(i,1))), // vector at vertex 1
                                                         (vertsT.row(faces(i,2)) - vertsT.row(faces(i,1)));

            this -> VecT.col(Vec_index+2).transpose() << (vertsT.row(faces(i,0)) - vertsT.row(faces(i,2))), // vector at vertex 2
                                                         (vertsT.row(faces(i,1)) - vertsT.row(faces(i,2)));
            // FOR REFERENCE
            this -> VecR.col(Vec_index).transpose()   << (vertsR.row(faces(i,1)) - vertsR.row(faces(i,0))), // vector at vertex 0
                                                         (vertsR.row(faces(i,2)) - vertsR.row(faces(i,0)));

            this -> VecR.col(Vec_index+1).transpose() << (vertsR.row(faces(i,0)) - vertsR.row(faces(i,1))), // vector at vertex 1
                                                         (vertsR.row(faces(i,2)) - vertsR.row(faces(i,1)));

            this -> VecR.col(Vec_index+2).transpose() << (vertsR.row(faces(i,0)) - vertsR.row(faces(i,2))), // vector at vertex 2
                                                         (vertsR.row(faces(i,1)) - vertsR.row(faces(i,2)));




            Vec_index = Vec_index+3;
    }
}

void cloth_calc::cloth_eig_2D()
{
    cloth_vec();

    // initialize the matrix to store the eigenvalue and eigenvector
    this -> Eigval_2D.resize(faces.rows()*6,1);
    this -> Eigvec_2D.resize(faces.rows()*6,2);

    int Eig_index = 0;
    int Eig_num = faces.rows()*3;

    for(int i=0; i<Eig_num; i++)
    {
        // compute the transformation matrix (Transf)
        // T = [u1, u2] * [u1_, u2_]^-1
        // we have here 3D triangles, thus we need to transform at first to 2D triangles using affine transformation
        Eigen::MatrixXd Transf((Eigen::Map<Eigen::Matrix<double,3,2> >(this -> VecR.col(i).data())).block<2,2>(0,0) * ((Eigen::Map<Eigen::Matrix<double,3,2> >(this -> VecT.col(i).data())).block<2,2>(0,0)).inverse());

        // compute the eigenvalues and eigenvectors of transformation matrix and save it to eigval and eigvec
        // U^2 = T^transpose * T
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix2d> solv(Transf.transpose() * Transf);

        // we save the vector and matrix
        this -> Eigval_2D.block(Eig_index,0,2,1) << solv.eigenvalues().cwiseSqrt();
        this -> Eigvec_2D.block(Eig_index,0,2,2) << solv.eigenvectors();

        Eig_index = Eig_index+2;
    }


}

void cloth_calc::cloth_eig_3D()
{
    cloth_vec();

    // initialize the points set P(reference) and Q(template)
    Eigen::MatrixXd P, Q;
    // Eigen::MatrixXd H;

    // initialize the eigenvalues and eigenvectors
    this -> Eigval_3D.resize(faces.rows()*9,1);
    this -> Eigvec_3D.resize(faces.rows()*9,3);

    // initialize the trimesh
    cloth_init_neighbor();

    // we consider here the covariance matrix H as the approximated deformation gradient F
    // KABSCH ALGORITHM
    //  1.Translation to origin
    //  2.Computation of the covariance matrix
    //      H = P^T * Q -> F = P^T * Q
    //  3.Computation of the singular value decomposition
    //      H = U * S * V^T
    //      def: U, V - rotation informations
    //      def:    S - stretch informations

    int El_num = faces.rows();
    int Eig_index = 0;

    for(int El_index=0; El_index<El_num; El_index++) // for all elements
    {
        for(int Vert_index=0; Vert_index<3; Vert_index++) // for all vertice in each element
        {
            // face has the structure of face(i.j)
            // where i is the i-th element, j is the j-th vertex of i-th element

            // calculate the number of the nerghborred vertice of vertex i
            int Neighbor_num = _plyMesh -> trimesh::TriMesh::neighbors.at(faces(El_index,Vert_index)).size();

            P.resize(Neighbor_num, 3);
            Q.resize(Neighbor_num, 3);

            for(int Neighbor_index=0; Neighbor_index < Neighbor_num; Neighbor_index++) // for all neighbors in each vertex
            {

                // for template
                P.row(Neighbor_index) << vertsT.row(_plyMeshT -> trimesh::TriMesh::neighbors.at(faces(El_index, Vert_index)).at(Neighbor_index)) - vertsT.row(faces(El_index, Vert_index));
                // for reference
                Q.row(Neighbor_index) << vertsR.row(_plyMeshR -> trimesh::TriMesh::neighbors.at(faces(El_index, Vert_index)).at(Neighbor_index)) - vertsR.row(faces(El_index, Vert_index));

            }

            // Irving et al. - Invertible Finite Elements For Robust Simulation of Large Deformation
            Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv((P.transpose()*Q).transpose() * (P.transpose()*Q));

            this -> Eigval_3D.block(Eig_index,0,3,1) << solv.eigenvalues().cwiseSqrt();
            this -> Eigvec_3D.block(Eig_index,0,3,3) << solv.eigenvectors();

            P.resize(0,0);
            Q.resize(0,0);

            Eig_index = Eig_index+3;

        }
    }

    // this is for debug
    /*
    for(int neighbor_index=0; neighbor_index < _plyMeshR -> trimesh::TriMesh::neighbors.at(faces(0,0)).size(); neighbor_index++)
    {
         std::cout << vertsR.row(_plyMeshR -> trimesh::TriMesh::neighbors.at(faces(0,0)).at(neighbor_index)) << std::endl;
    }
    std::cout << Eigval_neighbor.rows() << std::endl;
    std::cout << Eigvec_neighbor.rows() << std::endl;
    */
}


void cloth_calc::cloth_defo_2D()
{
    cloth_eig_2D();

    // initialize the matrix to store the stretch tensor
    this -> Defo_2D.resize(faces.rows()*6,2);

    int Defo_num = faces.rows()*6;

    for(int Defo_index=0; Defo_index<Defo_num; Defo_index=Defo_index+2)
    {
        /*
           Defo = [
                    defo_el1_vert1 defo_el1_vert1
                    defo_el1_vert1 defo_el1_vert1

                    defo_el1_vert2 defo_el1_vert2
                    defo_el1_vert2 defo_el1_vert2
                    ]
        */

        // U = sqrt(/lamdba_1)*v1*v1^T + sqrt(/lamdba_2)*v2*v2^T
        Defo_2D.block(Defo_index,0,2,2) << ( (this -> Eigval_2D(Defo_index,0)) * (this -> Eigvec_2D.block(Defo_index,0,2,2).col(0)) * this -> Eigvec_2D.block(Defo_index,0,2,2).col(0).transpose() ) + ( (this -> Eigval_2D(Defo_index+1,0)) * (this -> Eigvec_2D.block(Defo_index,0,2,2).col(1)) * this -> Eigvec_2D.block(Defo_index,0,2,2).col(1).transpose());
    }
}

void cloth_calc::cloth_defo_3D()
{
    cloth_eig_3D();

    // initialize the matrix to store the stretch tensor
    this -> Defo_3D.resize(faces.rows()*9,3);

    int Defo_num = faces.rows()*9;

    for(int Defo_index=0; Defo_index<Defo_num; Defo_index=Defo_index+3)
    {
        // U = (/lamdba_1)*v1*v1^T + (/lamdba_2)*v2*v2^T + (/lamdba_3)*v3*v3^T
        Defo_3D.block(Defo_index,0,3,3) << ( (this -> Eigval_3D(Defo_index,0)) * (this -> Eigvec_3D.block(Defo_index,0,3,3).col(0)) * this -> Eigvec_3D.block(Defo_index,0,3,3).col(0).transpose() ) + ( (this -> Eigval_3D(Defo_index+1,0)) * (this -> Eigvec_3D.block(Defo_index,0,3,3).col(1)) * this -> Eigvec_3D.block(Defo_index,0,3,3).col(1).transpose()) + ( (this -> Eigval_3D(Defo_index+2,0)) * (this -> Eigvec_3D.block(Defo_index,0,3,3).col(2)) * this -> Eigvec_3D.block(Defo_index,0,3,3).col(2).transpose());
    }

}

void cloth_calc::cloth_defo_assemble()
{
    cloth_defo_3D();

    // now we assemble all the stretch tensor U by the vertex index

    // adjacentfaces is a list of lists of faces adjacent to each vertex.
    // That is, mesh->adjacentfaces[i][j] is the index of the jth triangle that touches vertex i.
    _plyMeshR -> trimesh::TriMesh::need_adjacentfaces();

    // we calculate at fiest the trianglearea based weights
    int Vert_num = verts.rows();
    Defo_3D_assemble.resize(Vert_num*3,3);


    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all vertice
    {
        // initialize the adjacent triangles of each vertex
        int El_num = _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).size();

        // we calculate here the adjacent area
        Eigen::MatrixXd Triangle_base;
        Eigen::MatrixXd Triangle_adjacent;

        Eigen::MatrixXd Area, weight;
        Area.resize(El_num,1);
        weight.resize(El_num,1);

        // for adjacent triangles
        for(int El_index=0; El_index<El_num; El_index++) // do loop for all adjacent triangles and save the area
        {

            Triangle_adjacent.resize(3,3);
            // we need the coordinate of each triangles
            Triangle_adjacent.col(0) << (this -> VecR(0, _plyMeshR -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(1, _plyMeshR -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(2, _plyMeshR -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3));
            Triangle_adjacent.col(1) << (this -> VecR(3, _plyMeshR -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(4, _plyMeshR -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(5, _plyMeshR -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3));
            Triangle_adjacent.col(2) << 1,
                                        1,
                                        1;

            Area.row(El_index) << 1./2.*Triangle_adjacent.determinant();

            Triangle_adjacent.resize(0,0);
        }

        double Area_sum = Area.sum();

        // we calculate here the weight
        if( El_num != 0 ) // if there exisit adjacent triangles of vertex
        {
            weight = (Area / Area_sum).cwiseAbs();
        }

        // assemble stretch tensor U applied by formular
        // U_i = U_i^{1/2} * exp(/sum_j {/omega_j */log(U_i^{1/2}*U_j*U_i^{-1/2})) * U_i^{1/2}
    }

    /*
    std::cout << Defo_3D.block(0,0,3,3) << std::endl;
    std::cout << _plyMeshR -> trimesh::TriMesh::adjacentfaces.at(6).at(1) << std::endl;
    // this is the jth triangle
    std::cout << _plyMeshR -> trimesh::TriMesh::adjacentfaces.at(6).at(1) << std::endl;
    std::cout << VecR.col(_plyMeshR -> trimesh::TriMesh::adjacentfaces.at(6).at(1)*3) << std::endl;
    std::cout << _plyMeshR -> trimesh::TriMesh::adjacentfaces.at(15059).size() << std::endl;
    */

}

void cloth_calc::cloth_displ()
{
    cloth_vec();

    // initialize the matrix to store the transformation matrix
    this -> Displ.resize(faces.rows()*6,2);

    int Displ_index = 0;
    int Displ_num = faces.rows()*3;

    for(int Vec_index=0; Vec_index<Displ_num; Vec_index++)
    {
        // compute the transformation matrix (Transf)
        // T = [u1, u2] * [u1_, u2_]^-1
        // we have here 3D triangles, thus we need to transform at first to 2D triangles using affine transformation
        Eigen::MatrixXd Transf((Eigen::Map<Eigen::Matrix<double,3,2> >(this -> VecT.col(Vec_index).data())).block<2,2>(0,0) * ((Eigen::Map<Eigen::Matrix<double,3,2> >(this -> VecR.col(Vec_index).data())).block<2,2>(0,0)).inverse());

        // H = F-1
        this -> Displ.block(Displ_index,0,2,2) << Transf - Eigen::MatrixXd::Identity(2,2);
        Displ_index = Displ_index+2;
    }
}


void cloth_calc::cloth_eig_neighbor()
{

    cloth_init_vert();

    // initialize the points set P(reference) and Q(template)
    Eigen::MatrixXd P, Q;
    // Eigen::MatrixXd H;

    // initialize the eigenvalues and eigenvectors
    int Vert_num = verts.rows();
    this -> Eigval_neighbor.resize(Vert_num*3,1);
    this -> Eigvec_neighbor.resize(Vert_num*3,3);

    // initialize the trimesh
    cloth_init_neighbor();

    // we consider here the covariance matrix H as the approximated deformation gradient F
    // KABSCH ALGORITHM
    //  1.Translation to origin
    //  2.Computation of the covariance matrix
    //      H = P^T * Q -> F = P^T * Q
    //  3.Computation of the singular value decomposition
    //      H = U * S * V^T
    //      def: U, V - rotation informations
    //      def:    S - stretch informations


    int Eig_index = 0;

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all elements
    {
        // face has the structure of face(i.j)
        // where i is the i-th element, j is the j-th vertex of i-th element

        // calculate the number of the nerghborred vertice of vertex i
        int Neighbor_num = _plyMesh -> trimesh::TriMesh::neighbors.at(Vert_index).size();

        P.resize(Neighbor_num, 3);
        Q.resize(Neighbor_num, 3);

        for(int Neighbor_index=0; Neighbor_index < Neighbor_num; Neighbor_index++) // for all neighbors in each vertex
        {
            // for template
            P.row(Neighbor_index) << vertsT.row(_plyMeshT -> trimesh::TriMesh::neighbors.at(Vert_index).at(Neighbor_index)) - vertsT.row(Vert_index);
            // for reference
            Q.row(Neighbor_index) << vertsR.row(_plyMeshR -> trimesh::TriMesh::neighbors.at(Vert_index).at(Neighbor_index)) - vertsR.row(Vert_index);


        }

        // Irving et al. - Invertible Finite Elements For Robust Simulation of Large Deformation
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv((P.transpose()*Q).transpose() * (P.transpose()*Q));

        this -> Eigval_neighbor.block(Eig_index,0,3,1) << solv.eigenvalues().cwiseSqrt();
        this -> Eigvec_neighbor.block(Eig_index,0,3,3) << solv.eigenvectors();



        P.resize(0,0);
        Q.resize(0,0);

        Eig_index = Eig_index+3;

    }
}

void cloth_calc::cloth_defo_neighbor()
{
    cloth_eig_neighbor();

    this -> Defo_neighbor.resize(faces.rows()*9,3);

    int Defo_num = faces.rows()*9;

    for(int Defo_index=0; Defo_index<Defo_num; Defo_index=Defo_index+3)
    {
        // U = /lamdba_1*v1*v1^T + /lamdba_2*v2*v2^T + /lamdba_3*v3*v3^T
        Defo_neighbor.block(Defo_index,0,3,3) << ( (this -> Eigval_neighbor(Defo_index,0)) * (this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(0)) * this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(0).transpose() ) + ( (this -> Eigval_neighbor(Defo_index+1,0)) * (this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(1)) * this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(1).transpose()) + ( (this -> Eigval_neighbor(Defo_index+2,0)) * (this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(2)) * this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(2).transpose());
    }
}

void cloth_calc::cloth_eig_neighbor2x()
{
    cloth_init_vert();

    // initialize the points set P(reference) and Q(template)
    Eigen::MatrixXd P, Q;
    // Eigen::MatrixXd H;

    // initialize the eigenvalues and eigenvectors
    int Vert_num = verts.rows();
    this -> Eigval_neighbor2x.resize(Vert_num*3,1);
    this -> Eigvec_neighbor2x.resize(Vert_num*3,3);

    // initialize the trimesh
    cloth_init_neighbor();

    int Eig_index = 0;
    int Neighbor_num;
    int Neighbor2x_num;


    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all elements
    {
        // face has the structure of face(i.j)
        // where i is the i-th element, j is the j-th vertex of i-th element

        // calculate the number of the nerghborred vertice of vertex i
        Neighbor_num = _plyMesh -> trimesh::TriMesh::neighbors.at(Vert_index).size();

        Neighbor2x_num = 0;

        // evaluate the size of Neighbor2x
        for(int Neighbor_index=0; Neighbor_index < Neighbor_num; Neighbor_index++) // for all neighbors in each base vertex i
        {
            int Neighbor2x_Vert_index = _plyMesh -> trimesh::TriMesh::neighbors.at(Vert_index).at(Neighbor_index);
            Neighbor2x_num = Neighbor2x_num + _plyMesh -> trimesh::TriMesh::neighbors.at(Neighbor2x_Vert_index).size();
        }

        P.resize(Neighbor2x_num, 3);
        Q.resize(Neighbor2x_num, 3);

        int index=0;

        for(int Neighbor_index=0; Neighbor_index < Neighbor_num; Neighbor_index++) // for all neighbors in each vertex
        {
            // calculate the neighbor2x vertex index
            int Neighbor2x_Vert_index = _plyMesh -> trimesh::TriMesh::neighbors.at(Vert_index).at(Neighbor_index);
            // the number of the neighbor2x vertex
            Neighbor2x_num = _plyMesh -> trimesh::TriMesh::neighbors.at(Neighbor2x_Vert_index).size();

            for(int Neighbor2x_index=0; Neighbor2x_index<Neighbor2x_num; Neighbor2x_index++) //for all neighbor in each neighbor2x vertex
            {
                // for template
                P.row(index) << vertsT.row(_plyMeshT -> trimesh::TriMesh::neighbors.at(Neighbor2x_Vert_index).at(Neighbor2x_index)) - vertsT.row(Neighbor2x_Vert_index);
                // for reference
                Q.row(index) << vertsR.row(_plyMeshR -> trimesh::TriMesh::neighbors.at(Neighbor2x_Vert_index).at(Neighbor2x_index)) - vertsR.row(Neighbor2x_Vert_index);

                index++;
            }
        }
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv((P.transpose()*Q).transpose() * (P.transpose()*Q));

        this -> Eigval_neighbor2x.block(Eig_index,0,3,1) << solv.eigenvalues().cwiseSqrt();
        this -> Eigvec_neighbor2x.block(Eig_index,0,3,3) << solv.eigenvectors();

        P.resize(0,0);
        Q.resize(0,0);

        Eig_index = Eig_index+3;
    }

}


void cloth_calc::cloth_vec_normalize(Eigen::MatrixXd Eigenval, int dim)
{
    int Eigval_norm_size = Eigenval.rows() / dim;

    Eigval_norm_dir1.resize(Eigval_norm_size,1);
    Eigval_norm_dir2.resize(Eigval_norm_size,1);
    Eigval_norm_dir3.resize(Eigval_norm_size,1);

    if(dim==2)
    {
        // this is to normlize the eigenvalue in order to smooth the solution space
        Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<2> >Eigval_norm_dir1(Eigenval.data()  ,Eigval_norm_size,1);
        Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<2> >Eigval_norm_dir2(Eigenval.data()+1,Eigval_norm_size,1);

        this -> Eigval_norm_dir1 = (Eigval_norm_dir1 - Eigval_norm_dir1.minCoeff() * Eigen::MatrixXd::Identity(Eigval_norm_size, 1)) / (Eigval_norm_dir1.maxCoeff() - Eigval_norm_dir1.minCoeff());
        this -> Eigval_norm_dir2 = (Eigval_norm_dir2 - Eigval_norm_dir2.minCoeff() * Eigen::MatrixXd::Identity(Eigval_norm_size, 1)) / (Eigval_norm_dir2.maxCoeff() - Eigval_norm_dir2.minCoeff());
    }

    if(dim==3)
    {
        // this is to normlize the eigenvalue in order to smooth the solution space
        Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<3> >Eigval_norm_dir1(Eigenval.data()  ,Eigval_norm_size,1);
        Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<3> >Eigval_norm_dir2(Eigenval.data()+1,Eigval_norm_size,1);
        Eigen::Map<Eigen::MatrixXd, 0, Eigen::InnerStride<3> >Eigval_norm_dir3(Eigenval.data()+2,Eigval_norm_size,1);

        this -> Eigval_norm_dir1 = (Eigval_norm_dir1 - Eigval_norm_dir1.minCoeff() * Eigen::MatrixXd::Identity(Eigval_norm_size, 1)) / (Eigval_norm_dir1.maxCoeff() - Eigval_norm_dir1.minCoeff());
        this -> Eigval_norm_dir2 = (Eigval_norm_dir2 - Eigval_norm_dir2.minCoeff() * Eigen::MatrixXd::Identity(Eigval_norm_size, 1)) / (Eigval_norm_dir2.maxCoeff() - Eigval_norm_dir2.minCoeff());
        this -> Eigval_norm_dir3 = (Eigval_norm_dir3 - Eigval_norm_dir3.minCoeff() * Eigen::MatrixXd::Identity(Eigval_norm_size, 1)) / (Eigval_norm_dir3.maxCoeff() - Eigval_norm_dir3.minCoeff());
    }
}


void cloth_calc::cloth_WriteColor(Eigen::MatrixXd Eigval_norm, const std::string &  ifileName)
{

    // set the color and write as ply files

    ply_module* plyColor;
    plyColor = new ply_module();

    int Eigval_num = Eigval_norm.rows();

    for(int Eigval_index=0; Eigval_index<Eigval_num; Eigval_index++)
    {
        if(isnan(Eigval_norm(Eigval_index, 0)))
            {Eigval_norm(Eigval_index, 0) = 0;}
    }

    // set vertice and colors
    plyColor -> setVertices(verts);
    plyColor -> setCurvatures(Eigval_norm);

    plyColor -> writePLY(ifileName, true, false, false, true, false);

    // using color-map
    /*
    std::vector< std::vector<double> > cmap = makeColorMap();

    Eigen::MatrixXi Eigval_color;
    Eigval_color.resize(Eigval_num, 3);

    for(int Eigval_index=0; Eigval_index<Eigval_num; Eigval_index++)
    {
        int idx;

        if(isnan(Eigval_norm(Eigval_index, 0)))
            {idx = 0;}
        else
            {idx = Eigval_norm(Eigval_index, 0) * (cmap.size()-1);}

        Eigval_color.row(Eigval_index) << cmap[idx][0], cmap[idx][1], cmap[idx][2];
    }
    */

}


Eigen::MatrixXd cloth_calc::GetEigval()
{
    return this -> Eigval_2D;
}

Eigen::MatrixXd cloth_calc::GetEigvec()
{
    return this -> Eigvec_2D;
}

Eigen::MatrixXd cloth_calc::GetDefo_2D()
{
    return this -> Defo_2D;
}

Eigen::MatrixXd cloth_calc::GetDefo_3D()
{
    return this -> Defo_3D;
}

Eigen::MatrixXd cloth_calc::GetDispl()
{
    return this -> Displ;
}

Eigen::MatrixXd cloth_calc::GetEigval_3D()
{
    return this -> Eigval_3D;
}

Eigen::MatrixXd cloth_calc::GetEigvec_3D()
{
    return this -> Eigvec_3D;
}


Eigen::MatrixXd cloth_calc::GetEigval_neighbor()
{
    return this -> Eigval_neighbor;
}

Eigen::MatrixXd cloth_calc::GetEigvec_neighbor()
{
    return this -> Eigvec_neighbor;
}

Eigen::MatrixXd cloth_calc::GetEigval_neighbor2x()
{
    return this -> Eigval_neighbor2x;
}

Eigen::MatrixXd cloth_calc::GetEigvec_neighbor2x()
{
    return this -> Eigvec_neighbor2x;
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

    //neighbor.at( vertex_index ).at( num_vertex )
    double num_neighbor = mymesh -> trimesh::TriMesh::neighbors.at(35946).at(1);
    double vert_x = mymesh -> trimesh::TriMesh::vertices.at(35483)[0];

    std::cout << num_neighbor << std::endl;
    std::cout << vert_x << std::endl;

}

cloth_calc::~cloth_calc()
{

}



