#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector>
#include <string>
#include <map>
#include <exception>
#include <ostream>
#include "matrixVector.h"
#include "sparseMatrixVector.h"

template<typename T>
class Matrix : public MatrixVector< MatrixVector<T> >
{
public:
	Matrix() {}


	Matrix(int _row, int _col)
	{
		this->resize(_row);
		for (int i = 0; i < _row; ++i)
		{
			(*this)[i].resize(_col);
		}
	}


	int row() { return this->dim(); }

	
};


template<typename T>
class SparseMatrix : public MatrixVector<SparseMatrixVector<T> >
{
public:
	SparseMatrix(){}


	SparseMatrix(int _row, int _col) { this->_row = _row; this->_col = _col; }


	int row() { return _row; }

	int col() { return _col; }
private:
	int _row;
	int _col;
};



#endif /* _MATRIX_H_ */