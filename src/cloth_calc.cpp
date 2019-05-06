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
    _plyModule ->readPLY(BS, true, true, true, true, true);
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


void cloth_calc::cloth_stretchTensor_2D()
{
    cloth_eig_2D();

    // initialize the matrix to store the stretch tensor
    this -> U_2D.resize(faces.rows()*6,2);

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
        U_2D.block(Defo_index,0,2,2) << ( (this -> Eigval_2D(Defo_index,0)) * (this -> Eigvec_2D.block(Defo_index,0,2,2).col(0)) * this -> Eigvec_2D.block(Defo_index,0,2,2).col(0).transpose() ) + ( (this -> Eigval_2D(Defo_index+1,0)) * (this -> Eigvec_2D.block(Defo_index,0,2,2).col(1)) * this -> Eigvec_2D.block(Defo_index,0,2,2).col(1).transpose());
    }
}

void cloth_calc::cloth_stretchTensor_3D()
{
    cloth_eig_3D();

    // initialize the matrix to store the stretch tensor
    this -> U_3D.resize(faces.rows()*9,3);

    int Defo_num = faces.rows()*9;

    for(int Defo_index=0; Defo_index<Defo_num; Defo_index=Defo_index+3)
    {
        // U = (/lamdba_1)*v1*v1^T + (/lamdba_2)*v2*v2^T + (/lamdba_3)*v3*v3^T
        U_3D.block(Defo_index,0,3,3) << ( (this -> Eigval_3D(Defo_index,0)) * (this -> Eigvec_3D.block(Defo_index,0,3,3).col(0)) * this -> Eigvec_3D.block(Defo_index,0,3,3).col(0).transpose() ) + ( (this -> Eigval_3D(Defo_index+1,0)) * (this -> Eigvec_3D.block(Defo_index,0,3,3).col(1)) * this -> Eigvec_3D.block(Defo_index,0,3,3).col(1).transpose()) + ( (this -> Eigval_3D(Defo_index+2,0)) * (this -> Eigvec_3D.block(Defo_index,0,3,3).col(2)) * this -> Eigvec_3D.block(Defo_index,0,3,3).col(2).transpose());
    }

}

void cloth_calc::cloth_stretchTensor_assemble()
{
    cloth_stretchTensor_3D();

    // now we assemble all the stretch tensor U by the vertex index

    // adjacentfaces is a list of lists of faces adjacent to each vertex.
    // That is, mesh->adjacentfaces[i][j] is the index of the jth triangle that touches vertex i.
    _plyMesh -> trimesh::TriMesh::need_adjacentfaces();

    // we calculate at fiest the trianglearea based weights
    int Vert_num = verts.rows();

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
            Triangle_adjacent.col(0) << (this -> VecR(0, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(1, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(2, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3));
            Triangle_adjacent.col(1) << (this -> VecR(3, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(4, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(5, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3));
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

void cloth_calc::cloth_displGrad_2D()
{
    cloth_vec();

    // initialize the matrix to store the transformation matrix
    this -> H_2D.resize(faces.rows()*6,2);

    int Displ_index = 0;
    int Displ_num = faces.rows()*3;

    for(int Vec_index=0; Vec_index<Displ_num; Vec_index++)
    {
        // compute the transformation matrix (Transf)
        // T = [u1, u2] * [u1_, u2_]^-1
        // we have here 3D triangles, thus we need to transform at first to 2D triangles using affine transformation
        Eigen::MatrixXd Transf((Eigen::Map<Eigen::Matrix<double,3,2> >(this -> VecT.col(Vec_index).data())).block<2,2>(0,0) * ((Eigen::Map<Eigen::Matrix<double,3,2> >(this -> VecR.col(Vec_index).data())).block<2,2>(0,0)).inverse());

        // H = F-1
        this -> H_2D.block(Displ_index,0,2,2) << Transf - Eigen::MatrixXd::Identity(2,2);
        Displ_index = Displ_index+2;
    }
}

void cloth_calc::cloth_velGrad_3D(Eigen::MatrixXd F_CT, Eigen::MatrixXd F_CR, double deltaT)
{
    int Vert_num = verts.rows();

    this -> F_der.resize(Vert_num*3,3);
    this -> F_inv.resize(Vert_num*3,3);
    this -> L.resize(Vert_num*3,3);
    this -> D.resize(Vert_num*3,3);
    this -> W.resize(Vert_num*3,3);

    // calculate the time derivation of F
    this -> F_der = (F_CR - F_CT) / deltaT;
    // calculate the time inverse of F
    int Defo_index = 0;
    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++)
    {
        this -> F_inv.block(Defo_index,0,3,3) << F_CR.block(Defo_index,0,3,3).inverse();
        Defo_index = Defo_index+3;
    }
    // calculate the L
    int Vel_index = 0;
    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++)
    {
        this -> L.block(Vel_index,0,3,3) << F_der.block(Vel_index,0,3,3) * F_inv.block(Vel_index,0,3,3);
        Vel_index = Vel_index+3;
    }
    // claculate the D and W
    int index = 0;
    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++)
    {
        this -> D.block(index,0,3,3) << 1./2.*(L.block(index,0,3,3) + L.block(index,0,3,3).transpose());
        this -> W.block(index,0,3,3) << 1./2.*(L.block(index,0,3,3) - L.block(index,0,3,3).transpose());
        index = index+3;
    }
}

void cloth_calc::cloth_velGrad_assemble(Eigen::MatrixXd VelGrad, double Per)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //Rieman tensor
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
    cloth_vec();
    cloth_init_neighbor();

    // now we assemble all the stretch tensor U by the vertex index

    // adjacentfaces is a list of lists of faces adjacent to each vertex.
    // That is, mesh->adjacentfaces[i][j] is the index of the jth triangle that touches vertex i.
    _plyMesh -> trimesh::TriMesh::need_adjacentfaces();

    // we calculate at fiest the trianglearea based weights
    int Vert_num = verts.rows();
    this -> D_assem.resize(Vert_num*3,3);

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
            Triangle_adjacent.col(0) << (this -> VecR(0, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(1, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(2, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3));
            Triangle_adjacent.col(1) << (this -> VecR(3, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(4, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3)),
                                        (this -> VecR(5, _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index)*3));
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
        // assemble stretch tensor L applied by formular
        // L_i = L_i^{1/2} * exp(/sum_j {/omega_j * /log(L_i^{1/2}*L_j*L_i^{-1/2})) * L_i^{1/2}

        if(El_num == 0)
        {
            this -> D_assem.block(Vert_index*3,0,3,3) = VelGrad.block(Vert_index*3,0,3,3);
        }

        else
        {
            Eigen::MatrixXd L_j, L_i, L_sum, L_tmp, L_new;
            L_i.resize(3,3);
            L_j.resize(El_num*3,3);

            L_sum.resize(3,3);
            L_tmp.resize(3,3);
            L_new.resize(3,3);

            L_i = VelGrad.block(Vert_index*3,0,3,3);
            // std::cout <<VelGrad.block(this -> _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(0)*3,0,3,3) << std::endl;
            // std::cout << Vert_index << std::endl;
            //std::cout << El_num << std::endl;

            for(int El_index=0; El_index<El_num; El_index++)
            {
                L_j.block(El_index*3,0,3,3) = VelGrad.block(this -> _plyMesh -> trimesh::TriMesh::neighbors.at(Vert_index).at(El_index)*3,0,3,3);
            }

            L_sum.Zero(3,3);
            for(int El_index=0; El_index<El_num; El_index++)
            {
                // some error here because of the square roots
                L_tmp = (L_i.cwiseSqrt() * L_j.block(El_index*3,0,3,3) * L_i.cwiseSqrt()).array().log10() * weight(El_index);
                L_sum = L_sum + L_tmp;
                L_tmp.resize(0,0);
            }

            L_new = L_sum.array().exp();

            this -> D_assem.block(Vert_index*3,0,3,3) = L_i.cwiseSqrt() * L_new * L_i.cwiseSqrt();
            // std::cout << L_sum << std::endl;

            L_i.resize(0,0);
            L_j.resize(0,0);
            L_sum.resize(0,0);
            L_tmp.resize(0,0);
            L_new.resize(0,0);
            weight.resize(0,0);
        }

    }
    */
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    cloth_init_vert();

    // initialize the weights matrix
    Eigen::MatrixXd weights;

    // initialize the eigenvalues and eigenvectors
    int Vert_num = verts.rows();
    this -> D_assem.resize(Vert_num*3,3);

    std::vector<double> query_pt(3);

    typedef nanoflann::KDTreeEigenMatrixAdaptor<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> kd_tree;

    kd_tree vert_index(3, std::cref(verts),10 /* max leaf */ );
    vert_index.index -> buildIndex();

    const size_t num_results = verts.rows() * Per ; // using 2% total vertices
    // const size_t num_results = 200; // using 30 neighboring vertices

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all Vertices
    {
        for (size_t d = 0; d < 3; d++)
          query_pt[d] = this -> verts(Vert_index, d);

        std::vector<size_t> ret_indexes(num_results); // the neighbor vertice index
        std::vector<double> out_dists_sqr(num_results); // the distance to the neighbor

        nanoflann::KNNResultSet<double> resultSet(num_results);

        resultSet.init(&ret_indexes[0], &out_dists_sqr[0]);
        vert_index.index->findNeighbors(resultSet, &query_pt[0], nanoflann::SearchParams(10));

        // weight metric computed as the inverse of distance between vertex i and closest verteices
        double dists_sum = 0;
        for(int i=0; i<num_results; i++)
        {
            if(out_dists_sqr[i]!=0)
            {
                dists_sum = dists_sum + (1./out_dists_sqr[i]);
            }
        }

        weights.resize(num_results,1);
        for(int i=0; i<num_results; i++)
        {
            if(out_dists_sqr[i]!=0)
            {
                weights(i) = (1./out_dists_sqr[i]) / (dists_sum);
            }
            else
            {
                weights(i) = 1;
            }
        }

        for(int i=0; i<num_results; i++)
        {
            // std::cout << ret_indexes[i] << std::endl;
            this -> D_assem.block(Vert_index*3,0,3,3) = this -> D_assem.block(Vert_index*3,0,3,3) + weights(i) * VelGrad.block(ret_indexes[i]*3,0,3,3);
        }

        query_pt.clear();
        ret_indexes.clear();
        out_dists_sqr.clear();

        weights.resize(0,0);

    }

}

void cloth_calc::cloth_velGrad_normalize(Eigen::MatrixXd VelGrad)
{
    int Vert_num = verts.rows();

    Eigen::MatrixXd D_dir1;
    D_dir1.resize(Vert_num,1);

    this -> D_norm_dir1.resize(Vert_num,1);

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++)
    {
        D_dir1(Vert_index) = VelGrad(Vert_index*3,0);
    }

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++)
    {
        this -> D_norm_dir1(Vert_index) = (D_dir1(Vert_index) - D_dir1.minCoeff())/(D_dir1.maxCoeff() - D_dir1.minCoeff());
    }

    //this -> D_norm_dir1 = (D_dir1 - D_dir1.minCoeff() * Eigen::MatrixXd::Identity(Vert_num, 1)) / (D_dir1.maxCoeff() - D_dir1.minCoeff());

    // std::cout << D_norm_dir1 << std::endl;

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

        // calculate the number of the nerghboring vertice of vertex i
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

void cloth_calc::cloth_stretchTensor_neighbor()
{
    cloth_eig_neighbor();

    this -> U_neighbor.resize(faces.rows()*9,3);

    int Defo_num = faces.rows()*9;

    for(int Defo_index=0; Defo_index<Defo_num; Defo_index=Defo_index+3)
    {
        // U = /lamdba_1*v1*v1^T + /lamdba_2*v2*v2^T + /lamdba_3*v3*v3^T
        U_neighbor.block(Defo_index,0,3,3) << ( (this -> Eigval_neighbor(Defo_index,0)) * (this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(0)) * this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(0).transpose() ) + ( (this -> Eigval_neighbor(Defo_index+1,0)) * (this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(1)) * this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(1).transpose()) + ( (this -> Eigval_neighbor(Defo_index+2,0)) * (this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(2)) * this -> Eigvec_neighbor.block(Defo_index,0,3,3).col(2).transpose());
    }
}

void cloth_calc::cloth_eig_kdTree(double Per)
{
    cloth_init_vert();

    // initialize the points set P(reference) and Q(template)
    Eigen::MatrixXd P, Q;
    Eigen::MatrixXd H;
    Eigen::MatrixXd C;

    // initialize the eigenvalues and eigenvectors
    int Vert_num = verts.rows();
    this -> Eigval_neighborKdTree.resize(Vert_num*3,1);
    this -> Eigvec_neighborKdTree.resize(Vert_num*3,3);
    this -> F.resize(Vert_num*3,3);

    std::vector<double> query_pt(3);

    typedef nanoflann::KDTreeEigenMatrixAdaptor<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> kd_tree;

    kd_tree vert_index(3, std::cref(verts),10 /* max leaf */ );
    vert_index.index -> buildIndex();

    const size_t num_results = verts.rows() * Per ; // using 2% total vertices
    // const size_t num_results = 200; // using 30 neighboring vertices

    int Eig_index = 0;

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all Vertices
    {
        for (size_t d = 0; d < 3; d++)
          query_pt[d] = this -> verts(Vert_index, d);

        std::vector<size_t> ret_indexes(num_results); // the neighbor vertice index
        std::vector<double> out_dists_sqr(num_results); // the distance to the neighbor

        nanoflann::KNNResultSet<double> resultSet(num_results);

        P.resize(num_results, 3);
        Q.resize(num_results, 3);
        H.resize(3,3);
        C.resize(3,3);

        resultSet.init(&ret_indexes[0], &out_dists_sqr[0]);
        vert_index.index->findNeighbors(resultSet, &query_pt[0], nanoflann::SearchParams(10));

        for(int i=0; i<num_results; i++)
        {
            // for template
            P.row(i) << vertsT.row(ret_indexes[i]) - vertsT.row(Vert_index);
            // for reference
            Q.row(i) << vertsR.row(ret_indexes[i]) - vertsR.row(Vert_index);
        }

        H = P.transpose() * Q;
        C = H.transpose() * H;

        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv(C);
        this -> Eigval_neighborKdTree.block(Eig_index,0,3,1) << solv.eigenvalues().cwiseSqrt();
        this -> Eigvec_neighborKdTree.block(Eig_index,0,3,3) << solv.eigenvectors();
        this -> F.block(Eig_index,0,3,3) << H;

        // initialize
        P.resize(0,0);
        Q.resize(0,0);
        H.resize(0,0);
        C.resize(0,0);

        query_pt.clear();
        ret_indexes.clear();
        out_dists_sqr.clear();

        Eig_index = Eig_index+3;
    }

    /*
    std::cout << "knnSearch(nn=" << num_results << "): \n";
    for (size_t i = 0; i < num_results; i++)
      std::cout << "ret_index[" << i << "]=" << ret_indexes[i]
                << " out_dist_sqr=" << out_dists_sqr[i] << std::endl;
    */
}

void cloth_calc::cloth_wrink_vec_field(Eigen::MatrixXd Eigenval, Eigen::MatrixXd Eigenvec)
{
    int Vert_num = verts.rows();
    this -> v.resize(Vert_num*3,1);
    this -> v_norm.resize(Vert_num,1);

    for(int Vert_index=0;Vert_index<Vert_num;Vert_index++)
    {
        if((1-Eigenval(Vert_index*3)) > 0)
        {
            v.block(Vert_index*3,0,3,1) = (1 - Eigenval(Vert_index*3)) * Eigenvec.block(Vert_index*3,1,3,1);
        }
        else
        {
            v.block(Vert_index*3,0,3,1) = 0 * Eigenvec.block(Vert_index*3,1,3,1);
        }
        v_norm(Vert_index) = v.block(Vert_index*3,0,3,1).norm();
    }
}

void cloth_calc::cloth_eig_neighbor2x()
{
    cloth_init_vert();

    // initialize the points set P(reference) and Q(template)
    Eigen::MatrixXd P, Q;
    Eigen::MatrixXd H;
    Eigen::MatrixXd C;

    // initialize the eigenvalues and eigenvectors
    int Vert_num = verts.rows();
    this -> Eigval_neighbor2x.resize(Vert_num*3,1);
    this -> Eigvec_neighbor2x.resize(Vert_num*3,3);
    this -> F.resize(Vert_num*3,3);

    // initialize the trimesh
    cloth_init_neighbor();

    int Eig_index = 0;

    int Neighbor_num;
    int Neighbor2x_num;

    std::vector<int> Neighbor_Vert;

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all Vertice
    {
        // face has the structure of face(i.j)
        // where i is the i-th element, j is the j-th vertex of i-th element

        // base vertex index
        // Vert_index

        // size of the 1st Neighbor vertice
        Neighbor_num = _plyMesh -> trimesh::TriMesh::neighbors.at(Vert_index).size();

        // evaluate the 1st Neighbor
        for(int Neighbor_index=0; Neighbor_index < Neighbor_num; Neighbor_index++) // for all neighbors in each base vertex i
        {
            // 1st Neighbor vertex index
            int Neighbor_Vert_index = _plyMesh -> trimesh::TriMesh::neighbors.at(Vert_index).at(Neighbor_index);
            // size of the 2nd Neighbor vertice
            Neighbor2x_num = _plyMesh -> trimesh::TriMesh::neighbors.at(Neighbor_Vert_index).size();

            // store the index of the 1st Neighbor vertex index
            Neighbor_Vert.push_back(Neighbor_Vert_index);

            // evaluate the 2nd Neighbor
            for(int Neighbor2x_index=0; Neighbor2x_index<Neighbor2x_num; Neighbor2x_index++) //for all neighbor in each neighbor2x vertex
            {
                // 2nd Neighbor vertex index
                int Neighbor2x_Vert_index = _plyMeshT -> trimesh::TriMesh::neighbors.at(Neighbor_Vert_index).at(Neighbor2x_index);

                // store the index of the 2nd Neighbor vertex index
                Neighbor_Vert.push_back(Neighbor2x_Vert_index);
            }
        }

        // erase the doppel vertex index
        sort(Neighbor_Vert.begin(), Neighbor_Vert.end());
        Neighbor_Vert.erase(unique(Neighbor_Vert.begin(), Neighbor_Vert.end()), Neighbor_Vert.end());

        int Neighbor_Vert_size = Neighbor_Vert.size();

        P.resize(Neighbor_Vert_size, 3);
        Q.resize(Neighbor_Vert_size, 3);
        H.resize(3,3);
        C.resize(3,3);

        for(int i=0; i<Neighbor_Vert_size; i++)
        {
            // for template
            P.row(i) << vertsT.row(Neighbor_Vert[i]) - vertsT.row(Vert_index);
            // for reference
            Q.row(i) << vertsR.row(Neighbor_Vert[i]) - vertsR.row(Vert_index);
        }

        H = P.transpose() * Q;
        C = H.transpose() * H;
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv(C);

        this -> Eigval_neighbor2x.block(Eig_index,0,3,1) << solv.eigenvalues().cwiseSqrt();
        this -> Eigvec_neighbor2x.block(Eig_index,0,3,3) << solv.eigenvectors();
        this -> F.block(Eig_index,0,3,3) << H;

        // initialize
        P.resize(0,0);
        Q.resize(0,0);
        H.resize(0,0);
        C.resize(0,0);
        Neighbor_Vert.clear();

        Eig_index = Eig_index+3;
    }

    // this is for debug
    std::cout<< "the number of neighboring vertives is " << _plyMesh -> trimesh::TriMesh::neighbors.at(0).size()<<std::endl;

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

    // set pink color as default
    Eigen::MatrixXi pink;
    pink.resize(Eigval_num,3);
    for(int i=0;i<Eigval_num;i++)
    {
        pink.row(i) << 255, 0, 255;
    }

    // set vertice and colors
    plyColor -> setVertices(verts);
    plyColor -> setColors(pink);
    plyColor -> setCurvatures(Eigval_norm);

    plyColor -> writePLY(ifileName, true, false, false, true, false);

}


Eigen::MatrixXd cloth_calc::GetEigval()
{
    return this -> Eigval_2D;
}

Eigen::MatrixXd cloth_calc::GetEigvec()
{
    return this -> Eigvec_2D;
}

Eigen::MatrixXd cloth_calc::GetDefoGrad()
{
    return this -> F;
}

Eigen::MatrixXd cloth_calc::GetStretchTensor_2D()
{
    return this -> U_2D;
}

Eigen::MatrixXd cloth_calc::GetStretchTensor_3D()
{
    return this -> U_3D;
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

Eigen::MatrixXd cloth_calc::GetEigval_neighborKdTree()
{
    return this -> Eigval_neighborKdTree;
}

Eigen::MatrixXd cloth_calc::GetEigvec_neighborKdTree()
{
    return this -> Eigvec_neighborKdTree;
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

Eigen::MatrixXd cloth_calc::GetWrinkVecField()
{
    return this -> v;
}

Eigen::MatrixXd cloth_calc::GetWrinkVecField_norm()
{
    return this -> v_norm;
}

Eigen::MatrixXd cloth_calc::GetStrTensor()
{
    return this -> D;
}

Eigen::MatrixXd cloth_calc::GetRotTensor()
{
    return this -> W;
}

Eigen::MatrixXd cloth_calc::GetStrTensorAsemmble()
{
    return this -> D_assem;
}

Eigen::MatrixXd cloth_calc::GetStrTensor_norm_dir1()
{
    return this -> D_norm_dir1;
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



