#include "cloth_window.h"
// #include "cloth_glwidget.h"
#include "cloth_calc.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <string>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // close window does not quit all programm
    // QApplication::setQuitOnLastWindowClosed(false);

    Cloth_Window w;

    //////////////////////////
    /// SET THE SIMULATION ///
    //////////////////////////
    // read all the ply in data folder
    std::vector<std::string> _filename, _plyname;
    std::string lambda = "/lambda1.ply"; // please change the name for different lambda
    std::string data_path   = "../data/";
    std::string output_path = "../output/debug/regulation-";
    std::string data_file, output_file;

    boost::filesystem::path path(data_path);
    boost::filesystem::recursive_directory_iterator end_iter;

    for(boost::filesystem::recursive_directory_iterator iter(data_path);iter!=end_iter;iter++)
    {
        data_file = iter -> path().filename().string();
        data_file = data_path + data_file;
        _filename.push_back(data_file);


    }
    std::sort(_filename.begin(),_filename.end());

    for(int i=1; i<70; i++)
    {
        output_file = output_path + std::to_string(i) + lambda;
        _plyname.push_back(output_file);
    }
    std::ofstream filename_list("../output/filename_list.txt");
    for(std::vector<std::string>::iterator it = _filename.begin(); it != _filename.end(); ++it) {
        filename_list<<  *it <<std::endl;
    }
    filename_list.close();
    std::ofstream output_list("../output/output_list.txt");
    for(std::vector<std::string>::iterator it = _plyname.begin(); it != _plyname.end(); ++it) {
        output_list<<  *it <<std::endl;
    }
    output_list.close();

    ////////////////////////////////
    /// NOW START THE SIMULATION ///
    ////////////////////////////////
    cloth_calc* cloth = new cloth_calc("../data/Template-1_0001_bi.ply", "../data/Template-5_0005_bi.ply");

    cloth -> cloth_eig_neighbor();
    Eigen::MatrixXd val = cloth->GetEigval_neighbor();
    cloth -> cloth_vec_normalize(val, 3);

    // cloth -> cloth_WriteColor(test->GetEigval_norm_dir3(), "../output/debug/lambda3.ply");

    /////////////////////////
    /// THIS IS FOR DEBUG ///
    /////////////////////////
    // std::ofstream outfile("../output/debug/filename.txt");
    // outfile<<  _filename[i] <<std::endl;
    // outfile.close();

    w.show();

    return a.exec();


}
