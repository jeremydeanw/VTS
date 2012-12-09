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

// Iterator should ususally accept the pointers to data!
template <typename T, bool isConst = false>
class VecterXXsIterator
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

	// Good thing to know: copy constructor can access private
	// variables of another object of the same class. The variable
	// scope works on class level, not on object level.
	VectorXXsIterator(const VectorXXsIterator<T, false> vit)
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
	Eigen::Block<T> operator*()
	{
		return m_p->block(m_row, 0, 1, m_p->cols());
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

	VectorXXsIterator& VectorXXsIterator::operator+=(const int & i)
	{
		m_row += i;
		return *this;
	}

	VectorXXsIterator& VectorXXsIterator::operator--()
	{
		--m_row;
		return *this;
	}

	VectorXXsIterator VectorXXsIterator::operator--(int)
	{
		VectorXXsIterator tmp(*this);
		operator--();
		return tmp;
	}

	VectorXXsIterator& VectorXXsIterator::operator-=(const int & i)
	{
		m_row -= i;
		return *this;
	}
	
	bool equal(const VectorXXsIterator & rhs) const
	{
		return (m_row == rhs.m_row && m_p == rhs.m_p);
	}

	const int& VectorXXsIterator::getCurrentRow() const
	{
		return m_row;
	}

private:
	T * m_p;

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
	return !( rhs.equal( rhs ) );
}

typedef VecterXXsIterator<VectorX2s, false> VecterX2s_iterator;
typedef VecterXXsIterator<const VectorX2s, true> VecterX2s_const_iterator;

namespace mathdef {
	void resize( VectorX2s & v, int size );
	void resize( VectorX1s & v, int size );
	void resize( VectorX1i & v, int size );
	void setZero( VectorX2s & v );
	void setZero( VectorX1s & v );
	void setZero( VectorX1i & v );
}

#endif
