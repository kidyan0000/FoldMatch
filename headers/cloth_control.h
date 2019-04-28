#ifndef CLOTH_CONTROL_H
#define CLOTH_CONTROL_H

#include <boost/filesystem.hpp>

#include <iostream>
#include <string>

class cloth_control
{
public:
    cloth_control();

    void cloth_lambda(std::string lambda);
    void cloth_input(std::string inputpath);
    void cloth_lambda_output(std::string outputpath);
    void cloth_stretch_output(std::string outputpath);
    void cloth_wrinkVecField_output(std::string outputpath);

    static bool compare(std::string a, std::string b);

    std::string GetInput(int i);
    std::string GetLambda();
    std::string GetLambdaOutput(int i);
    std::string GetStretchOutput(int i);
    std::string GetWrinkVecFieldOutput(int i);
    std::string Readme(int i);

private:
    std::vector<std::string> _inputname, _lambdaname, _stretchname, _wrinkVecFieldname;
    std::vector<std::string> _readme;
    std::string lambda;
};

#endif // CLOTH_CONTROL_H
