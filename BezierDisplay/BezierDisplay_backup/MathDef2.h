//
//  MathDef.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef BezierDisplay_MathDef_h2
#define BezierDisplay_MathDef_h2

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

// VectorX2sIterator is an iterator class used for iterating through VectorX2s structure.
// Can be used like a bidirectional iterator (will be useful for implementing curve connection)
//
// Note: The element returned 
class VectorX2sIterator : public std::iterator<std::bidirectional_iterator_tag, VectorX2s>
{
public:
	VectorX2sIterator();
	VectorX2sIterator(  VectorX2s & v );
	VectorX2sIterator(  VectorX2s & v, const int & row );
	VectorX2sIterator(  VectorX2sIterator& vit );
	
	void setVectorX2s(  VectorX2s & v );
	void setVectorX2s(  VectorX2s & v, const int & row );
	
	VectorX2sIterator& operator++();
	VectorX2sIterator operator++(int);
	VectorX2sIterator& operator+=(const int & i);
	VectorX2sIterator& operator--();
	VectorX2sIterator operator--(int);
	VectorX2sIterator& operator-=(const int & i);
	bool operator==( const VectorX2sIterator & rhs) const;
	bool operator!=( const VectorX2sIterator & rhs) const;
	
	//	const Eigen::Block<Eigen::Matrix<double, -1, 2, 0, -1, 2> > & operator*() const;
	// Eigen::Block<Eigen::Matrix<double, -1, 2, 0, -1, 2> > operator*();
	
	// To get reference to this block directly, DON'T CONVERT TO ANYTHING. Do:
	// Block<VectorX2s> myBlock = (*iterator);
	Eigen::Block<VectorX2s> operator*();
	
	const int& getCurrentRow() const;
	
private:
	VectorX2s * m_p;

	int m_row;
};

namespace mathdef {
	void resize( VectorX2s & v, int size );
	void resize( VectorX1s & v, int size );
	void resize( VectorX1i & v, int size );
	void setZero( VectorX2s & v );
	void setZero( VectorX1s & v );
	void setZero( VectorX1i & v );
}

#endif
