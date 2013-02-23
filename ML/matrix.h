 
#ifndef _MATRIX_H
#define	_MATRIX_H
#include <vector>
#include "random.h"

using namespace std;

template <class T>
struct Matrix : public vector<vector<T> > 
{
	Matrix(unsigned row, unsigned col) : 
		vector<vector<T> >(row, vector<T>(col, 0.0)), row(row), col(col) {}

	Matrix(unsigned n) : 
		vector<vector<T> >(n, vector<T>(n, 0.0)), row(n), col(n) {}

    void random_init();
	void random_init_0();
	void interactive_init();
	
	// throws exception if not square 
	Matrix invert() const;

	Matrix xtx() const;

	// The same as above, but we transpose other
	Matrix multiply_by_transposed(const Matrix & other) const;

	Matrix transform(vector<T> (*t)(const vector<T> &)) const;

	static Matrix<T> diag(unsigned n, T lambda); 

	bool is_square() const { return row == col; }

	unsigned row;
	unsigned col;
};

template <class T>
ostream & operator<<(ostream & os, const Matrix<T> & m);

template <class T>
ostream & operator<<(ostream & os, const vector<T> & v)
{
	for (unsigned i = 0; i < v.size(); ++i)
		cerr << v[i] << "\t";

	return os;
}

template <class T>
Matrix<T> Matrix<T>::diag(unsigned n, T lambda)
{
	Matrix<T> r(n);
	for (unsigned i = 0; i < n; ++i)
		r[i][i] = lambda;

	return r;
}

template <class T>
void Matrix<T>::random_init() 
{
	unsigned TRAINING_SET = row;
	unsigned DIM = col;

	for (unsigned m = 0; m < TRAINING_SET; ++m)
		(*this)[m] = random_example<T>(DIM);

#if 0
	cerr << "Training set is initialized as:" << endl << *this << endl;
#endif
}

template <class T>
void Matrix<T>::random_init_0() 
{
	unsigned TRAINING_SET = row;
	unsigned DIM = col;

	for (unsigned m = 0; m < TRAINING_SET; ++m)
		(*this)[m] = random_example_0<T>(DIM);

#if 0
	cerr << "Training set is initialized as:" << endl << *this << endl;
#endif
}

template <class T>
Matrix<T> Matrix<T>::transform(vector<T> (*t)(const vector<T> &)) const
{
	Matrix transformed(this->row, 0);
	for (unsigned r = 0; r < this->row; ++r)
		transformed[r] = t((*this)[r]);

	transformed.col = transformed[0].size();

	return transformed;
}

template <class T>
Matrix<T> Matrix<T>::invert() const
{
	if (!is_square())
		throw exception("cannot inverse non-square matrix\n");

	Matrix<T> matrix(*this);
	unsigned n = matrix.size();
	Matrix<T> inverse(n);

	// Init inverse as ident matrix
	for (unsigned i = 0; i < n; ++i)
		for (unsigned j = 0; j < n; ++j)
			if (i == j) inverse[i][j] = 1.0;

	for (unsigned i = 0; i < n; ++i)
	{
		for (unsigned j = 0; j < n; ++j)
		{
			if (i != j)
			{
				if (matrix[i][i] == 0.0)
					throw exception("matrix cannot be inverted\n");
				T ratio = matrix[j][i]/matrix[i][i];
				for (unsigned k = 0; k < n; ++k)
				{
					matrix[j][k] -= ratio * matrix[i][k];
					inverse[j][k] -= ratio * inverse[i][k];
				}
			}
		}
	}

	for (unsigned i = 0; i < n; ++i)
	{
		T a = matrix[i][i];
		if (a == 0.0)
			throw exception("matrix cannot be inverted\n");
		for (unsigned j = 0; j < n; ++j)
		{
			matrix[i][j] /= a;
			inverse[i][j] /= a;
		}
	}

#if 0
	cerr << "The inverse matrix is: " << endl << inverse << endl;
#endif
	return inverse;
}

//a[i][j] = sigma[k:0->x.row](x[k][i] * x[k][j])
template <class T>
Matrix<T> Matrix<T>::xtx() const
{
	unsigned DIM = col;
	unsigned TRAINING_EXAMPLES = row;

	Matrix<T> res(DIM);
	for (unsigned n = 0; n < DIM; ++n)
	{
		for (unsigned q = 0; q < DIM; ++q)
		{
			T elem = 0.0;
			for (unsigned m = 0; m < TRAINING_EXAMPLES; ++m)
			{
				elem += (*this)[m][n] * (*this)[m][q];
			}
			res[n][q] = elem;
		}
	}

#if 0
	cerr << "XTX is:" << endl << res << endl;
#endif

	return res;
}

template <class T>
Matrix<T> & operator+=(Matrix<T> & m1, const Matrix<T> & m2)
{
	if (m1.row != m2.row || m1.col != m2.col)
		throw exception("not compatible for operator +=");

	for (unsigned r = 0; r < m1.row; ++r)
		for (unsigned c = 0; c < m1.col; ++c)
			m1[r][c] += m2[r][c];

	return m1;
}

template <class T>
Matrix<T> operator *(const Matrix<T> & m1, const Matrix<T> & m2)
{
	Matrix<T> res(m1.row, m2.col);
	for (unsigned m = 0; m < m1.row; ++m)
	{
		for (unsigned n = 0; n < m2.col; ++n) 
		{
			T elem = 0.0;
			for (unsigned k = 0; k < m2.row; ++k)
			{
				elem += m1[m][k] * m2[k][n]; 
			}
			res[m][n] = elem;
		}
	}

	return res;
}

// X(m,n), Y(m,n) - treated as transposed
template <class T>
Matrix<T> Matrix<T>::multiply_by_transposed(const Matrix<T> & other) const
{
	Matrix<T> res(this->row, other.row);
	for (unsigned m = 0; m < this->row; ++m)
	{
		for (unsigned n = 0; n < other.row; ++n) // we transpose other
		{
			T elem = 0.0;
			for (unsigned k = 0; k < other.col; ++k)
			{
				elem += (*this)[m][k] * other[n][k]; 
			}
			res[m][n] = elem;
		}
	}

#if 0
	cerr << "multiply_by_transposed is:" << endl << res << endl;
#endif

	return res;
}

template <class T> 
ostream & operator<<(ostream & os, const Matrix<T> & m)
{
	for (unsigned i = 0; i < m.row; ++i)
		os << m[i] << endl;
     
	return os;
}

#endif