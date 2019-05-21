#include "cloth_control.h"

cloth_control::cloth_control()
{

}

void cloth_control::cloth_lambda(std::string lambda)
{
    this -> lambda = lambda;
}

void cloth_control::cloth_lambdaAssemble(std::string lambda)
{
    this -> lambda_assemble = lambda;
}

bool cloth_control::compare(std::string a, std::string b)
{
    std::string number;
    int size_a = a.size();
    int size_b = b.size();

    for(int i=0; i<size_a;i++)
    {
        if(isdigit(a[i]))
        {
            number += a[i];
        }
    }
    long num_a = stol(number);
    number.clear();

    for(int i=0; i<size_b;i++)
    {
        if(isdigit(b[i]))
        {
            number += b[i];
        }
    }
    long num_b = stol(number);
    number.clear();

    return num_a < num_b;
}

void cloth_control::cloth_input(std::string inputpath)
{
    std::string input_file, output_file;

    boost::filesystem::path path(inputpath);
    boost::filesystem::recursive_directory_iterator end_iter;

    for(boost::filesystem::recursive_directory_iterator iter(inputpath);iter!=end_iter;iter++)
    {
        input_file = iter -> path().filename().string();
        input_file = inputpath + input_file;
        this -> _inputname.push_back(input_file);


    }

    std::sort(_inputname.begin(),_inputname.end(), compare);

    std::ofstream input_list("../output/input_list.txt");
    for(std::vector<std::string>::iterator it = _inputname.begin(); it != _inputname.end(); ++it) {
        input_list<<  *it <<std::endl;
    }
    input_list.close();
}

void cloth_control::cloth_lambda_output(std::string outputpath)
{
    std::string output_file;
    for(int i=1; i<=75; i++)
    {
        output_file = outputpath + lambda + "_" + std::to_string(i) +".ply";
        this -> _lambdaname.push_back(output_file);
    }
    std::string readme_file;
    for(int i=1; i<=75; i++)
    {
        readme_file = outputpath + "readme/" + lambda + "_" + std::to_string(i) + "_readme.txt";
        this -> _readme.push_back(readme_file);
    }

    std::ofstream output_list("../output/output_list.txt");
    for(std::vector<std::string>::iterator it = _lambdaname.begin(); it != _lambdaname.end(); ++it) {
        output_list<<  *it <<std::endl;
    }
    output_list.close();
}

void cloth_control::cloth_lambdaAssemble_output(std::string outputpath)
{
    std::string output_file;
    for(int i=1; i<=75; i++)
    {
        output_file = outputpath + lambda_assemble + "_" + std::to_string(i) +".ply";
        this -> _lambdaAssemblename.push_back(output_file);
    }
    std::string readme_file;
    for(int i=1; i<=75; i++)
    {
        readme_file = outputpath + "readme/" + lambda_assemble + "_" + std::to_string(i) + "_readme.txt";
        this -> _readme.push_back(readme_file);
    }

    std::ofstream output_list("../output/output_list.txt");
    for(std::vector<std::string>::iterator it = _lambdaAssemblename.begin(); it != _lambdaAssemblename.end(); ++it) {
        output_list<<  *it <<std::endl;
    }
    output_list.close();

}

void cloth_control::cloth_stretchCCM_output(std::string outputpath)
{
    std::string output_file;
    for(int i=1; i<=75; i++)
    {
        output_file = outputpath + "stretchCCM" + "_" + std::to_string(i) +".txt";
        this -> _stretchCCMname.push_back(output_file);
    }
    std::string readme_file;
    for(int i=1; i<=75; i++)
    {
        readme_file = outputpath + "readme/" + "stretchCCM" + "_" + std::to_string(i) + "_readme.txt";
        this -> _readme.push_back(readme_file);
    }

    std::ofstream output_list("../output/output_list.txt");
    for(std::vector<std::string>::iterator it = _stretchCCMname.begin(); it != _stretchCCMname.end(); ++it) {
        output_list<<  *it <<std::endl;
    }
    output_list.close();

}

void cloth_control::cloth_stretchFreq_output(std::string outputpath)
{
    std::string output_file;
    for(int i=1; i<=75; i++)
    {
        output_file = outputpath + "stretchFreq" + "_" + std::to_string(i) +".txt";
        this -> _stretchFreqname.push_back(output_file);
    }

    std::string readme_file;
    for(int i=1; i<=75; i++)
    {
        readme_file = outputpath + "readme/" + "stretchFreq" + "_" + std::to_string(i) + "_readme.txt";
        this -> _readme.push_back(readme_file);
    }

    std::ofstream output_list("../output/output_list.txt");
    for(std::vector<std::string>::iterator it = _stretchFreqname.begin(); it != _stretchFreqname.end(); ++it) {
        output_list<<  *it <<std::endl;
    }
    output_list.close();

}

void cloth_control::cloth_stretchMap_output(std::string outputpath)
{
    std::string output_file;
    for(int i=1; i<=75; i++)
    {
        output_file = outputpath + "stretchMap" + "_" + std::to_string(i) +".txt";
        this -> _stretchMapname.push_back(output_file);
    }
    std::string readme_file;
    for(int i=1; i<=75; i++)
    {
        readme_file = outputpath + "readme/" + "stretchMap" + "_" + std::to_string(i) + "_readme.txt";
        this -> _readme.push_back(readme_file);
    }

    std::ofstream output_list("../output/output_list.txt");
    for(std::vector<std::string>::iterator it = _stretchMapname.begin(); it != _stretchMapname.end(); ++it) {
        output_list<<  *it <<std::endl;
    }
    output_list.close();

}

void cloth_control::cloth_stretch_output(std::string outputpath)
{
    std::string output_file;
    for(int i=1; i<=75; i++)
    {
        output_file = outputpath + "stretch" + "_" + std::to_string(i) +".ply";
        this -> _stretchname.push_back(output_file);
    }
    std::string readme_file;
    for(int i=1; i<=75; i++)
    {
        readme_file = outputpath + "readme/" + "stretch_" + std::to_string(i) + "_readme.txt";
        this -> _readme.push_back(readme_file);
    }

    std::ofstream output_list("../output/output_list.txt");
    for(std::vector<std::string>::iterator it = _stretchname.begin(); it !=_stretchname.end(); ++it) {
        output_list<<  *it <<std::endl;
    }
    output_list.close();

}

void cloth_control::cloth_wrinkVecField_output(std::string outputpath)
{
    std::string output_file;
    for(int i=1; i<=75; i++)
    {
        output_file = outputpath + "wrinkVecField" + "_" + std::to_string(i) +".ply";
        this -> _wrinkVecFieldname.push_back(output_file);
    }
    std::string readme_file;
    for(int i=1; i<=75; i++)
    {
        readme_file = outputpath + "readme/" + "wrinkVecField_" + std::to_string(i) + "_readme.txt";
        this -> _readme.push_back(readme_file);
    }

    std::ofstream output_list("../output/output_list.txt");
    for(std::vector<std::string>::iterator it = _wrinkVecFieldname.begin(); it !=_wrinkVecFieldname.end(); ++it) {
        output_list<<  *it <<std::endl;
    }
    output_list.close();
}

void cloth_control::cloth_vertsUpdate_input(std::string inputpath)
{
    std::string input_file, output_file;

    boost::filesystem::path path(inputpath);
    boost::filesystem::recursive_directory_iterator end_iter;

    for(boost::filesystem::recursive_directory_iterator iter(inputpath);iter!=end_iter;iter++)
    {
        input_file = iter -> path().filename().string();
        input_file = inputpath + input_file;
        this -> _input_vertsUpdatename.push_back(input_file);
    }

    std::sort(_inputname.begin(),_inputname.end(), compare);

    std::ofstream input_list("../output/update_list.txt");
    for(std::vector<std::string>::iterator it = _input_vertsUpdatename.begin(); it != _input_vertsUpdatename.end(); ++it) {
        input_list<<  *it <<std::endl;
    }
    input_list.close();

}

void cloth_control::cloth_vertsUpdate_output(std::string outputpath)
{
    std::string output_file;
    for(int i=1; i<=75; i++)
    {
        output_file = outputpath + "Template-Update" + "_" + std::to_string(i) +".ply";
        this -> _output_vertsUpdatename.push_back(output_file);
    }

    std::string readme_file;
    for(int i=1; i<=75; i++)
    {
        readme_file = outputpath + "readme/" + "Template-Update_" + std::to_string(i) + "_readme.txt";
        this -> _readme.push_back(readme_file);
    }

    std::ofstream output_list("../output/Template-Update_output_list.txt");
    for(std::vector<std::string>::iterator it = _output_vertsUpdatename.begin(); it !=_output_vertsUpdatename.end(); ++it) {
        output_list<<  *it <<std::endl;
    }
    output_list.close();

}



std::string cloth_control::GetInput(int i)
{
    return this -> _inputname[i];
}

std::string cloth_control::GetLambdaOutput(int i)
{
    return this -> _lambdaname[i];
}

std::string cloth_control::GetLambda()
{
    return this -> lambda;
}

std::string cloth_control::GetLambdaAssemble()
{
    return this -> lambda_assemble;
}

std::string cloth_control::GetLambdaAssembleOutput(int i)
{
    return this -> _lambdaAssemblename[i];
}

std::string cloth_control::GetStretchOutput(int i)
{
    return this -> _stretchname[i];
}

std::string cloth_control::GetStretchCCMOutput(int i)
{
    return this -> _stretchCCMname[i];
}

std::string cloth_control::GetStretchFreqOutput(int i)
{
    return this -> _stretchFreqname[i];
}

std::string cloth_control::GetStretchMapOutput(int i)
{
    return this -> _stretchMapname[i];
}

std::string cloth_control::GetWrinkVecFieldOutput(int i)
{
    return this -> _wrinkVecFieldname[i];
}

std::string cloth_control::GetVertsUpdateInput(int i)
{
    return this -> _input_vertsUpdatename[i];
}

std::string cloth_control::GetVertsUpdateOutput(int i)
{
    return this -> _output_vertsUpdatename[i];
}

std::string cloth_control::Readme(int i)
{
    return this -> _readme[i];
}
