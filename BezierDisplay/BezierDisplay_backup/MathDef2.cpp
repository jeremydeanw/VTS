//
//  MathDef.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "MathDef2.h"

VectorX2sIterator::VectorX2sIterator()
: m_row( 0 )
, m_p( NULL )
{}


VectorX2sIterator::VectorX2sIterator(  VectorX2s & v )
: m_row( 0 )
, m_p( &v )
{}

VectorX2sIterator::VectorX2sIterator(  VectorX2s & v, const int & row )
: m_row( row )
, m_p( &v )
{}

// Good thing to know: copy constructor can access private
// variables of another object of the same class. The variable
// scope works on class level, not on object level.
VectorX2sIterator::VectorX2sIterator(const VectorX2sIterator& vit)
: m_p( vit.m_p )
, m_row( vit.m_row )
{}

void VectorX2sIterator::setVectorX2s( VectorX2s & v )
{
	m_row = 0;
	m_p = &v;
}

void VectorX2sIterator::setVectorX2s( VectorX2s & v, const int & row )
{
	m_row = row;
	m_p = &v;
}

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

VectorX2sIterator& VectorX2sIterator::operator--()
{
	--m_row;
	return *this;
}

VectorX2sIterator VectorX2sIterator::operator--(int)
{
	VectorX2sIterator tmp(*this);
	operator--();
	return tmp;
}

VectorX2sIterator& VectorX2sIterator::operator-=(const int & i)
{
	m_row -= i;
	return *this;
}

bool VectorX2sIterator::operator==( const VectorX2sIterator & rhs) const
{
	return (m_row == rhs.m_row && m_p == rhs.m_p);
}

bool VectorX2sIterator::operator!=( const VectorX2sIterator & rhs) const
{
	return (m_row != rhs.m_row || m_p != rhs.m_p);
}

//Vector12s VectorX2sIterator::operator*() const
//{
//	Vector12s elem = m_p->block(m_row, 0, 1, 2);
//	return elem;
//}

Eigen::Block<VectorX2s> VectorX2sIterator::operator*()
{
	return m_p->block(m_row, 0, 1, 2);
}

//Eigen::Block<Eigen::Matrix<double, -1, 2, 0, -1, 2> > VectorX2sIterator::operator*()
//{
//	return m_p->block(m_row, 0, 1, 2);
//}

const int& VectorX2sIterator::getCurrentRow() const
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

void mathdef::resize( VectorX1i & v, int size )
{
	v.resize( size, 1 );
}

void mathdef::setZero( VectorX2s & v )
{
	v = VectorX2s::Zero(v.rows(), 2);
}

void mathdef::setZero( VectorX1s & v )
{
	v = VectorX1s::Zero(v.rows(), 1);
}

void mathdef::setZero( VectorX1i & v )
{
	v = VectorX1i::Zero(v.rows(), 1);
}

