#include <cfloat>
#include <cmath>
#include <cstdlib>
#include "matrix.h"

using namespace std;

Vector::Vector()
{
	data = new double[3];
	data[0] = 0;
	data[1] = 0;
	data[2] = 1;
}

Vector::Vector(const Vector& oldVector) 
{
	data = new double[3];
	data[0] = oldVector[0];
	data[1] = oldVector[1];
	data[2] = oldVector[2];
}

Vector::Vector(const double x, const double y)
{
	data = new double[3];
	data[0] = x;
	data[1] = y;
	data[2] = 1;
}


Vector::~Vector()
{
	delete[] data;
}

double& Vector::operator[](int index) const
{
	return data[index];
}

Matrix::Matrix() 
{
	data = new double *[3];

	for (int i = 0; i < 3; i++) {

		data[i] = new double [3];

		for (int j = 0; j < 3; j++) {

			if (i == j){data[i][j] = 1;}
			else {data[i][j] = 0;}
		}
	}
}

Matrix::Matrix(const Matrix& oldMatrix) 
{
	data = new double *[3];

	for (int i = 0; i < 3; i++) {

		data[i] = new double[3];
	}

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			data [i][j] = oldMatrix[i][j];
		}
	}
}


Matrix::~Matrix()
{
	delete[] data;
}

Matrix* Matrix::multiply(const Matrix* otherMatrix) const
{
	Matrix* answer = new Matrix[2];

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			(*answer)[i][j] = 0;

			for (int k = 0; k < 3; k++){

			(*answer)[i][j] += data[i][k] * (*otherMatrix)[k][j]; 
			}
		}
	}
	return answer;
}

Vector* Matrix::multiply(const Vector* theVector) const
{
	Vector* answer = new Vector[3];

	for (int i =0; i < 3; i++) {

		for (int j=0; j < 3; j++) {

			(*answer)[i] += data[i][j] * (*theVector)[j];
			}
		}
	return answer;
}

double* Matrix::operator[](int index) const
{
	return data[index];
}

Matrix* Matrix::translation(double deltaX, double deltaY)
{
	Matrix* translate = new Matrix();

	(*translate)[0][0] = 1;
	(*translate)[0][1] = 0;
	(*translate)[0][2] = deltaX;
	(*translate)[1][0] = 0;
	(*translate)[1][1] = 1;
	(*translate)[1][2] = deltaY;
	(*translate)[2][0] = 0;
	(*translate)[2][1] = 0;
	(*translate)[2][2] = 1;

	return translate;
}

Matrix* Matrix::rotation(double theta)
{
	Matrix* rotation = new Matrix();

	(*rotation)[0][0] = cos(theta);
	(*rotation)[0][1] = -sin(theta);
	(*rotation)[0][2] = 0;
	(*rotation)[1][0] = sin(theta);
	(*rotation)[1][1] = cos(theta);
	(*rotation)[1][2] = 0;
	(*rotation)[2][0] = 0;
	(*rotation)[2][1] = 0;
	(*rotation)[2][2] = 1;

	return rotation;

}
Matrix* Matrix::shearing(double shearXY, double shearYX)
{
	Matrix* shearing = new Matrix();
	(*shearing)[0][0] = 1;
	(*shearing)[0][1] = shearXY;
	(*shearing)[0][2] = 0;
	(*shearing)[1][0] = shearYX;
	(*shearing)[1][1] = 1;
	(*shearing)[1][2] = 0;
	(*shearing)[2][0] = 0;
	(*shearing)[2][1] = 0;
	(*shearing)[2][2] = 1;

	return shearing;
}

Matrix* Matrix::scaling(double scaleX, double scaleY)
{
	Matrix* scale = new Matrix();
	(*scale)[0][0] = scaleX;
	(*scale)[0][1] = 0;
	(*scale)[0][2] = 0;
	(*scale)[1][0] = 0;
	(*scale)[1][1] = scaleY;
	(*scale)[1][2] = 0;
	(*scale)[2][0] = 0;
	(*scale)[2][1] = 0;
	(*scale)[2][2] = 1;

	return scale;
}

Matrix* Matrix::getInverse() const
{
	//Matrix made using "new Matrix[3]" rather than
	//the suggested implementation due to simplicity
	Matrix* answer = new Matrix[3];

	double det = (-data[0][2] * data[1][1] * data[2][0]) +
				 (data[0][1] * data[1][2] * data[2][0]) +
				 (data[0][2] * data[1][0] * data[2][1]) -
				 (data[0][0] * data[1][2] * data[2][1]) -
				 (data[0][1] * data[1][0] * data[2][2]) +
				 (data[0][0] * data[1][1] * data[2][2]);

	(*answer)[0][0] = ((-data[1][2] * data[2][1]) +
					(data[1][1] * data[2][2])) / det;
	(*answer)[0][1] = ((data[1][2] * data[2][0]) -
					(data[1][0] * data[2][2])) / det;
	(*answer)[0][2] = ((-data[1][1] * data[2][0]) +
					(data[1][0] * data[2][1])) / det;
	(*answer)[1][0] = ((data[0][2] * data[2][1]) -
					(data[0][1] * data[2][2])) / det;
	(*answer)[1][1] = ((-data[0][2] * data[2][0]) +
					(data[0][0] * data[2][2])) / det;
	(*answer)[1][2] = ((data[0][1] * data[2][0]) -
					(data[0][0] * data[2][1])) / det;
	(*answer)[2][0] = ((-data[0][2] * data[1][1]) +
					(data[0][1] * data[1][2])) / det;
	(*answer)[2][1] = ((data[0][2] * data[1][0]) -
					(data[0][0] * data[1][2])) / det;
	(*answer)[2][2] = ((-data[0][1] * data[1][0]) +
					(data[0][0] * data[1][1])) / det;
	
	return answer;
}
