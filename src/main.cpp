#include "ply_Module.h"

int main(int* argc, char** argv)
{
    ply_module* _plyModuleT = new ply_module();
    ply_module* _plyModuleR = new ply_module();
    _plyModuleT->readPLY("../data/elephant.ply", true, true, true, false, true);
    _plyModuleR->readPLY("../data/elephant.ply", true, true, true, false, true);
}
