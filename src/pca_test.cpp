#include <iostream>
#include <fstream>
#include "pca.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <fstream>
#include <numeric>

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

using namespace std;

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> generarPuntos(int npuntos){
	vector<float> x(npuntos);
	vector<float> y(npuntos);
	vector<float> z(npuntos);

	float eps = 2.220446;

	for(int i = 0; i < npuntos; i++){
		x[i] = static_cast<float>(i - npuntos / 2);

		float random_angle_1 = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;
        float random_angle_2 = static_cast<float>(rand()) / RAND_MAX * 2 * M_PI;

		y[i] = 10 * sin(random_angle_1);
		z[i] = 10 * sin(random_angle_2);
	}

	Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> resutl(npuntos, 3);
	for(int i = 0; i < npuntos; i++){
		resutl(i, 0) = x[i];
		resutl(i, 1) = y[i];
		resutl(i, 2) = z[i];
	}

	return resutl;
}

int main(int argc, char* argv[])
{
	// ------------------- Export data to .txt file -------------------
	ofstream puntosXYZ_Originales("puntosXYZ_Originales.txt");
	ofstream puntosXYZ("puntosXYZ.txt");
	ofstream rectaPCA("rectaPCA.txt");
	ofstream proyeccion("proyeccion.txt");
	ofstream reProyeccion("reProyeccion.txt");
	ofstream planoOrtogonal("planoOrtogonal.txt");

	if(!puntosXYZ || !puntosXYZ_Originales || !rectaPCA || !proyeccion || !reProyeccion || !planoOrtogonal )
	{
		cout << "Error al abrir los archivos" << endl;
		return EXIT_FAILURE;
	}
	//------------------------------------------------------------------
	const int ndim = 3;
	const int npuntos = 6;
	//const int npuntos = 500;

	Eigen::Matrix<float, npuntos, ndim> pca_data_matrix;
	Eigen::Matrix<float, npuntos, ndim> pca_center_matrix;

	// Ejemplo 1...
	//pca_data_matrix = generarPuntos(npuntos); 

	// Ejemplo 2... 
	
	pca_data_matrix << 
		126.0, 78.0, -1.0,
		128.0, 80.0, -1.0,
		128.0, 82.0, 0.0,
		130.0, 82.0, 0.0,
		130.0, 84.0, 1.0,
		132.0, 86.0, 1.0;
	

	pca_t<float> pca;
	pca.set_input(pca_data_matrix);
	pca.compute();

	
	std::cout
		<< "Input Matrix:		\n" << pca.get_input_matrix() << std::endl << std::endl
		<< "Centered Matrix:	\n" << pca.get_centered_matrix() << std::endl << std::endl
		<< "Covariance Matrix:	\n" << pca.get_covariance_matrix() << std::endl << std::endl
		<< "Projection Matrix:	\n" << pca.get_projection_matrix() << std::endl << std::endl
		<< "Mean Matrix:		\n" << pca.get_mean() << std::endl << std::endl
		<< "Eigen Values:		\n" << pca.get_eigen_values() << std::endl << std::endl
		<< "Eigen Vectors:		\n" << pca.get_eigen_vectors() << std::endl << std::endl;
	

	const auto& reprojection = pca.reprojection();
	auto error = (pca_data_matrix - reprojection).norm();

	
	std::cout
		//<< "Reprojected Matrix:	\n" << reprojection << std::endl << std::endl
		<< std::fixed
		<< "Error:				\n" << error << std::endl << std::endl;
	

	// ------------------- Export data to .txt file -------------------:
	// 1. Exportar datos orginales a un archivo .txt
	Eigen::Matrix<float, npuntos, ndim> inputMatrix2 = pca.get_input_matrix();
	cout << "Exportando datos originales: " << endl;
	for(int i = 0; i < inputMatrix2.rows(); i++){
		for(int j = 0; j < inputMatrix2.cols(); j++){
			//cout << inputMatrix2(i,j) << " ";
			puntosXYZ_Originales << inputMatrix2(i,j) << " ";
		}
		//cout << endl;
		puntosXYZ_Originales << "\n";
	}


	// 2. Exportar los puntos centrados a un archivo .txt
	Eigen::Matrix<float, npuntos, ndim> inputMatrix = pca.get_centered_matrix();
	cout << "Exportando puntos centrados: " << endl;
	for(int i = 0; i < inputMatrix.rows(); i++){
		for(int j = 0; j < inputMatrix.cols(); j++){
			//cout << inputMatrix(i,j) << " ";
			puntosXYZ << inputMatrix(i,j) << " ";
		}
		//cout << endl;
		puntosXYZ << "\n";
	}

	// 3. Exportar la recta PCA a un archivo .txt
	Eigen::Matrix<float, ndim, ndim> eigenVector = pca.get_eigen_vectors();
	cout << "Exportando recta PCA: " << endl;
	for(int i = 0; i < eigenVector.rows(); i++){
		//cout << eigenVector(i,0) << endl;
		rectaPCA << eigenVector(i,0) << "\n";
	}

	// 4. Exportar la proyeccion de los puntos a un archivo .txt	
	cout << "Exportando proyeccion: " << endl;	
	// --> Producto escalar
	Eigen::Matrix<float, ndim, npuntos> newPointsProyected = pca.get_projection_matrix(); 
	// --> Proyeccion
	Eigen::Matrix<float, npuntos, ndim> newPointsProyected2 = pca.get_centered_matrix(); // solo de base
	for(int i = 0; i < npuntos; i++){
		for(int j = 0; j < ndim; j++){
			//cout << "Colocando en posicion (" << i << "," << j << ") -> " << newPointsProyected(0,i) << " * " << eigenVector(j,0) << endl;
			newPointsProyected2(i,j) = newPointsProyected(0,i) * eigenVector(j,0);
		}
	}

	for(int i = 0; i < newPointsProyected2.rows(); i++){	
		for(int j = 0; j < newPointsProyected2.cols(); j++){
			//cout << newPointsProyected2(i,j) << " ";
			proyeccion << newPointsProyected2(i,j) << " ";
		} 
		//cout << endl;
		proyeccion << "\n";
	}

	// 5. Exportar la proyeccion de los puntos ORIGINALES a un archivo .txt
	cout << "Exportando proyeccion de los puntos ORIGINALES: " << endl;
	for(int i = 0; i < reprojection.rows(); i++){
		for(int j = 0; j < reprojection.cols(); j++){
			//cout << reprojection(i,j) << " ";
			reProyeccion << reprojection(i,j) << " ";
		}
		//cout << endl;
		reProyeccion << "\n";
	}

	return EXIT_SUCCESS;
}