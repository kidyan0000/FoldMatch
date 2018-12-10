/*
* P.Thesis Seamless Cloth Simulation with High Geometric Fidelity 
*/

#include <iostream>
#include <stdlib.h>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>

class ply_module
{
public:
	ply_module();
	virtual ~ply_module();
	
	bool readPLY(const std::string &  ifileName, bool readColorsFlag, bool readFacesFlag, bool readNormalsFlag, bool readFaceTextureFlag, bool readCurvatureFlag);
	bool writePLY(const std::string & oFileName, bool writeColorsFlag, bool writeFacesFlag, bool writeNormalsFlag, bool writeCurvatureFlag, bool isBinary);

	int getNumberOfVertices();
	int getNumberOfFaces();
	int getNumberOfNormals();

	Eigen::VectorXi getIndices();
	Eigen::MatrixXi getColors();
	Eigen::MatrixXd getNormals();
	Eigen::MatrixXd getVertices();
	Eigen::MatrixXi getFaces();
	Eigen::MatrixXd getCurvatures();

	void setIndices(Eigen::VectorXi indices);
	void setColors(Eigen::MatrixXi colors);
	void setNormals(Eigen::MatrixXd normals);
	void setVertices(Eigen::MatrixXd vertices);
	void setFaces(Eigen::MatrixXi faces);
	void setCurvatures(Eigen::MatrixXd curvatures);
	
	int setReadColorFlag();
	int setReadNormalFlag();
	int setReadFacesFlag();
	int setReadCameraFlag();
	int setReadCurvaturesFlag();

	
protected:

private:
	
	Eigen::MatrixXd normals;
	Eigen::MatrixXd vertices;
	Eigen::MatrixXd curvatures;
	Eigen::MatrixXd textureCoords;
	Eigen::MatrixXi faces;
	Eigen::MatrixXi colors;

	bool readColorFlag;
	bool readNormalFlag;
	bool readFacesFlag;
	bool readCameraFlag;

};
