//
//  MathDef.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef BezierDisplay_MathDef_h
#define BezierDisplay_MathDef_h

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <iterator>
#include <assert.h>

typedef double scalar;

typedef Eigen::Matrix<scalar, 1, 2> Vector12s;
typedef Eigen::Matrix<scalar, Eigen::Dynamic, 1> VectorX1s;
typedef Eigen::Matrix<scalar, Eigen::Dynamic, 2> VectorX2s;

typedef Eigen::Matrix<int, Eigen::Dynamic, 1> VectorX1i;

typedef Eigen::Matrix<scalar, 2, 2> Matrix2s;
typedef Eigen::Matrix<scalar, Eigen::Dynamic, Eigen::Dynamic> MatrixXs;

template <bool flag, typename IsTrue, typename IsFalse>
struct choose;

template <typename IsTrue, typename IsFalse>
struct choose<true, IsTrue, IsFalse> {
   typedef IsTrue type;
};

template <typename IsTrue, typename IsFalse>
struct choose<false, IsTrue, IsFalse> {
   typedef IsFalse type;
};

// A vector matrix iterator that can step through each row of the vector,
// and return the reference to entire row or more (see operator*()
// and getBlock() methods)
//
// Learn: Iterator should ususally accept the pointers to data!
template <typename T, bool isConst = false>
class VectorXXsIterator
			: public std::iterator<std::bidirectional_iterator_tag,
									T, ptrdiff_t, T*, T&>
{
public:

	typedef T data;
	typedef typename choose<isConst, const T&, T&>::type reference;
	typedef typename choose<isConst, const T*, T*>::type pointer;
	
	VectorXXsIterator( pointer v = 0, int row = 0)
	: m_row( row )
	, m_p( v )
	{}

	// Learn: copy constructor can access private
	// variables of another object of the same class. The variable
	// scope works on class level, not on object level.
	VectorXXsIterator(const VectorXXsIterator<T, false> & vit)
	: m_p( vit.m_p )
	, m_row( vit.m_row )
	{}

	//void VectorXXsIterator::setVectorX2s( VectorX2s & v )
	//{
	//	m_row = 0;
	//	m_p = &v;
	//}
	//
	//void VectorXXsIterator::setVectorX2s( VectorX2s & v, const int & row )
	//{
	//	m_row = row;
	//	m_p = &v;
	//}

	// Return an entire row of the current row
	// Learn: When return Eigen::Block<T> it provides reference by default. No & needed.
	Eigen::Block<T> operator*()
	{
		return m_p->block(m_row, 0, 1, m_p->cols());
	}
	
	// Return a block specified by the number of rows and columns
	// starting at position (i,j) = (current row, 0)
	// Learn: When return Eigen::Block<T> it provides reference by default. No & needed.
	Eigen::Block<T> getBlock(const int & rows, const int & cols)
	{
		assert (cols <= m_p->cols());
		assert (rows + m_row <= m_p->rows());
		
		return m_p->block(m_row, 0, rows, cols);
	}

	VectorXXsIterator& operator++()
	{
		++m_row;
		return *this;
	}

	VectorXXsIterator operator++(int)
	{
		VectorXXsIterator tmp(*this);
		operator++();
		return tmp;
	}

	VectorXXsIterator& operator+=(const int & i)
	{
		m_row += i;
		return *this;
	}

	VectorXXsIterator& operator--()
	{
		--m_row;
		return *this;
	}

	VectorXXsIterator operator--(int)
	{
		VectorXXsIterator tmp(*this);
		operator--();
		return tmp;
	}

	VectorXXsIterator& operator-=(const int & i)
	{
		m_row -= i;
		return *this;
	}
	
	bool equal(const VectorXXsIterator & rhs) const
	{
		return (m_row == rhs.m_row && m_p == rhs.m_p);
	}

	const int& getCurrentRow() const
	{
		return m_row;
	}

private:
	// Pointer to a generic matrix type T
	T * m_p;

	// Current row of the matrix that the iterator is pointing at
	int m_row;
};

template <typename T, bool isConst = false>
inline bool operator==( const VectorXXsIterator<T,isConst> & lhs,
						const VectorXXsIterator<T,isConst> & rhs)
{
	return lhs.equal( rhs );
}

template <typename T, bool isConst = false>
inline bool operator!=( const VectorXXsIterator<T,isConst> & lhs,
						const VectorXXsIterator<T,isConst> & rhs)
{
	return !( lhs.equal( rhs ) );
}

typedef VectorXXsIterator<VectorX2s, false> VectorX2s_iterator;
typedef VectorXXsIterator<const VectorX2s, true> VectorX2s_const_iterator;

namespace mathdef {
	void resize( VectorX2s & v, int size );
	void resize( VectorX1s & v, int size );
	void resize( VectorX1i & v, int size );
	void setZero( VectorX2s & v );
	void setZero( VectorX1s & v );
	void setZero( VectorX1i & v );
}

#endif
