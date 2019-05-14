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

void cloth_calc::cloth_map_neighbor(int MODE)
{
    cloth_init_neighbor();
    cloth_init_vert();

    int Vert_num = verts.rows();
    std::vector<int> Neighbor_Vert;

    switch (MODE)
    {
        case 1:
        {
            int Neighbor_num;

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
                    // store the index of the 1st Neighbor vertex index
                    Neighbor_Vert.push_back(Neighbor_Vert_index);
                }


                // erase the doppel vertex index
                sort(Neighbor_Vert.begin(), Neighbor_Vert.end());
                Neighbor_Vert.erase(unique(Neighbor_Vert.begin(),Neighbor_Vert.end()),Neighbor_Vert.end());

                // map the neighborhood informations vector
                this -> mapNeighbor[Vert_index] = std::vector<int>();
                this -> mapNeighbor[Vert_index].swap(Neighbor_Vert);

                Neighbor_Vert.clear();

                // std::cout << mapNeighbor[Vert_index].size() << std::endl;


            }
            break;
        }
        case 2:
        {
            int Neighbor_num;
            int Neighbor2x_num;

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
                Neighbor_Vert.erase(unique(Neighbor_Vert.begin(),Neighbor_Vert.end()),Neighbor_Vert.end());

                // map the neighborhood informations vector
                this -> mapNeighbor2x[Vert_index] = std::vector<int>();
                this -> mapNeighbor2x[Vert_index].swap(Neighbor_Vert);

                Neighbor_Vert.clear();


            }

            break;
        }
        case 3:
        {
            for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all Vertice
            {
                int Neighbor_num;
                int Neighbor2x_num;
                int Neighbor3x_num;
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

                        Neighbor3x_num = _plyMesh -> trimesh::TriMesh::neighbors.at(Neighbor2x_Vert_index).size();
                        // store the index of the 2nd Neighbor vertex index
                        Neighbor_Vert.push_back(Neighbor2x_Vert_index);

                        // evaluate the 3rd Neighbor
                        for(int Neighbor3x_index=0; Neighbor3x_index<Neighbor3x_num; Neighbor3x_index++) //for all neighbor in each neighbor2x vertex
                        {
                            // 2nd Neighbor vertex index
                            int Neighbor3x_Vert_index = _plyMeshT -> trimesh::TriMesh::neighbors.at(Neighbor2x_Vert_index).at(Neighbor3x_index);

                            // store the index of the 3rd Neighbor vertex index
                            Neighbor_Vert.push_back(Neighbor3x_Vert_index);
                        }
                    }
                }

                // erase the doppel vertex index
                sort(Neighbor_Vert.begin(), Neighbor_Vert.end());
                Neighbor_Vert.erase(unique(Neighbor_Vert.begin(), Neighbor_Vert.end()), Neighbor_Vert.end());

                // map the neighborhood informations vector
                this -> mapNeighbor3x[Vert_index] = std::vector<int>();
                this -> mapNeighbor3x[Vert_index].swap(Neighbor_Vert);

                Neighbor_Vert.clear();
            }
            break;
        }
        case 4:
        {
            int Neighbor_num;
            int Neighbor2x_num;
            int Neighbor3x_num;
            int Neighbor4x_num;
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

                        Neighbor3x_num = _plyMesh -> trimesh::TriMesh::neighbors.at(Neighbor2x_Vert_index).size();
                        // store the index of the 2nd Neighbor vertex index
                        Neighbor_Vert.push_back(Neighbor2x_Vert_index);

                        // evaluate the 3rd Neighbor
                        for(int Neighbor3x_index=0; Neighbor3x_index<Neighbor3x_num; Neighbor3x_index++) //for all neighbor in each neighbor2x vertex
                        {
                            // 2nd Neighbor vertex index
                            int Neighbor3x_Vert_index = _plyMeshT -> trimesh::TriMesh::neighbors.at(Neighbor2x_Vert_index).at(Neighbor3x_index);

                            Neighbor4x_num = _plyMesh -> trimesh::TriMesh::neighbors.at(Neighbor3x_Vert_index).size();
                            // store the index of the 3rd Neighbor vertex index
                            Neighbor_Vert.push_back(Neighbor3x_Vert_index);

                            for(int Neighbor4x_index=0; Neighbor4x_index<Neighbor4x_num; Neighbor4x_index++) //for all neighbor in each neighbor2x vertex
                            {
                                // 2nd Neighbor vertex index
                                int Neighbor4x_Vert_index = _plyMeshT -> trimesh::TriMesh::neighbors.at(Neighbor3x_Vert_index).at(Neighbor4x_index);

                                // store the index of the 3rd Neighbor vertex index
                                Neighbor_Vert.push_back(Neighbor4x_Vert_index);
                            }
                        }
                    }
                }

                // erase the doppel vertex index
                sort(Neighbor_Vert.begin(), Neighbor_Vert.end());
                Neighbor_Vert.erase(unique(Neighbor_Vert.begin(), Neighbor_Vert.end()), Neighbor_Vert.end());

                // map the neighborhood informations vector
                this -> mapNeighbor4x[Vert_index] = std::vector<int>();
                this -> mapNeighbor4x[Vert_index].swap(Neighbor_Vert);
                // std::cout << mapNeighbor4x[Vert_index].size() << std::endl;

                Neighbor_Vert.clear();
            }
            break;
        }

    }

}

void cloth_calc::cloth_map_adjacent()
{
    cloth_init_neighbor();
    _plyMesh -> trimesh::TriMesh::need_adjacentfaces();

    std::vector<int> Neighbor_Face;

    int Vert_num = verts.rows();

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all vertice
    {
        // initialize the adjacent triangles of each vertex
        int El_num;
        El_num = _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).size();
        for(int El_index=0; El_index<El_num; El_index++) // do loop for all adjacent triangles and save the area
        {
            int Neighbor_Face_index = _plyMesh -> trimesh::TriMesh::adjacentfaces.at(Vert_index).at(El_index);
            Neighbor_Face.push_back(Neighbor_Face_index);
        }

        // map the neighborhood informations vector
        this -> mapAdjacent[Vert_index] = std::vector<int>();
        this -> mapAdjacent[Vert_index].swap(Neighbor_Face);
        // std::cout << mapAdjacent[Vert_index].size() << std::endl;

        Neighbor_Face.clear();
    }

}

void cloth_calc::cloth_map_neighbor_kdTree(double Per)
{
    cloth_init_vert();
    int Vert_num = verts.rows();

    std::vector<double> query_pt(3);

    typedef nanoflann::KDTreeEigenMatrixAdaptor<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> kd_tree;

    kd_tree vert_index(3, std::cref(verts),10 /* max leaf */ );
    vert_index.index -> buildIndex();

    const size_t num_results = verts.rows() * 0.01 ; // using 2% total vertices
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

        // map the neighborhood informations vector
        this -> mapNeighborKdTree[Vert_index] = std::vector<size_t>();
        this -> mapNeighborKdTree[Vert_index].swap(ret_indexes);

        // std::cout <<  this -> mapNeighborKdTree[Vert_index].size() << std::endl;

        query_pt.clear();
        ret_indexes.clear();
        out_dists_sqr.clear();
    }

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

void cloth_calc::cloth_stretchTensor_3D(Eigen::MatrixXd Eigenval, Eigen::MatrixXd Eigenvec)
{
    // initialize the matrix to store the stretch tensor
    this -> U_3D.resize(verts.rows()*3,3);

    int Defo_num = verts.rows()*3;

    // std::cout << Defo_num;

    for(int Defo_index=0; Defo_index<Defo_num; Defo_index=Defo_index+3)
    {
        // U = (/lamdba_1)*v1*v1^T + (/lamdba_2)*v2*v2^T + (/lamdba_3)*v3*v3^T
        U_3D.block(Defo_index,0,3,3) << ( (Eigenval(Defo_index,0)) * (Eigenvec.block(Defo_index,0,3,3).col(0)) * Eigenvec.block(Defo_index,0,3,3).col(0).transpose() ) + ( (Eigenval(Defo_index+1,0)) * (Eigenvec.block(Defo_index,0,3,3).col(1)) * Eigenvec.block(Defo_index,0,3,3).col(1).transpose()) + ( (Eigenval(Defo_index+2,0)) * (Eigenvec.block(Defo_index,0,3,3).col(2)) * Eigenvec.block(Defo_index,0,3,3).col(2).transpose());
    }

}

void cloth_calc::cloth_stretchTensor_assemble(Eigen::MatrixXd U, std::map<int, std::vector<int>> MapNeighbor)
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // affine-invariant Riemannian metric
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    // cloth_init_neighbor();
    /*
    // now we assemble all the stretch tensor U by the vertex index

    // adjacentfaces is a list of lists of faces adjacent to each vertex.
    // That is, mesh->adjacentfaces[i][j] is the index of the jth triangle that touches vertex i.
    _plyMesh -> trimesh::TriMesh::need_adjacentfaces();

    // we calculate at fiest the trianglearea based weights
    int Vert_num = verts.rows();
    this -> U_3D_assem.resize(Vert_num*3,3);
    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all vertice
    {
        // std::cout << MapNeighbor[Vert_index].size() << std::endl;
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
        if( Area_sum != 0 ) // if there exisit adjacent triangles of vertex
        {
            weight = (Area / Area_sum).cwiseAbs();
        }

        // now we could do the assemble
        if(Area_sum == 0) // if there are no neighborhood
        {
            this -> U_3D_assem.block(Vert_index*3,0,3,3) = U.block(Vert_index*3,0,3,3);
        }
        else
        {
            Eigen::MatrixXd U_j, U_i, U_sum, U_tmp, U_new;
            U_i.resize(3,3); // base vertex
            U_j.resize(El_num*3,3); // neighboring vertices

            U_sum.resize(3,3);
            U_tmp.resize(3,3);
            U_new.resize(3,3);

            // calculate the U^{1/2}
            Eigen::MatrixXd Eigval, Eigvec;
            Eigval.resize(3,1);
            Eigvec.resize(3,3);
            Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv(U.block(Vert_index*3,0,3,3));
            Eigval << solv.eigenvalues().cwiseSqrt();
            Eigvec << solv.eigenvectors();
            U_i = ( (Eigval(0,0)) * Eigvec.col(0) * Eigvec.col(0).transpose() ) + ( (Eigval(1,0)) * Eigvec.col(1) * Eigvec.col(1).transpose() )+ ( (Eigval(2,0)) * Eigvec.col(2) * Eigvec.col(2).transpose() );


            for(int El_index=0; El_index<El_num; El_index++)
            {
                // some problem here because I can't use the weights
                // U_j.block(El_index*3,0,3,3) = U.block(MapNeighbor[Vert_index][El_index]*3,0,3,3);
            }
            // we sum up for all neighboring vertices
            U_sum.Zero(3,3);
            for(int El_index=0; El_index<El_num; El_index++)
            {
                // Logarithm of a matrix
                // https://en.wikipedia.org/wiki/Logarithm_of_a_matrix
                // U_tmp = (U_i.inverse() * U_j.block(El_index*3,0,3,3) * U_i.inverse()).array().log10() * weight(El_index);
                U_tmp = (U_i.inverse() * U_j.block(El_index*3,0,3,3) * U_i.inverse() - Eigen::MatrixXd::Identity(3, 3)) * weight(El_index);
                U_sum = U_sum + U_tmp;
                U_tmp.resize(0,0);
            }

            // Exponential of a matrix
            // https://en.wikipedia.org/wiki/Matrix_exponential
            U_new = U_sum + Eigen::MatrixXd::Identity(3, 3);

            this -> U_3D_assem.block(Vert_index*3,0,3,3) = U_i * U_new * U_i;
            // std::cout << U_sum << std::endl;

            // erase the matrices
            U_i.resize(0,0);
            U_j.resize(0,0);
            U_sum.resize(0,0);
            U_tmp.resize(0,0);
            U_new.resize(0,0);
            weight.resize(0,0);
            Eigval.resize(0,0);
            Eigvec.resize(0,0);
        }
    }


    */
    // we calculate at fiest the trianglearea based weights
    // std::ofstream Test("../output/Test.txt");
    cloth_vec();

    int Vert_num = verts.rows();
    this -> U_3D_assem.resize(Vert_num*3,3);
    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all vertice
    {
        int Neighbor_num = MapNeighbor[Vert_index].size();

        // we calculate here the adjacent area
        Eigen::MatrixXd weight;

        if(Neighbor_num!=0)
        {
            weight.resize(Neighbor_num,1);
            double dists_sum = 0;
            for(int Neighbor_index=0; Neighbor_index<Neighbor_num; Neighbor_index++) // do loop for all adjacent triangles and save the area
            {
                double dists_temp = 1./(vertsT.row(MapNeighbor[Vert_index][Neighbor_index]) - vertsT.row(Vert_index)).norm();
                if((isinf(dists_temp)))
                {
                    continue;
                }
                dists_sum = dists_sum + dists_temp;

            }

            for(int Neighbor_index=0; Neighbor_index<Neighbor_num; Neighbor_index++)
            {
                double dists_temp = 1./(vertsT.row(MapNeighbor[Vert_index][Neighbor_index]) - vertsT.row(Vert_index)).norm();
                if((isinf(dists_temp)))
                {
                    weight(Neighbor_index) = 0;
                    continue;
                }
                weight(Neighbor_index) = dists_temp / dists_sum;
            }
            // std::cout << weight << std::endl;
            Eigen::MatrixXd U_j, U_i, U_sum, U_tmp, U_new;
            U_i.resize(3,3); // base vertex
            U_j.resize(Neighbor_num*3,3); // neighboring vertices

            U_sum.resize(3,3);
            U_tmp.resize(3,3);
            U_new.resize(3,3);

            // calculate the U^{1/2}
            Eigen::MatrixXd Eigval, Eigvec;
            Eigval.resize(3,1);
            Eigvec.resize(3,3);
            Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv(U.block(Vert_index*3,0,3,3));
            Eigval << solv.eigenvalues().cwiseSqrt();
            Eigvec << solv.eigenvectors();
            U_i = ( (Eigval(0,0)) * Eigvec.col(0) * Eigvec.col(0).transpose() ) + ( (Eigval(1,0)) * Eigvec.col(1) * Eigvec.col(1).transpose() )+ ( (Eigval(2,0)) * Eigvec.col(2) * Eigvec.col(2).transpose() );


            for(int Neighbor_index=0; Neighbor_index<Neighbor_num; Neighbor_index++)
            {
                U_j.block(Neighbor_index*3,0,3,3) = U.block(MapNeighbor[Vert_index][Neighbor_index]*3,0,3,3);
            }
            // we sum up for all neighboring vertices
            U_sum.Zero(3,3);
            for(int Neighbor_index=0; Neighbor_index<Neighbor_num; Neighbor_index++)
            {
                // Logarithm of a matrix
                // https://en.wikipedia.org/wiki/Logarithm_of_a_matrix
                // U_tmp = ((U_i.inverse() * U_j.block(Neighbor_index*3,0,3,3) * U_i.inverse() - Eigen::MatrixXd::Identity(3, 3)) - 1./2.*(U_i.inverse() * U_j.block(Neighbor_index*3,0,3,3) * U_i.inverse() - Eigen::MatrixXd::Identity(3, 3))*(U_i.inverse() * U_j.block(Neighbor_index*3,0,3,3) * U_i.inverse() - Eigen::MatrixXd::Identity(3, 3)) ) * weight(Neighbor_index);
                U_tmp = (U_i.inverse() * U_j.block(Neighbor_index*3,0,3,3) * U_i.inverse() - Eigen::MatrixXd::Identity(3, 3)) * weight(Neighbor_index);

                U_sum = U_sum + U_tmp;
                U_tmp.resize(0,0);
            }

            // Exponential of a matrix
            // https://en.wikipedia.org/wiki/Matrix_exponential
            // U_new = Eigen::MatrixXd::Identity(3, 3) + U_sum + U_sum * U_sum / 2.;
            U_new = Eigen::MatrixXd::Identity(3, 3) + U_sum;

            this -> U_3D_assem.block(Vert_index*3,0,3,3) = U_i * U_new * U_i;

            // erase the matrices
            U_i.resize(0,0);
            U_j.resize(0,0);
            U_sum.resize(0,0);
            U_tmp.resize(0,0);
            U_new.resize(0,0);
            weight.resize(0,0);
            Eigval.resize(0,0);
            Eigvec.resize(0,0);
        }
        else
        {
            this -> U_3D_assem.block(Vert_index*3,0,3,3) = U.block(Vert_index*3,0,3,3);
        }
        // Test << weight << std::endl;
    }

    // Test.close();
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
    // affine-invariant Riemannian metric
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    cloth_vec();
    // cloth_init_neighbor();

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
                L_tmp = (L_i.inverse().cwiseSqrt() * L_j.block(El_index*3,0,3,3) * L_i.inverse().cwiseSqrt()).array().log10() * weight(El_index);
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
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    // Linear BLend Simulation
    /*
    cloth_init_vert();

    // initialize the weights matrix
    Eigen::MatrixXd weights;

    // initialize the eigenvalues and eigenvectors
    int Vert_num = verts.rows();
    this -> D_assem.resize(Vert_num*3,3);

    std::vector<double> query_pt(3);

    typedef nanoflann::KDTreeEigenMatrixAdaptor<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>> kd_tree;

    kd_tree vert_index(3, std::cref(verts),10); // max leaf
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
    */

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

void cloth_calc::cloth_rotationTensor(Eigen::MatrixXd F, Eigen::MatrixXd U)
{
    int Vert_num = this -> vertsT.rows();
    this -> R.resize(Vert_num*3,3);

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++)
    {
        this -> R.block(Vert_index*3,0,3,3) = F.block(Vert_index*3,0,3,3) * U.block(Vert_index*3,0,3,3).inverse();
    }


}

void cloth_calc::cloth_translationVec(Eigen::MatrixXd R, std::map<int, std::vector<int>> MapNeighbor)
{
    int Vert_num = this -> vertsT.rows();
    this -> t.resize(Vert_num*3,1);

    // calculate the centre of mess
    this -> vertsT_cog.resize(Vert_num,3);
    this -> vertsR_cog.resize(Vert_num,3);

    // Eigen::MatrixXd verts_sum;
    double xT,yT,zT,xR,yR,zR;

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all Vertice
    {
        /*
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

                Neighbor3x_num = _plyMesh -> trimesh::TriMesh::neighbors.at(Neighbor2x_Vert_index).size();
                // store the index of the 2nd Neighbor vertex index
                Neighbor_Vert.push_back(Neighbor2x_Vert_index);

                // evaluate the 3rd Neighbor
                for(int Neighbor3x_index=0; Neighbor3x_index<Neighbor3x_num; Neighbor3x_index++) //for all neighbor in each neighbor2x vertex
                {
                    // 2nd Neighbor vertex index
                    int Neighbor3x_Vert_index = _plyMeshT -> trimesh::TriMesh::neighbors.at(Neighbor2x_Vert_index).at(Neighbor3x_index);

                    Neighbor4x_num = _plyMesh -> trimesh::TriMesh::neighbors.at(Neighbor3x_Vert_index).size();
                    // store the index of the 3rd Neighbor vertex index
                    Neighbor_Vert.push_back(Neighbor3x_Vert_index);

                    for(int Neighbor4x_index=0; Neighbor4x_index<Neighbor4x_num; Neighbor4x_index++) //for all neighbor in each neighbor2x vertex
                    {
                        // 2nd Neighbor vertex index
                        int Neighbor4x_Vert_index = _plyMeshT -> trimesh::TriMesh::neighbors.at(Neighbor3x_Vert_index).at(Neighbor4x_index);

                        // store the index of the 3rd Neighbor vertex index
                        Neighbor_Vert.push_back(Neighbor4x_Vert_index);
                    }
                }
            }
        }

        // erase the doppel vertex index
        sort(Neighbor_Vert.begin(), Neighbor_Vert.end());
        Neighbor_Vert.erase(unique(Neighbor_Vert.begin(), Neighbor_Vert.end()), Neighbor_Vert.end());
        */

        int Neighbor_Vert_size = MapNeighbor[Vert_index].size();

        if(Neighbor_Vert_size == 0)
        {
            this -> vertsT_cog.row(Vert_index) = verts.row(Vert_index);
        }
        else
        {
            xT=0;
            yT=0;
            zT=0;
            xR=0;
            yR=0;
            zR=0;
            for(int i=0; i<Neighbor_Vert_size; i++)
            {
              xT = xT + this -> vertsT(MapNeighbor[Vert_index][i],0);
              yT = yT + this -> vertsT(MapNeighbor[Vert_index][i],1);
              zT = zT + this -> vertsT(MapNeighbor[Vert_index][i],2);
              xR = xR + this -> vertsR(MapNeighbor[Vert_index][i],0);
              yR = yR + this -> vertsR(MapNeighbor[Vert_index][i],1);
              zR = zR + this -> vertsR(MapNeighbor[Vert_index][i],2);

            }
            this -> vertsT_cog.row(Vert_index) << xT / Neighbor_Vert_size, yT / Neighbor_Vert_size, zT / Neighbor_Vert_size;
            this -> vertsR_cog.row(Vert_index) << xR / Neighbor_Vert_size, yR / Neighbor_Vert_size, zR / Neighbor_Vert_size;
        }

        this -> t.block(Vert_index*3,0,3,1) << this -> vertsR_cog.row(Vert_index).transpose() - R.block(Vert_index*3,0,3,3) * this -> vertsT_cog.row(Vert_index).transpose();
    }

}

void cloth_calc::cloth_transformationMat(Eigen::MatrixXd R, Eigen::MatrixXd t)
{
    int Vert_num = this -> vertsT.rows();
    this -> T.resize(Vert_num*3,4);
    for(int Vert_index=0; Vert_index<Vert_num*3; Vert_index++)
    {
        this -> T.row(Vert_index) << R.row(Vert_index), t(Vert_index);
    }
}

void cloth_calc::cloth_update(Eigen::MatrixXd R, Eigen::MatrixXd t)
{
    int Vert_num = this -> vertsT.rows();
    this -> vertsUpdate.resize(Vert_num, 3);
    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++)
    {
        this -> vertsUpdate.row(Vert_index).transpose() = (R.block(Vert_index*3,0,3,3) * this->vertsT.row(Vert_index).transpose()) + t.block(Vert_index*3,0,3,1);
        if(isnan(this->vertsUpdate(Vert_index,0)))
        {
            this -> vertsUpdate.row(Vert_index).transpose() << 0,0,0;
        }
    }
}

void cloth_calc::cloth_WriteVerts(Eigen::MatrixXd update, const std::string &ifileName)
{
    // set the color and write as ply files
    plyUpdate = new ply_module();

    int update_num = update.rows();

    // set pink color as default
    Eigen::MatrixXi pink;
    pink.resize(update_num,3);
    for(int i=0;i<update_num;i++)
    {
        pink.row(i) << 255, 0, 255;
    }

    // set vertice and colors
    plyUpdate -> setVertices(update);
    plyUpdate -> setFaces(faces);
    plyUpdate -> setColors(pink);

    plyUpdate -> writePLY(ifileName, true, true, true, false, true);

}


void cloth_calc::cloth_eig_neighbor(std::map<int, std::vector<int>> MapNeighbor)
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
    // cloth_init_neighbor();

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
        int Neighbor_num = MapNeighbor[Vert_index].size();

        P.resize(Neighbor_num, 3);
        Q.resize(Neighbor_num, 3);

        for(int Neighbor_index=0; Neighbor_index < Neighbor_num; Neighbor_index++) // for all neighbors in each vertex
        {
            // for template
            P.row(Neighbor_index) << vertsT.row(MapNeighbor[Vert_index][Neighbor_index]) - vertsT.row(Vert_index);
            // for reference
            Q.row(Neighbor_index) << vertsR.row(MapNeighbor[Vert_index][Neighbor_index]) - vertsR.row(Vert_index);

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
    // cloth_eig_neighbor();

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

void cloth_calc::cloth_eig_neighbor2x(std::map<int, std::vector<int>> MapNeighbor)
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

    int Eig_index = 0;

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all Vertice
    {
        // face has the structure of face(i.j)
        // where i is the i-th element, j is the j-th vertex of i-th element

        // base vertex index
        // Vert_index

        // size of the 1st Neighbor vertice

        H.resize(3,3);
        C.resize(3,3);

        int Neighbor_num = MapNeighbor[Vert_index].size();

        P.resize(Neighbor_num, 3);
        Q.resize(Neighbor_num, 3);

        for(int Neighbor_index=0; Neighbor_index < Neighbor_num; Neighbor_index++) // for all neighbors in each vertex
        {
            // for template
            P.row(Neighbor_index) << vertsT.row(MapNeighbor[Vert_index][Neighbor_index]) - vertsT.row(Vert_index);
            // for reference
            Q.row(Neighbor_index) << vertsR.row(MapNeighbor[Vert_index][Neighbor_index]) - vertsR.row(Vert_index);

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

        Eig_index = Eig_index+3;
    }

}

void cloth_calc::cloth_eig_neighbor3x(std::map<int, std::vector<int>> MapNeighbor)
{
    cloth_init_vert();

    // initialize the points set P(reference) and Q(template)
    Eigen::MatrixXd P, Q;
    Eigen::MatrixXd H;
    Eigen::MatrixXd C;

    // initialize the eigenvalues and eigenvectors
    int Vert_num = verts.rows();
    this -> Eigval_neighbor3x.resize(Vert_num*3,1);
    this -> Eigvec_neighbor3x.resize(Vert_num*3,3);
    this -> F.resize(Vert_num*3,3);

    int Eig_index = 0;

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all Vertice
    {
        H.resize(3,3);
        C.resize(3,3);

        int Neighbor_num = MapNeighbor[Vert_index].size();

        P.resize(Neighbor_num, 3);
        Q.resize(Neighbor_num, 3);

        for(int Neighbor_index=0; Neighbor_index < Neighbor_num; Neighbor_index++) // for all neighbors in each vertex
        {
            // for template
            P.row(Neighbor_index) << vertsT.row(MapNeighbor[Vert_index][Neighbor_index]) - vertsT.row(Vert_index);
            // for reference
            Q.row(Neighbor_index) << vertsR.row(MapNeighbor[Vert_index][Neighbor_index]) - vertsR.row(Vert_index);

        }

        H = P.transpose() * Q;
        C = H.transpose() * H;
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv(C);

        this -> Eigval_neighbor3x.block(Eig_index,0,3,1) << solv.eigenvalues().cwiseSqrt();
        this -> Eigvec_neighbor3x.block(Eig_index,0,3,3) << solv.eigenvectors();
        this -> F.block(Eig_index,0,3,3) << H;

        // initialize
        P.resize(0,0);
        Q.resize(0,0);
        H.resize(0,0);
        C.resize(0,0);

        Eig_index = Eig_index+3;
    }
}

void cloth_calc::cloth_eig_neighbor4x(std::map<int, std::vector<int>> MapNeighbor)
{
    cloth_init_vert();

    // initialize the points set P(reference) and Q(template)
    Eigen::MatrixXd P, Q;
    Eigen::MatrixXd H;
    Eigen::MatrixXd C;

    // initialize the eigenvalues and eigenvectors
    int Vert_num = verts.rows();
    this -> Eigval_neighbor4x.resize(Vert_num*3,1);
    this -> Eigvec_neighbor4x.resize(Vert_num*3,3);
    this -> F.resize(Vert_num*3,3);

    int Eig_index = 0;

    for(int Vert_index=0; Vert_index<Vert_num; Vert_index++) // for all Vertice
    {

        H.resize(3,3);
        C.resize(3,3);

        int Neighbor_num = MapNeighbor[Vert_index].size();

        P.resize(Neighbor_num, 3);
        Q.resize(Neighbor_num, 3);

        for(int Neighbor_index=0; Neighbor_index < Neighbor_num; Neighbor_index++) // for all neighbors in each vertex
        {
            // for template
            P.row(Neighbor_index) << vertsT.row(MapNeighbor[Vert_index][Neighbor_index]) - vertsT.row(Vert_index);
            // for reference
            Q.row(Neighbor_index) << vertsR.row(MapNeighbor[Vert_index][Neighbor_index]) - vertsR.row(Vert_index);

        }

        H = P.transpose() * Q;
        C = H.transpose() * H;
        Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solv(C);

        this -> Eigval_neighbor4x.block(Eig_index,0,3,1) << solv.eigenvalues().cwiseSqrt();
        this -> Eigvec_neighbor4x.block(Eig_index,0,3,3) << solv.eigenvectors();
        this -> F.block(Eig_index,0,3,3) << H;

        // initialize
        P.resize(0,0);
        Q.resize(0,0);
        H.resize(0,0);
        C.resize(0,0);

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
    // cloth_init_vert();

    // set the color and write as ply files
    this -> plyUpdate = new ply_module();

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
    plyUpdate -> setVertices(verts);
    plyUpdate -> setFaces(faces);
    plyUpdate -> setColors(pink);
    plyUpdate -> setCurvatures(Eigval_norm);


    plyUpdate -> writePLY(ifileName, true, true, true, true, false);

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

Eigen::MatrixXd cloth_calc::GetStretchTensorAsemmble()
{
    return this -> U_3D_assem;
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

Eigen::MatrixXd cloth_calc::GetEigval_neighbor3x()
{
    return this -> Eigval_neighbor3x;
}

Eigen::MatrixXd cloth_calc::GetEigvec_neighbor3x()
{
    return this -> Eigvec_neighbor3x;
}

Eigen::MatrixXd cloth_calc::GetEigval_neighbor4x()
{
    return this -> Eigval_neighbor4x;
}

Eigen::MatrixXd cloth_calc::GetEigvec_neighbor4x()
{
    return this -> Eigvec_neighbor4x;
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

Eigen::MatrixXd cloth_calc::GetRotationTensor()
{
    return this -> R;
}

Eigen::MatrixXd cloth_calc::GetTranslationVec()
{
    return this -> t;
}

Eigen::MatrixXd cloth_calc::GetTransformationMat()
{
    return this -> T;
}

Eigen::MatrixXd cloth_calc::GetVertsUpdate()
{
    return this -> vertsUpdate;
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


std::map<int, std::vector<int> > cloth_calc::GetMapNeighbor()
{
    return this -> mapNeighbor;
}

std::map<int, std::vector<int> > cloth_calc::GetMapNeighbor2x()
{
    return this -> mapNeighbor2x;
}

std::map<int, std::vector<int> > cloth_calc::GetMapNeighbor3x()
{
    return this -> mapNeighbor3x;
}

std::map<int, std::vector<int> > cloth_calc::GetMapNeighbor4x()
{
    return this -> mapNeighbor4x;
}

std::map<int, std::vector<size_t> > cloth_calc::GetMapNeighborKdTree()
{
    return this -> mapNeighborKdTree;
}

std::map<int, std::vector<int> > cloth_calc::GetMapAdjacent()
{
    return this -> mapAdjacent;
}

cloth_calc::~cloth_calc()
{

}



