#include "cloth_window.h"
// #include "cloth_glwidget.h"
#include "cloth_calc.h"

#include <boost/filesystem.hpp>
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // close window does not quit all programm
    // QApplication::setQuitOnLastWindowClosed(false);

    Cloth_Window w;

    // read all the ply in current folde
    std::vector<std::string> filename_;
    std::string current_path = "../data";
    boost::filesystem::path path(current_path);
    boost::filesystem::recursive_directory_iterator end_iter;
    std::string current_file;

    // search through all file
    for(boost::filesystem::recursive_directory_iterator iter(current_path);iter!=end_iter;iter++)
    {
        current_file = iter -> path().filename().string();
        current_file = current_path + current_file;
        filename_.push_back(current_file);

    }
    std::sort(filename_.begin(),filename_.end());
    std::cout<< filename_[2] << std::endl;

    cloth_calc* test = new cloth_calc("../data/Template-1_0001_bi.ply", "../data/Template-5_0005_bi.ply");

    // THIS IS FOR NEIGHBOR

    test -> cloth_eig_neighbor();
    Eigen::MatrixXd val = test->GetEigval_neighbor();
    test -> cloth_vec_normalize(val, 3);


    // WRITE AS PLY
    // test -> cloth_WriteColor(test->GetEigval_norm_dir3(), "../output/debug/lambda3.ply");

    std::ofstream outfile("../output/debug/Test.txt");
    outfile<< test->GetEigval_neighbor() <<std::endl;
    outfile.close();

    // test->test();

    w.show();

    return a.exec();


}
