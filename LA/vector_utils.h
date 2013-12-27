#ifndef _VECTOR_UTILS_H_
#define _VECTOR_UTILS_H_

#include <vector>
#include <cstdlib>

//#include "LA/matrix.h"

using namespace std;

template <class T> 
T inner_product(const vector<T> & v1, const vector<T> & v2)
{
	if (v1.size() != v2.size())
		throw exception("inner_product: v1.size() != v2.size()");

	T retval = 0.0;

	for (unsigned i = 0; i < v1.size(); ++i)
		retval += (v1[i] * v2[i]);

	return retval;
}

template <class T> 
Matrix<T> outer_product(const vector<T> & v1, const vector<T> & v2)
{
	return Matrix<T>(v1, v2);
}

template <class T> 
T square_dist(const vector<T> & v1, const vector<T> & v2)
{
	if (v1.size() != v2.size())
		throw exception("dist: v1.size() != v2.size()");

	T result = 0;
	for (auto it1 = v1.begin(), it1End = v1.end(), 
		it2 = v2.begin(); it1 != it1End; ++it1, ++it2)
		result += static_cast<T>(pow(*it1 - *it2, 2));

	return result;
}
	
template <class T> 
T euclidian_dist(const vector<T> & v1, const vector<T> & v2)
{
	return sqrt(square_dist(v1, v2));
}

template <class T>
T norm(const vector<T> & v, T p = 2)
{
	if (p < 1)
		throw exception("norm of vector is not defined for p < 1");

	T sum = 0;
	for (const auto & val : v) 
		sum += pow(val, p);

	return pow(sum, 1/p);
}

template <class T>
void make_vector_set(vector<T> & v)
{
	sort(v.begin(), v.end());
	auto itEnd = unique(v.begin(), v.end());
	v.erase(itEnd, v.end());
}

template <class T>T operator * (const vector<T> & v1, const vector<T> & v2)
{	return inner_product(v1, v2);
}

template <class T>
vector<T> & operator *= (vector<T> & v, const T & scalar)
{
	for (auto & el : v)
		el *= scalar;

	return v;
}

template <class T>
vector<T> & operator /= (vector<T> & v, const T & scalar)
{
	for (auto & el : v)
		if (el != 0) el /= scalar; // special attention to el == 0 to work around -0 result

	return v;
}

template <class T>
vector<T> & operator += (vector<T> & v1, const vector<T> & v2)
{
	auto it2 = v2.begin();
	for (auto it1 = v1.begin(), it1End = v1.end(); it1 != it1End; ++it1, ++it2)
		*it1 += *it2;

	return v1;
}

template <class T>
vector<T> & operator -= (vector<T> & v1, const vector<T> & v2)
{
	auto it2 = v2.begin();
	for (auto it1 = v1.begin(), it1End = v1.end(); it1 != it1End; ++it1, ++it2)
		*it1 -= *it2;

	return v1;
}
template <class T>
ostream & operator<<(ostream & os, const vector<T> & v)
{
	std::copy(v.begin(), v.end(), ostream_iterator<T>(os, " "));
	return os;
}


#endif
