#include "cloth_control.h"

cloth_control::cloth_control()
{

}

void cloth_control::cloth_lambda(std::string lambda)
{
    this->lambda = lambda;
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

std::string cloth_control::GetStretchOutput(int i)
{
    return this -> _stretchname[i];
}

std::string cloth_control::GetWrinkVecFieldOutput(int i)
{
    return this -> _wrinkVecFieldname[i];
}

std::string cloth_control::Readme(int i)
{
    return this -> _readme[i];
}
