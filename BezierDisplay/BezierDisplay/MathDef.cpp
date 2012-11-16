//
//  MathDef.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "MathDef.h"

VectorX2sIterator::VectorX2sIterator(VectorX2s & v, VectorX1s & degs )
: m_row( 0 )
, m_degs( &degs )
, m_p( &v )
{}

VectorX2sIterator::VectorX2sIterator(VectorX2s & v, VectorX1s & degs, int row )
: m_row( row )
, m_degs( &degs )
, m_p( &v )
{}

// Good thing to know: copy constructor can access private
// variables of another object of the same class. The variable
// scope works on class level, not on object level.
VectorX2sIterator::VectorX2sIterator(const VectorX2sIterator& vit)
: m_p( vit.m_p )
, m_degs( vit.m_degs )
, m_row( vit.m_row )
{}

VectorX2sIterator& VectorX2sIterator::operator++()
{
	++m_row;
	return *this;
}

VectorX2sIterator VectorX2sIterator::operator++(int)
{
	VectorX2sIterator tmp(*this);
	operator++();
	return tmp;
}

VectorX2sIterator& VectorX2sIterator::operator+=(const int & i)
{
	m_row += i;
	return *this;
}

// No operator*() implemented since temporary modification from block() or row() operations not allowed.
// Use block() or row() explicitly.

bool VectorX2sIterator::operator==( const VectorX2sIterator & rhs)
{
	return (m_row == rhs.m_row && m_p == rhs.m_p);
}

bool VectorX2sIterator::operator!=( const VectorX2sIterator & rhs)
{
	return (m_row != rhs.m_row || m_p != rhs.m_p);
}

const int& VectorX2sIterator::getRow() const
{
	return m_row;
}





void mathdef::resize( VectorX2s & v, int size )
{
	v.resize( size, 2 );
}

void mathdef::resize( VectorX1s & v, int size )
{
	v.resize( size, 1 );
}

