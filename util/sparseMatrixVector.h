#ifndef _SPARSEMATRIXVECTOR_H_
#define _SPARSEMATRIXVECTOR_H_

template<typename T>
class SparseMatrixVector : public std::map<int, T>
{
public:
	SparseMatrixVector() {}


	SparseMatrixVector(int _dim) { return dim; }
	T& operator[](int index)
	{
		if (index >= dim)
			throw IndexOutOfBound();
		//else if (this->find(index) == this->end())
		//return T(0);
		else
			return (*this)[index];
	}

	const T& operator[](int index) const
	{
		if (index >= dim)
			throw IndexOutOfBound();
		//else if (this->find(index) == this->end())
		//return T(0);
		else
			return (*this)[index];
	}

	std::vector<int> keys()
	{
		std::vector<int> _keys;
		for (auto p : *this)
		{
			_keys.push_back(p.first);
		}
		return _keys;
	}


private:
	int dim;
};

#endif /*SPARSEMATRIXVECTOR_H_*/