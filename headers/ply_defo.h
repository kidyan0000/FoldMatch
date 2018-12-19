#ifndef PLY_DEFO_H
#define PLY_DEFO_H

// this is to use to calculate the deformation gradient

#include "ply_Module.h"
#include <vector>

class ply_defo : public ply_module
{
public:

    void ply_getData();
    void ply_CalVec();

};

#endif // PLY_DEFO_H
