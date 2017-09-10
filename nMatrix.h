#include <valarray>
#include <iostream>

using std::valarray;
using std::slice;

template<class T, int N> class nMatrix;

template<class T, int N>
class SubMatrix
{
public:
	SubMatrix(slice sl, valarray<T>* vl, size_t* dim):
		s(sl), v(vl), dim(dim){}
	
	SubMatrix<T, N-1> operator[] (size_t ind)
	{
		int shift = 1;
		for(int i = 0; i < N-1; ++i)
			shift *= dim[i];
		return SubMatrix<T, N-1>(slice(ind * shift + s.start(), shift, 1), v, dim);
	}

	const SubMatrix<T, N-1> operator[] (size_t ind) const
	{
		int shift = 1;
		for(int i = 0; i < N-1; ++i)
			shift *= dim[i];
		return SubMatrix<T, N-1>(slice(ind * shift + s.start(), shift, 1), v, dim);
	}
	
	operator std::valarray<T>()
	{
		return valarray<T>( (*v)[s] );
	}
	
	template<class Q, int P> friend class nMatrix;
private:
	SubMatrix();	
	SubMatrix& operator=(const SubMatrix&);

	valarray<T>* v;
	slice s;
	size_t* dim;
};

template<class T>
class SubMatrix<T, 1>
{
public:
	SubMatrix(slice sl, valarray<T>* val, size_t* dim):
		s(sl), v(val), dim(dim){}
	
	T& operator[] (size_t ind)
	{
		return (*v)[s.start() + ind * s.stride()];
	}

	const T& operator[] (size_t ind) const
	{
		return (*v)[s.start() + ind * s.stride()];
	}
	
	operator valarray<T>()
	{
		return valarray<T>(v[s]);
	}
private:
	SubMatrix();
	SubMatrix& operator=(const SubMatrix&);
	
	valarray<T>* v;
	slice s;
	size_t* dim;
};

template<class T, int N>
class nMatrix
{
public:
	nMatrix(size_t* dimension):
		sz(1)
	{
		for(int i = 0; i < N; ++i)
		{
			dim[N-1-i] = dimension[i];
			sz *= dimension[i];
		}
		v = new valarray<T>(sz);
	}
	
	template<class AnotherType>
	nMatrix(const SubMatrix<AnotherType,N>& rhs):
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
	
	nMatrix(const nMatrix& rhs)
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
	
	template<class AnotherType>
	nMatrix(const nMatrix<AnotherType,N>& rhs)
	{
		sz = rhs.sz;
		v = new valarray<T>(sz);
		for(int i = 0; i < N; ++i)
		{
			dim[i] = rhs.dim[i];			
			v->operator[](i) = rhs.v->operator[](i);
		}
	}
	
	~nMatrix()
	{
		delete v;
	}
	
	nMatrix& operator= (const nMatrix& rhs)
	{
		sz = rhs.sz;
		v->resize(sz);
		slice s(0, sz, 1);
		v->operator[](s) = *rhs.v;
		return *this;
	}

	template<class AnotherType>
	nMatrix& operator= (const nMatrix<AnotherType, N>& rhs)
	{
		sz = rhs.sz;
		v->resize(sz);
		slice s(0, sz, 1);
		v->operator[](s) = *rhs->v;
	}

	template<class AnotherType>
	nMatrix& operator= (const SubMatrix<AnotherType, N>& rhs)
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

	SubMatrix<T, N-1> operator[] (size_t ind)
	{
		int shift = 1;
		for(int i = 0; i < N-1; ++i)
			shift *= dim[i];
		return SubMatrix<T, N-1>(slice(ind*shift, shift, 1), v, dim);
	}

	const SubMatrix<T, N-1> operator[] (size_t ind) const
	{
		int shift = 1;
		for(int i = 0; i < N-1; ++i)
			shift *= dim[i];
		return SubMatrix<T, N-1>(slice(ind*shift, shift, 1), const_cast<valarray<T>*>(v), const_cast<size_t*>(dim));
	}

	void print() const
	{
		for(size_t i = 0; i < sz; ++i)
			std::cout << (*v)[i] << " " ;
		std::cout << '\n';
	}
	
	template<class Q, int P> friend class nMatrix;

	size_t size() const
	{
		return sz;
	}

	template<int Dimension> 
	size_t get() const
	{
		return dim[N - Dimension - 1];
	}
private:
	valarray<T>* v;
	size_t dim[N];
	size_t sz;
};

template<class T>
class nMatrix<T, 1>
{
public:
	nMatrix(size_t dimension):
		dim(dimension)
	{
		v = new valarray<T>(dim);
	}
	
	~nMatrix()
	{
		delete v;
	}
	
	T& operator[] (size_t ind)
	{
		return (*v)[ind];
	}

	const T& operator[] (size_t ind) const
	{
		return (*v)[ind];
	}

	void print() const
	{
		for(size_t i = 0; i < dim; ++i)
			std::cout << (*v)[i] << " " ;
		std::cout << '\n';
	}

	size_t size() const
	{
		return dim;
	}

	template<int Dimension> 
	size_t get() const
	{
		return dim;
	}
private:
	valarray<T>* v;
	size_t dim;
};