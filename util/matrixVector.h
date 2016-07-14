#ifndef _MATRIXVECTOR_H_
#define _MATRIXVECTOR_H_

#include <ostream>
#include <vector>
using namespace std;
/*
* @brief index out of bound exception, inherit the c++ stardard exception
*/
class IndexOutOfBound : public std::exception
{
public:
	const char* what()
	{
		std::string msg = "Index exceeds matrix bound!\n";
		return msg.c_str();
	}
};

class DimNotMatch : public std::exception
{
public:
	const char* what()
	{
		std::string msg = "The dimension of two matrixs do not match!\n";
		return msg.c_str();
	}
};


template<typename T>
class MatrixVector
{
public:
	MatrixVector(){}

	MatrixVector(int _dim){ vec.resize(_dim); }

	MatrixVector(std::vector<T> _vec){ vec = _vec; }

	T& operator[](int index)
	{
		int _dim = dim();
		if (index >= _dim)
			throw IndexOutOfBound();
		else
			return vec[index];
	}

	const T& operator[](int index) const
	{
		int _dim = dim();
		if (index >= _dim)
			throw IndexOutOfBound();
		else
			return vec[index];
	}

	int dim() const{ return vec.size(); }

	template<typename T>
	friend T dotProduct(const MatrixVector<T>& v1, const MatrixVector<T>& v2);
	template<typename T>
	friend MatrixVector<T> operator*(const MatrixVector<T> &lhs, const T &rhs);
	template<typename T>
	friend MatrixVector<T> operator*(const T &lhs, const MatrixVector<T> &rhs);
	template<typename T>
	friend MatrixVector<T> operator+(const MatrixVector<T> &lhs, const MatrixVector<T> &rhs);
	template<typename T>
	friend MatrixVector<T> operator-(const MatrixVector<T> &lhs, const MatrixVector<T> &rhs);
	template<typename T>
	friend std::ostream &operator<<(std::ostream& os, const MatrixVector<T>& rhs);

	MatrixVector<T>& operator+=(const MatrixVector<T>& rhs)
	{
		(*this) = (*this) + rhs;
		return *this;
	}

	MatrixVector<T>& operator-=(const MatrixVector<T>& rhs)
	{
		(*this) = (*this) - rhs;
		return *this;
	}


private:
	std::vector<T> vec;
};


template<typename T>
T dotProduct(const MatrixVector<T>& lhs, const MatrixVector<T>& rhs)
{
	if (lhs.dim() != rhs.dim())
		throw DimNotMatch();
	T res = 0;
	for (int i = 0; i < lhs.dim(); ++i)
		res += lhs.vec[i] * rhs.vec[i];

	return res;
}

template<typename T>
MatrixVector<T> operator*(const MatrixVector<T> &lhs, const T &rhs)
{
	vector<T> res = lhs.vec;
	for (int i = 0; i < lhs.dim(); ++i)
		res[i] = res[i] * rhs;

	return res;
}

template<typename T>
MatrixVector<T> operator*(const T &lhs, const MatrixVector<T> &rhs)
{
	return rhs * lhs;
}

template<typename T>
MatrixVector<T> operator+(const MatrixVector<T> &lhs, const MatrixVector<T> &rhs)
{
	//The dimensions of two vector are not same
	if (lhs.dim() != rhs.dim())
		throw DimNotMatch();

	vector<T> res(lhs.dim());
	for (int i = 0; i < lhs.dim(); ++i)
		res[i] = lhs.vec[i] + rhs.vec[i];

	return res;
}

template<typename T>
MatrixVector<T> operator-(const MatrixVector<T> &lhs, const MatrixVector<T> &rhs)
{
	//The dimensions of two vector are not same
	if (lhs.dim() != rhs.dim())
		throw DimNotMatch();

	vector<T> res(lhs.dim());
	for (int i = 0; i < lhs.dim(); ++i)
		res[i] = lhs.vec[i] - rhs.vec[i];

	return res;
}

template<typename T>
ostream &operator<<(ostream& os, const MatrixVector<T>& rhs)
{
	for (int i = 0; i < rhs.dim(); ++i)
		os << rhs.vec[i] << " ";
	return os;
}

#endif /*_MATRIXVECTOR_H_*/