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
    void cloth_lambdaAssemble(std::string lambda);
    void cloth_input(std::string inputpath);
    void cloth_lambda_output(std::string outputpath);
    void cloth_lambdaAssemble_output(std::string outputpath);
    void cloth_stretchCCM_output(std::string outputpath);
    void cloth_stretchFreq_output(std::string outputpath);
    void cloth_stretchMap_output(std::string outputpath);
    void cloth_stretch_output(std::string outputpath);
    void cloth_wrinkVecField_output(std::string outputpath);
    void cloth_vertsUpdate_input(std::string inputpath);
    void cloth_vertsUpdate_output(std::string outputpath);

    static bool compare(std::string a, std::string b);

    std::string GetInput(int i);
    std::string GetInputTransf(int i);
    std::string GetInputFreq(int i);
    std::string GetLambda();
    std::string GetLambdaAssemble();
    std::string GetLambdaOutput(int i);
    std::string GetLambdaAssembleOutput(int i);
    std::string GetStretchOutput(int i);
    std::string GetStretchCCMOutput(int i);
    std::string GetStretchFreqOutput(int i);
    std::string GetStretchMapOutput(int i);
    std::string GetWrinkVecFieldOutput(int i);
    std::string GetVertsUpdateInput(int i);
    std::string GetVertsUpdateOutput(int i);
    std::string Readme(int i);

private:
    std::vector<std::string> _inputname, _lambdaname, _lambdaAssemblename, _stretchname, _wrinkVecFieldname, _input_vertsUpdatename, _output_vertsUpdatename;
    std::vector<std::string> _stretchCCMname, _stretchFreqname, _stretchMapname;
    std::vector<std::string> _inputname_transf, _inputname_freq;
    std::vector<std::string> _readme;
    std::string lambda;
    std::string lambda_assemble;
};

#endif // CLOTH_CONTROL_H
