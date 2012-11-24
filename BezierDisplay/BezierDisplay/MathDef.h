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

// Todo: delete m_p.
class VectorX2sIterator : public std::iterator<std::forward_iterator_tag, VectorX2s>
{
public:
	VectorX2sIterator( VectorX2s & v, VectorX1i & degs );
	VectorX2sIterator( VectorX2s & v, VectorX1i & degs, int row );
	VectorX2sIterator( const VectorX2sIterator& vit );
	
	VectorX2sIterator& operator++();
	VectorX2sIterator operator++(int);
	VectorX2sIterator& operator+=(const int & i);
	bool operator==( const VectorX2sIterator & rhs);
	bool operator!=( const VectorX2sIterator & rhs);
	
	const int& getRow() const;
	
private:
	VectorX2s * m_p;
	VectorX1i * m_degs;
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
