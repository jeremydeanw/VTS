//
//  PolyBezierCurve.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "PolyBezierCurve.h"

PolyBezierCurve::PolyBezierCurve()
: m_degs()
, m_controlPoints()
, m_segments(0)
, m_iter(pointsBegin())
, m_currentSegment(0)
{}

//PolyBezierCurve::PolyBezierCurve( const int & segments )
//: m_degs(segments)
//, m_segments(<#args#>)
//{
//	mathdef::resize(m_degs, segments);
//	mathdef::resize(m_controlPoints, segments);
//}

PolyBezierCurve::PolyBezierCurve( const VectorX1s & degs, const VectorX2s & points )
: m_degs(degs)
, m_controlPoints(points)
, m_segments(degs.rows())
, m_iter(pointsBegin())
, m_currentSegment(0)
{}

void PolyBezierCurve::printTest()
{
	std::cout << "m_degs:\n" << m_degs << std::endl;
	std::cout << "m_controlPoints:\n" << m_controlPoints << std::endl;
	std::cout << "m_segments: " << m_segments << std::endl;
}

// Evaluate a sample point on the poly curve, given segment index and parameter t
Vector12s PolyBezierCurve::eval( const int & segment, scalar t )
{
	
}

void PolyBezierCurve::setSampleRate( const int & rate )
{

}

void PolyBezierCurve::generateSamplePoints()
{
	
}

VectorX2sIterator PolyBezierCurve::pointsBegin()
{
	return VectorX2sIterator( m_controlPoints, m_degs, 0 );
}

VectorX2sIterator PolyBezierCurve::pointsEnd()
{
	return VectorX2sIterator( m_controlPoints, m_degs, m_controlPoints.rows() - 1);
}

//const VectorX2s& PolyBezierCurve::getPoints( const int & segment ) const
//{
//	
//		
//}

VectorX1s PolyBezierCurve::getSegmentIndices( const int & segment )
{

	std::cout << "m_segments" << m_segments << std::endl;
	std::cout << "segment" << segment << std::endl;

	assert( (segment >= 0) && (segment < m_segments) );
	
	VectorX2sIterator pointsBeginIter = pointsBegin();
	int segCount = 0;
	
	// Advance the iterator of the control points by the degree of each precedented segment
	while (segCount < segment) {
		pointsBeginIter += m_degs[segCount];
		++segCount;
	}
	
	// Return indices of control points
	VectorX1s pointIndices;
	mathdef::resize(pointIndices, m_degs[segment] + 1);
	for (int i = 0; i <= m_degs[segment]; i++, pointsBeginIter++) {
		pointIndices[i] = pointsBeginIter.getRow();
	}
	
	return pointIndices;
	
}

VectorX1s PolyBezierCurve::getSegmentIndices()
{

	std::cout << "m_segments" << m_segments << std::endl;
	std::cout << "m_currentSegment" << m_currentSegment << std::endl;
	
	// Return indices of control points
	VectorX2sIterator tmp = m_iter;
	VectorX1s pointIndices;
	mathdef::resize(pointIndices, m_degs[m_currentSegment] + 1);
	for (int i = 0; i <= m_degs[m_currentSegment]; i++, tmp++) {
		pointIndices[i] = tmp.getRow();
	}
	
	std::cout << "tmp.getRow(): " << tmp.getRow() << std::endl;
	std::cout << "m_iter.getRow() (should be different): " << m_iter.getRow() << std::endl;
	
	return pointIndices;
	
}

VectorX2sIterator& PolyBezierCurve::resetToFirstSegment()
{
	m_iter = pointsBegin();
	m_currentSegment = 0;
	
	return m_iter;
}

bool PolyBezierCurve::isIterAtBegin() const
{
	return (m_iter.getRow() == 0);
}

bool PolyBezierCurve::isIterAtEnd() const
{
	return (m_iter.getRow() == m_degs.rows());
}

VectorX2sIterator& PolyBezierCurve::advanceSegment()
{
	m_iter += m_degs[m_currentSegment];
	++m_currentSegment;
	return m_iter;
}

VectorX2sIterator& PolyBezierCurve::gotoSegment(const int & segment)
{
	assert( (segment >= 0) && (segment < m_segments) );
	
	bool forward = (segment >= m_currentSegment);
	
	// Advance the iterator of the control points by the degree of each precedented segment
	while (m_currentSegment != segment) {
		m_iter += ((forward) ? (m_degs[m_currentSegment]) : (-m_degs[m_currentSegment-1]));
		m_currentSegment += ((forward) ? 1 : -1);
	}
	
	return m_iter;
}

VectorX2sIterator& PolyBezierCurve::getIter()
{
	return m_iter;
}

const VectorX2sIterator& PolyBezierCurve::getIter() const
{
	return m_iter;
}


VectorX1s& PolyBezierCurve::getDegs()
{
	return m_degs;
}

const VectorX1s& PolyBezierCurve::getDegs() const
{
	return m_degs;
}

VectorX2s& PolyBezierCurve::getControlPoints()
{
	return m_controlPoints;
}

const VectorX2s& PolyBezierCurve::getControlPoints() const
{
	return m_controlPoints;
}

VectorX2s& PolyBezierCurve::getSamplePoints()
{
	return m_samplePoints;
}

const VectorX2s& PolyBezierCurve::getSamplePoints() const
{
	return m_samplePoints;
}