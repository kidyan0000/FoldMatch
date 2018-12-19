#include "ply_defo.h"
#include <vector>

// this is to use to calculate the deformation gradient


void ply_defo::ply_getData()
{

    std::vector<float> verts;

    // get ply datas
    ply_defo* _ply_Defo = new ply_defo();
    _ply_Defo->readPLY("../data/bunny.ply", true, true, true, false, true, verts);

    // print the datas
    for (std::vector<float>::iterator it = verts.begin(); it != verts.end(); it++)
    {
        std::cout << *it << std::endl;
    }


}

void ply_defo::ply_CalVec()
{


}
