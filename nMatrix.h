#include <valarray>

using std::valarray;
using std::slice;

template<class T, int N> class nMatrix;

//Submatrix
template<class T, int N>
class SubMatrix
{
public:
	SubMatrix(slice sl, valarray<T>* vl, size_t* dim):
		s(sl), v(vl), dim(dim){}
	
	SubMatrix<T, N-1>			operator[] (size_t ind);

	const SubMatrix<T, N-1>		operator[] (size_t ind) const;	
	
	template<class AnotherType>
	SubMatrix&					operator+= (const AnotherType& value);

	template<class AnotherType>
	SubMatrix&					operator-= (const AnotherType& value);

	operator std::valarray<T>()
	{
		return valarray<T>( (*v)[s] );
	}
	
	template<class Q, int P> friend class nMatrix;
private:
	SubMatrix();	
	SubMatrix&					operator=(const SubMatrix&);

	valarray<T>*				v;
	slice						s;
	size_t*						dim;
};

template<class T, int N>
SubMatrix<T, N-1> SubMatrix<T, N>::operator[] (size_t ind)
{
	int shift = 1;
	for(int i = 0; i < N-1; ++i)
		shift *= dim[i];
	return SubMatrix<T, N-1>(slice(ind * shift + s.start(), shift, 1), v, dim);
}

template<class T, int N>
const SubMatrix<T, N-1>	SubMatrix<T, N>::operator[] (size_t ind) const
{
	int shift = 1;
	for(int i = 0; i < N-1; ++i)
		shift *= dim[i];
	return SubMatrix<T, N-1>(slice(ind * shift + s.start(), shift, 1), v, dim);
}

template<class T, int N>
template<class AnotherType>
SubMatrix<T, N>& SubMatrix<T, N>::operator+= (const AnotherType& value)
{	
	for(size_t i = s.start(); i < s.size(); i=i+s.stride() )
		v->operator[](i) += value;
	return *this;
}

template<class T, int N>
template<class AnotherType>
SubMatrix<T, N>& SubMatrix<T, N>::operator-= (const AnotherType& value)
{
	for(size_t i = s.start(); i < s.size(); i=i+s.stride() )
		v->operator[](i) -= value;
	return *this;
}

//1d Submatrix specialization
template<class T>
class SubMatrix<T, 1>
{
public:
	SubMatrix(slice sl, valarray<T>* val, size_t* dim):
		s(sl), v(val), dim(dim){}
	
	T&							operator[] (size_t ind)
	{
		return (*v)[s.start() + ind * s.stride()];
	}

	const T&					operator[] (size_t ind) const
	{
		return (*v)[s.start() + ind * s.stride()];
	}
	
	template<class AnotherType>
	SubMatrix&					operator+= (const AnotherType& value);

	template<class AnotherType>
	SubMatrix&					operator-= (const AnotherType& value);

	operator valarray<T>()
	{
		return valarray<T>(v[s]);
	}
private:
	SubMatrix();
	SubMatrix&					operator=(const SubMatrix&);
	
	valarray<T>*				v;
	slice						s;
	size_t*						dim;
};

template<class T>
template<class AnotherType>
SubMatrix<T, 1>& SubMatrix<T, 1>::operator+= (const AnotherType& value)
{	
	for(size_t i = s.start(); i < s.size(); i=i+s.stride() )
		v->operator[](i) += value;
	return *this;
}

template<class T>
template<class AnotherType>
SubMatrix<T, 1>& SubMatrix<T, 1>::operator-= (const AnotherType& value)
{
	for(size_t i = s.start(); i < s.size(); i=i+s.stride() )
		v->operator[](i) -= value;
	return *this;
}

//nMatrix
template<class T, int N>
class nMatrix
{
public:
	nMatrix(size_t* dimension);

	template<class AnotherType>
	nMatrix(const SubMatrix<AnotherType,N>& rhs);

	nMatrix(const nMatrix& rhs);

	template<class AnotherType>
	nMatrix(const nMatrix<AnotherType,N>& rhs);
	
	~nMatrix()
	{
		delete v;
	}
	
	nMatrix&					operator= (const nMatrix& rhs);

	template<class AnotherType>
	nMatrix&					operator= (const nMatrix<AnotherType, N>& rhs);

	template<class AnotherType>
	nMatrix&					operator= (const SubMatrix<AnotherType, N>& rhs);

	template<class AnotherType>
	nMatrix&					operator= (const AnotherType& value);

	template<class AnotherType>
	nMatrix&					operator+= (const AnotherType& value);

	template<class AnotherType>
	nMatrix&					operator-= (const AnotherType& value);

	SubMatrix<T, N-1>			operator[] (size_t ind);

	const SubMatrix<T, N-1>		operator[] (size_t ind) const;
	
	template<class Q, int P> friend class nMatrix;

	size_t						size() const
	{
		return sz;
	}

	template<int Dimension> 
	size_t						get() const
	{
		return dim[N - Dimension - 1];
	}
private:
	valarray<T>*				v;
	size_t						dim[N];
	size_t						sz;
};

template<class T, int N>
nMatrix<T,N>::nMatrix(size_t* dimension):
		sz(1)
{
	for(int i = 0; i < N; ++i)
	{
		dim[N-1-i] = dimension[i];
		sz *= dimension[i];
	}
	v = new valarray<T>(sz);
}

template<class T, int N>
template<class AnotherType>
nMatrix<T, N>::nMatrix(const SubMatrix<AnotherType,N>& rhs):
	sz(1)
{
	for(int i = 0; i < N; ++i)
	{
		dim[i] = rhs.dim[i];
		sz *= dim[i];
	}
	v = new valarray<T>(sz);
	for(size_t i = 0; i < sz; ++i)
	{
		int start = rhs.s.start();
		v->operator[](i) = rhs.v->operator[](start + i);
	}
}

template<class T, int N>
nMatrix<T, N>::nMatrix(const nMatrix<T, N>& rhs)
{
	sz = rhs.sz;
	v = new valarray<T>(sz);
	for(int i = 0; i < N; ++i)
	{
		dim[i] = rhs.dim[i];
	}		
	slice sl(0, sz, 1);
	v->operator[](sl) = *rhs.v;
}

template<class T, int N>
template<class AnotherType>
nMatrix<T, N>::nMatrix(const nMatrix<AnotherType,N>& rhs)
{
	sz = rhs.sz;
	v = new valarray<T>(sz);
	for(int i = 0; i < N; ++i)
	{
		dim[i] = rhs.dim[i];			
		v->operator[](i) = rhs.v->operator[](i);
	}
}

template<class T, int N>
nMatrix<T, N>&	nMatrix<T, N>::operator= (const nMatrix<T, N>& rhs)
{
	sz = rhs.sz;
	v->resize(sz);
	slice s(0, sz, 1);
	v->operator[](s) = *rhs.v;
	return *this;
}

template<class T, int N>
template<class AnotherType>
nMatrix<T, N>&	nMatrix<T, N>::operator= (const nMatrix<AnotherType, N>& rhs)
{
	sz = rhs.sz;
	v->resize(sz);
	slice s(0, sz, 1);
	v->operator[](s) = *rhs->v;
}

template<class T, int N>
template<class AnotherType>
nMatrix<T, N>&	nMatrix<T, N>::operator= (const SubMatrix<AnotherType, N>& rhs)
{
	sz = 1;
	for(int i = 0; i < N; ++i)
	{
		dim[i] = rhs.dim[i];
		sz *= dim[i];
	}
	v->resize(sz);
	v->operator[](rhs.s) = *rhs.v;
	return *this;
}

template<class T, int N>
template<class AnotherType>
nMatrix<T, N>&	nMatrix<T, N>::operator= (const AnotherType& value)
{
	v->operator=(value);
	return *this;
}

template<class T, int N>
template<class AnotherType>
nMatrix<T, N>&	nMatrix<T, N>::operator+= (const AnotherType& value)
{		
	v->operator+=(value);
	return *this;
}

template<class T, int N>
template<class AnotherType>
nMatrix<T, N>&	nMatrix<T, N>::operator-= (const AnotherType& value)
{
	v->operator-=(value);
	return *this;
}

template<class T, int N>
SubMatrix<T, N-1> nMatrix<T, N>::operator[] (size_t ind)
{
	int shift = 1;
	for(int i = 0; i < N-1; ++i)
		shift *= dim[i];
	return SubMatrix<T, N-1>(slice(ind*shift, shift, 1), v, dim);
}

template<class T, int N>
const SubMatrix<T, N-1>	nMatrix<T, N>::operator[] (size_t ind) const
{
	int shift = 1;
	for(int i = 0; i < N-1; ++i)
		shift *= dim[i];
	return SubMatrix<T, N-1>(slice(ind*shift, shift, 1), const_cast<valarray<T>*>(v), const_cast<size_t*>(dim));
}

//1d specialization
template<class T>
class nMatrix<T, 1>
{
public:
	nMatrix(size_t dimension):
		dim(dimension)
	{
		v = new valarray<T>(dim);
	}

	template<class AnotherType>
	nMatrix(const SubMatrix<AnotherType, 1>& rhs);

	nMatrix(const nMatrix& rhs);

	template<class AnotherType>
	nMatrix(const nMatrix<AnotherType, 1>& rhs);
	
	~nMatrix()
	{
		delete v;
	}
	
	T&							operator[] (size_t ind)
	{
		return (*v)[ind];
	}

	const T&					operator[] (size_t ind) const
	{
		return (*v)[ind];
	}

	nMatrix&					operator= (const nMatrix& rhs);

	template<class AnotherType>
	nMatrix&					operator= (const nMatrix<AnotherType, 1>& rhs);

	template<class AnotherType>
	nMatrix&					operator= (const AnotherType& value);

	template<class AnotherType>
	nMatrix&					operator= (const SubMatrix<AnotherType, 1>& rhs);

	template<class AnotherType>
	nMatrix&					operator+= (const AnotherType& value);

	template<class AnotherType>
	nMatrix&					operator-= (const AnotherType& value);

	size_t						size() const
	{
		return dim;
	}

	template<int Dimension> 
	size_t						get() const
	{
		return dim;
	}
private:
	valarray<T>*				v;
	size_t						dim;
};

template<class T>
template<class AnotherType>
nMatrix<T, 1>::nMatrix(const SubMatrix<AnotherType, 1>& rhs):
	sz(1)
{
	dim = rhs.dim[0];
	v = new valarray<T>(dim);
	for(size_t i = 0; i < dim; ++i)
	{
		int start = rhs.s.start();
		v->operator[](i) = rhs.v->operator[](start + i);
	}
}

template<class T>
nMatrix<T, 1>::nMatrix(const nMatrix<T, 1>& rhs)
{
	dim = rhs.dim;
	v = new valarray<T>(dim);		
	slice sl(0, dim, 1);
	v->operator[](sl) = *rhs.v;
}

template<class T>
template<class AnotherType>
nMatrix<T, 1>::nMatrix(const nMatrix<AnotherType, 1>& rhs)
{
	dim = rhs.dim;
	for(int i = 0; i < dim; ++i)
	{					
		v->operator[](i) = rhs.v->operator[](i);
	}
}

template<class T>
nMatrix<T, 1>&	nMatrix<T, 1>::operator= (const nMatrix<T, 1>& rhs)
{
	dim = rhs.dim;
	v->resize(dim);
	slice s(0, dim, 1);
	v->operator[](s) = *rhs.v;
	return *this;
}

template<class T>
template<class AnotherType>
nMatrix<T, 1>&	nMatrix<T, 1>::operator= (const nMatrix<AnotherType, 1>& rhs)
{
	dim = rhs.dim;
	v->resize(dim);
	slice s(0, dim, 1);
	v->operator[](s) = *rhs->v;
}

template<class T>
template<class AnotherType>
nMatrix<T, 1>&	nMatrix<T, 1>::operator= (const AnotherType& value)
{
	v->operator=(value);
	return *this;
}

template<class T>
template<class AnotherType>
nMatrix<T, 1>&	nMatrix<T, 1>::operator= (const SubMatrix<AnotherType, 1>& rhs)
{
	dim = rhs.dim;
	v->resize(dim);
	v->operator[](rhs.s) = *rhs.v;
	return *this;
}

template<class T>
template<class AnotherType>
nMatrix<T, 1>&	nMatrix<T, 1>::operator+= (const AnotherType& value)
{
	v->operator+=(value);
	return *this;
}

template<class T>
template<class AnotherType>
nMatrix<T, 1>&	nMatrix<T, 1>::operator-= (const AnotherType& value)
{
	v->operator-=(value);
	return *this;
}


//Global functions
template<class T, class AnotherType, int N>
nMatrix<T,N> operator+(const nMatrix<T,N>& lhs, const AnotherType& rhs)
{
	nMatrix<T,N> result(lhs);
	result += rhs;
	return result;
}

template<class T, class AnotherType, int N>
nMatrix<T,N> operator-(const nMatrix<T,N>& lhs, const AnotherType& rhs)
{
	nMatrix<T,N> result(lhs);
	result -= rhs;
	return result;
}

template<class T, class AnotherType, int N>
nMatrix<T,N> operator+(const AnotherType& lhs, const nMatrix<T,N>& rhs)
{
	nMatrix<T,N> result(rhs);
	result += lhs;
	return result;
}

template<class T, class AnotherType, int N>
nMatrix<T,N> operator-(const AnotherType& lhs, const nMatrix<T,N>& rhs)
{
	nMatrix<T,N> result(rhs);
	result -= lhs;
	return result;
}