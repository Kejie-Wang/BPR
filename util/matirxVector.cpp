#include "matrixVector.h"

using namespace std;

template<typename T>
MatrixVector<T>::MatrixVector(){}

template<typename T>
MatrixVector<T>::MatrixVector(vector<T> _vec) 
{ 
	vec = _vec; 
}

template<typename T>
MatrixVector<T>::MatrixVector(int _dim) 
{ 
	vec.resize(_dim); 
}

template<typename T>
int MatrixVector<T>::dim() const { return vec.size(); }

template<typename T>
MatrixVector<T>& MatrixVector<T>::operator+=(const MatrixVector<T>& rhs)
{
	(*this) = (*this) + rhs;
	return *this;
}

template<typename T>
MatrixVector<T>& MatrixVector<T>::operator-=(const MatrixVector<T>& rhs)
{
	(*this) = (*this) - rhs;
	return *this;
}

