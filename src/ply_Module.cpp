#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <ctime>

#include "tinyply.h"
#include "ply_Module.h"

using namespace tinyply;

typedef std::chrono::time_point<std::chrono::high_resolution_clock> timepoint;
std::chrono::high_resolution_clock ck;

inline std::chrono::time_point<std::chrono::high_resolution_clock> now()
{
	return ck.now();
}

inline double difference_micros(timepoint start, timepoint end)
{
	return (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

// DEFAULT CONSTRUCTORS
ply_module::ply_module()
{
	this->vertices = Eigen::MatrixXd(0, 0);
	this->normals = Eigen::MatrixXd(0, 0);
	this->curvatures = Eigen::MatrixXd(0, 0);
	this->textureCoords = Eigen::MatrixXd(0, 0);
	this->faces = Eigen::MatrixXi(0, 0);
	this->colors = Eigen::MatrixXi(0, 0);
}


// GETTER FUNCTIONS
Eigen::MatrixXd ply_module::getVertices()
{
	return this->vertices;
}

Eigen::MatrixXd ply_module::getNormals()
{
	return this->normals;
}

Eigen::MatrixXd ply_module::getCurvatures()
{
	return this->curvatures;
}

Eigen::MatrixXi ply_module::getColors()
{
	return this->colors;
}

Eigen::MatrixXi ply_module::getFaces()
{
	return this->faces;
}


// SETTER FUNCTIONS 
void ply_module::setVertices(Eigen::MatrixXd vertices)
{
	if (vertices.size() > 0)
	{
		this->vertices.resize(0,0);
		this->vertices = vertices;
	}
	else
	{
		this->vertices.resize(0, 0);
	}
}

void ply_module::setNormals(Eigen::MatrixXd normals)
{
	if (normals.size() > 0)
	{
		this->normals.resize(0, 0);
		this->normals = normals;
	}
	else
	{
		this->normals.resize(0, 0);
	}
}

void ply_module::setCurvatures(Eigen::MatrixXd curvatures)
{
  	if (curvatures.size() > 0)
	{
		this->curvatures.resize(0, 0);
		this->curvatures = curvatures;
	}
	else
	{
		this->curvatures.resize(0, 0);
	}
}

void ply_module::setColors(Eigen::MatrixXi colors)
{
	if (colors.size() > 0)
	{
		this->colors.resize(0, 0);
		this->colors = colors;
	}
	else
	{
		this->colors.resize(0, 0);
	}
}

void ply_module::setFaces(Eigen::MatrixXi faces)
{
	if (faces.size() > 0)
	{
		this->faces.resize(0, 0);
		this->faces = faces;
	}
	else
	{
		this->faces.resize(0, 0);
	}
}

// READ/WRITE FUNCTIONS 
bool ply_module::readPLY(const std::string & filename, bool readColorsFlag, bool readFacesFlag, bool readNormalsFlag, bool readFaceTextureFlag, bool readCurvatureFlag, std::vector<float> &verts)
{
	// Tinyply can and will throw exceptions at you!
	try
	{
		// Read the file and create a std::istringstream suitable
		// for the lib -- tinyply does not perform any file i/o.
		std::ifstream ss(filename, std::ios::binary);

		// Parse the ASCII header fields
		PlyFile file(ss);

		for (auto e : file.get_elements())
		{
			std::cout << "element - " << e.name << " (" << e.size << ")" << std::endl;
			for (auto p : e.properties)
			{
				std::cout << "\tproperty - " << p.name << " (" << PropertyTable[p.propertyType].str << ")" << std::endl;
			}
		}
		std::cout << std::endl;

		for (auto c : file.comments)
		{
			std::cout << "Comment: " << c << std::endl;
		}

		// Define containers to hold the extracted data. The type must match
		// the property type given in the header. Tinyply will interally allocate the
		// the appropriate amount of memory.
        // std::vector<float> verts;
		std::vector<float> norms;
		std::vector<uint8_t> colors;
		std::vector<uint32_t> faces;
		std::vector<float> uvCoords;
		std::vector<float> curvatures;

		uint32_t vertexCount, normalCount, colorCount, faceCount, faceTexcoordCount, faceColorCount, curvatureCount;
		vertexCount = normalCount = colorCount = faceCount = faceTexcoordCount = faceColorCount = 0;

		// The count returns the number of instances of the property group. The vectors
		// above will be resized into a multiple of the property group size as
		// they are "flattened"... i.e. verts = {x, y, z, x, y, z, ...}
		// For properties that are list types, it is possibly to specify the expected count (ideal if a
		// consumer of this library knows the layout of their format a-priori). Otherwise, tinyply
		// defers allocation of memory until the first instance of the property has been found
		// as implemented in file.read(ss)
		vertexCount = file.request_properties_from_element("vertex", { "x", "y", "z" }, verts);
		if(readNormalsFlag == true) normalCount = file.request_properties_from_element("vertex", { "nx", "ny", "nz" }, norms);
		if(readColorsFlag == true) colorCount = file.request_properties_from_element("vertex", { "red", "green", "blue" }, colors);
		if(readCurvatureFlag == true) curvatureCount = file.request_properties_from_element("vertex", {"quality"}, curvatures);
		if(readFacesFlag == true) faceCount = file.request_properties_from_element("face", { "vertex_indices" }, faces, 3);
		if(readFaceTextureFlag == true) faceTexcoordCount = file.request_properties_from_element("face", { "texcoord" }, uvCoords, 6);
	
	  
		// Now populate the vectors...
		timepoint before = now();
		file.read(ss);
		timepoint after = now();

		// Good place to put a breakpoint!
		std::cout << "Parsing took " << difference_micros(before, after) << " micro seconds " << std::endl;
		std::cout << "\tRead " << verts.size() << " total vertices (" << vertexCount << " properties)." << std::endl;
		std::cout << "\tRead " << norms.size() << " total normals (" << normalCount << " properties)." << std::endl;
		std::cout << "\tRead " << colors.size() << " total vertex colors (" << colorCount << " properties)." << std::endl;
		std::cout << "\tRead " << curvatures.size() << " total vertex curvatures (" << curvatureCount << " properties)." << std::endl;
		std::cout << "\tRead " << faces.size() << " total faces (triangles) (" << faceCount << " properties)." << std::endl;
		std::cout << "\tRead " << uvCoords.size() << " total texcoords (" << faceTexcoordCount << " properties)." << std::endl;

		// For  C++11 it is a good practice to follow this instead below reinterpret_cast<double*> (verts.data());  works little faster
		if (vertexCount != 0)		this->vertices = (Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic,Eigen::RowMajor>::Map(&verts[0], vertexCount, 3)).cast<double>();
		if (normalCount != 0)		this->normals = (Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>::Map(&norms[0], normalCount, 3)).cast<double>();
		if (faceTexcoordCount != 0)	this->textureCoords = (Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>::Map(&uvCoords[0], faceTexcoordCount, 3)).cast<double>();
		if (faceCount != 0)		this->faces = Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>::Map(&faces[0], faceCount, 3).cast<int>();
		if (colorCount != 0)		this->colors = Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>::Map(&colors[0], colorCount, 3).cast<int>();
		if (curvatureCount != 0)	this->curvatures = (Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>::Map(&curvatures[0], curvatureCount, 1)).cast<double>();
		
        // here to print the vertice in Terminal
        // std::cout << verts[1] << std::endl;

        return true;
	}
	catch (const std::exception & e)
	{
		std::cerr << "Caught exception: " << e.what() << std::endl;
        return false;
	}

    return false;
}

bool ply_module::writePLY(const std::string & oFileName, bool writeColorsFlag, bool writeFacesFlag, bool writeNormalsFlag, bool writeCurvatureFlag, bool isBinary)
{
	try
	{	
		#if 0 
		Eigen::MatrixXf vertices_f = this->vertices.cast<float>().transpose();
		std::vector<float> verts(vertices_f.data(), vertices_f.data() + vertices_f.cols() * vertices_f.rows());

		Eigen::MatrixXf norms_f = this->normals.cast<float>().transpose();
		std::vector<float> norms(norms_f.data(), norms_f.data() + norms_f.rows() * norms_f.cols());

		Eigen::MatrixXf faceTexcoords_f = this->textureCoords.cast<float>().transpose();
		std::vector<float> faceTexcoords(textureCoords.data(), textureCoords.data() + textureCoords.rows() * textureCoords.cols());

		Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> colors_uint = this->colors.cast<uint8_t>().transpose();
		std::vector<uint8_t> colors(colors_uint.data(), colors_uint.data() + colors_uint.rows() * colors_uint.cols());

		Eigen::Matrix<int32_t, Eigen::Dynamic, Eigen::Dynamic> faces_int = this->faces.cast<int32_t>().transpose();
		std::vector<int32_t> Indicies(faces_int.data(), faces_int.data() + faces_int.rows() * faces_int.cols());*/
		
		#else
		std::vector<float> verts(this->vertices.rows() * this->vertices.cols());
		std::vector<float> norms(this->normals.rows() * this->normals.cols());
		std::vector<float> curves(this->curvatures.rows() * this->curvatures.cols());
		std::vector<float> faceTexcoords(this->textureCoords.rows() * this->textureCoords.cols());
		std::vector<uint8_t> colors(this->colors.rows() * this->colors.cols());
		std::vector<int32_t> Indicies(this->faces.rows() * this->faces.cols());

		if(this->vertices.rows() > 0)
			Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> >
			(&verts[0], this->vertices.rows(), this->vertices.cols()) = this->vertices.cast<float>();

		if (this->normals.rows() > 0 && writeNormalsFlag == true)
			Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> >
			(&norms[0], this->normals.rows(), this->normals.cols()) =  this->normals.cast<float>();

		if(this->curvatures.rows() > 0)
			Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> >
			(&curves[0], this->curvatures.rows(), this->curvatures.cols()) = this->curvatures.cast<float>();
		
		if (this->textureCoords.rows() > 0 && writeFacesFlag == true)
			Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> >
			(&faceTexcoords[0], this->textureCoords.rows(), this->textureCoords.cols()) = this->textureCoords.cast<float>();
		
		if (this->colors.rows() > 0 && writeColorsFlag == true)
			Eigen::Map<Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> >
			(&colors[0], this->colors.rows(), this->colors.cols()) = this->colors.cast<uint8_t>();

		if (this->faces.rows() > 0 && writeFacesFlag == true)
			Eigen::Map<Eigen::Matrix<int32_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> >
			(&Indicies[0], this->faces.rows(), this->faces.cols()) = this->faces.cast<int32_t>();
		#endif


		// Tinyply does not perform any file i/o internally
		std::filebuf fb;
		fb.open(oFileName, std::ios::out | std::ios::binary);
		std::ostream outputStream(&fb);
		PlyFile myFile;
		
		if (this->vertices.rows() > 0) 
			myFile.add_properties_to_element("vertex", { "x", "y", "z" }, verts);
		
		if (this->normals.rows() > 0 && writeNormalsFlag == true)
			myFile.add_properties_to_element("vertex", { "nx", "ny", "nz" }, norms);
		
		if (this->curvatures.rows() > 0 && writeCurvatureFlag == true)
			myFile.add_properties_to_element("vertex", { "quality" }, curves);
		
		if (this->colors.rows() > 0 && writeColorsFlag == true)
			myFile.add_properties_to_element("vertex", { "red", "green", "blue" }, colors);
		
		if (this->faces.rows() > 0 && writeFacesFlag == true)
			myFile.add_properties_to_element("face", { "vertex_indices" }, Indicies, 3, PlyProperty::Type::UINT8);
		
		if (this->textureCoords.rows() > 0 && writeFacesFlag == true)
			myFile.add_properties_to_element("face", { "texcoord" }, faceTexcoords, 6, PlyProperty::Type::UINT8);

		time_t _tm = time(NULL);
		struct tm * curtime = localtime(&_tm);
		myFile.comments.push_back("P.Thesis Seamless Cloth Simulation with High Geometric Fidelity");
		myFile.comments.push_back(asctime(curtime));
		myFile.write(static_cast < std::ostringstream & > (outputStream), isBinary);

		fb.close();
		return true;
	}
	catch (const std::exception & e)
	{
		std::cerr << "Caught exception: " << e.what() << std::endl;
		return false;
	}
	catch(...)
	{
		std::cerr << " Some unknown exception has ocurred ";
		return false;
	}
	return false;
}


// DEFAULT DESTRUCTORS
ply_module::~ply_module()
{
	this->vertices.resize(0, 0);
	this->normals.resize(0, 0);
	this->textureCoords(0, 0);
	this->faces.resize(0, 0);
	this->colors.resize(0,0);
}
