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

void cloth_control::cloth_output(std::string outputpath)
{
    std::string output_file;
    for(int i=1; i<75; i++)
    {
        output_file = outputpath + std::to_string(i) + lambda;
        this -> _outputname.push_back(output_file);
    }

    std::ofstream output_list("../output/output_list.txt");
    for(std::vector<std::string>::iterator it = _outputname.begin(); it != _outputname.end(); ++it) {
        output_list<<  *it <<std::endl;
    }
    output_list.close();
}


std::string cloth_control::GetInput(int i)
{
    return this -> _inputname[i];
}

std::string cloth_control::GetOutput(int i)
{
    return this -> _outputname[i];
}
