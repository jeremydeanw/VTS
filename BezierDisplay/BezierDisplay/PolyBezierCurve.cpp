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
, m_iter(segmentsBegin())
, m_currentSegment(0)
{}

//PolyBezierCurve::PolyBezierCurve( const int & segments )
//: m_degs(segments)
//, m_segments(<#args#>)
//{
//	mathdef::resize(m_degs, segments);
//	mathdef::resize(m_controlPoints, segments);
//}

PolyBezierCurve::PolyBezierCurve( const VectorX1i & degs, const VectorX2s & points )
: m_degs(degs)
, m_controlPoints(points)
, m_segments(degs.rows())
, m_iter(segmentsBegin())
, m_currentSegment(0)
{}

void PolyBezierCurve::printTest()
{
	std::cout << "m_degs:\n" << m_degs << std::endl;
	std::cout << "m_controlPoints:\n" << m_controlPoints << std::endl;
	std::cout << "m_segments: " << m_segments << std::endl;
}

Vector12s PolyBezierCurve::eval( const int & segment, scalar t )
{
	assert( segment >= 0 && segment < m_segments );
	assert( t >= 0.0 && t <= 1.0 );
	
	curvedef::Degree deg = getSegmentDegree( segment );
	BezierEvaluator eval ( deg );
	
	gotoSegment( segment );
	
	std::cout << eval.eval( m_controlPoints.block( m_iter.getCurrentRow(), 0, deg + 1, 2 ) , t) << std::endl;
	
	return eval.eval( m_controlPoints.block( m_iter.getCurrentRow(), 0, deg + 1, 2 ) , t);
	
}

//// TODO: Delete this
//void PolyBezierCurve::setSampleRate( const int & rate )
//{
//	// Hmm I need to keep the rate here...
//
//	mathdef::resize( m_samplePoints, rate + 1 );
//	mathdef::setZero( m_samplePoints );
//}

void PolyBezierCurve::generateSamplePoints( const curvedef::MapEvaluators & evalMap,
											const int & rate )
{
	assert( !evalMap.empty() );

	mathdef::resize( m_samplePoints, rate * getNumSegments() + 1 );
	mathdef::setZero( m_samplePoints );
	
	// Evaluate the first point on the first curve, then
	// Loop through all segment to sample 1st sample to last sample.
	
	resetToFirstSegment();
	
	int samp_i = 0;		// Sample temp index
	int j;
	curvedef::Degree deg;
	VectorX2s segCtrlPts;
	while (m_iter != segmentsEnd()) {
		deg = getSegmentDegree( m_currentSegment );
		const BezierEvaluator & evaluator = evalMap.find( deg )->second;
		
		assert( evaluator.isBasesComputed() );
		assert( deg > 0);
		
		segCtrlPts = m_controlPoints.block( m_iter.getCurrentRow(), 0, deg + 1, 2 );
		
		// Compute first end point if we are at first segment
		if (m_currentSegment == 0)
		{
			j = 0;
//			std::cout << "eval first t = " << computeParameterT(j, rate) << ": " << std::endl;
			setSamplePoint(samp_i, evaluator.eval(segCtrlPts, j));
//			std::cout << m_samplePoints.block(samp_i, 0, 1, 2) << std::endl;
			++samp_i;
		}
		
		j = 1;
		while (j <= rate)
		{
			// Sample segment point from index 1 to rate
//			std::cout << "eval t = " << computeParameterT(j, rate) << ": " << std::endl;
			setSamplePoint(samp_i, evaluator.eval(segCtrlPts, j));
//			std::cout << m_samplePoints.block(samp_i, 0, 1, 2) << std::endl;
			++samp_i;
			++j;
		}
		
		advanceSegment();
	}
	
//	
//	int segment = 0;	// Segment index
//	int samp_i = 0;		// Sample temp index
//	int ctrl_i = 0;		// Control point temp index
//	int j;
//	curvedef::Degree deg; int numPts;
//	VectorX2s segCtrlPts;
//	while (segment < getNumSegments())
//	{
//		deg = getSegmentDegree( segment );
//		const BezierEvaluator & evaluator = evalMap.find( deg )->second;
//		
//		assert( evaluator.isBasesComputed() );
//		assert( deg > 0);
//		
//		std::cout << "CONTROL POINTS:\n" << m_controlPoints << std::endl;
//		std::cout << "deg, ctrl_i, samp_i: " << deg << " " << ctrl_i << " " << samp_i << std::endl;
//		std::cout << m_controlPoints.block( ctrl_i, 0, deg + 1, 2) << std::endl;
//		
//		segCtrlPts = m_controlPoints.block( ctrl_i, 0, deg + 1, 2);
//		
//		// Compute first end point if we are at first segment
//		if (segment == 0)
//		{
//			j = 0;
//			std::cout << "eval first t = " << computeParameterT(j, rate) << ": " << std::endl;
//			setSamplePoint(samp_i, evaluator.eval(segCtrlPts, j));
//			std::cout << m_samplePoints.block(samp_i, 0, 1, 2) << std::endl;
//			++samp_i;
//		}
//		
//		j = 1;
//		while (j <= rate)
//		{
//			// Sample segment point from index 1 to rate
//			std::cout << "eval t = " << computeParameterT(j, rate) << ": " << std::endl;
//			setSamplePoint(samp_i, evaluator.eval(segCtrlPts, j));
//			std::cout << m_samplePoints.block(samp_i, 0, 1, 2) << std::endl;
//			++samp_i;
//			++j;
//		}
//		
//		ctrl_i += deg;
//		++segment;
//	}
	
}

void PolyBezierCurve::setSamplePoint( const int & row, const Vector12s & v)
{
	m_samplePoints.block(row, 0, 1, 2) = v;
}

VectorX2s_iterator PolyBezierCurve::segmentsBegin()
{
	return VectorX2s_iterator( &m_controlPoints, 0 );
}


//TODO: Should this be minus 1 or just 0?
//TODO: I am supposed to initialize this by pointer.
VectorX2s_iterator PolyBezierCurve::segmentsEnd()
{
	return VectorX2s_iterator( &m_controlPoints, m_controlPoints.rows() - 1);
}

//const VectorX2s& PolyBezierCurve::getPoints( const int & segment ) const
//{
//	
//		
//}

VectorX1s PolyBezierCurve::getSegmentCtrlPtsIndices( const int & segment )
{

//	std::cout << "m_segments" << m_segments << std::endl;
//	std::cout << "segment" << segment << std::endl;

	assert( (segment >= 0) && (segment < m_segments) );
	
	VectorX2s_iterator pointsBeginIter = segmentsBegin();
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
		pointIndices[i] = pointsBeginIter.getCurrentRow();
	}
	
	return pointIndices;
	
}

VectorX1s PolyBezierCurve::getSegmentCtrlPtsIndices()
{

	std::cout << "m_segments" << m_segments << std::endl;
	std::cout << "m_currentSegment" << m_currentSegment << std::endl;
	
	// Return indices of control points
	VectorX2s_iterator tmp = m_iter;
	VectorX1s pointIndices;
	mathdef::resize(pointIndices, m_degs[m_currentSegment] + 1);
	for (int i = 0; i <= m_degs[m_currentSegment]; ++i, ++tmp) {
		pointIndices[i] = tmp.getCurrentRow();
	}
	
	std::cout << "tmp.getCurrentRow(): " << tmp.getCurrentRow() << std::endl;
	std::cout << "m_iter.getCurrentRow() (should be different): " << m_iter.getCurrentRow() << std::endl;
	
	return pointIndices;
	
}

VectorX2s_iterator& PolyBezierCurve::resetToFirstSegment()
{
	m_iter = segmentsBegin();
	m_currentSegment = 0;
	
	return m_iter;
}

bool PolyBezierCurve::isIterAtBegin() const
{
	return (m_iter.getCurrentRow() == 0);
}

bool PolyBezierCurve::isIterAtEnd() const
{
	//TODO: Is this correct? (I don't think so)
	return (m_iter.getCurrentRow() == m_degs.rows());
}

VectorX2s_iterator& PolyBezierCurve::advanceSegment()
{
	m_iter += m_degs[m_currentSegment];
	++m_currentSegment;
	return m_iter;
}

VectorX2s_iterator& PolyBezierCurve::gotoSegment(const int & segment)
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

VectorX2s_iterator& PolyBezierCurve::getIter()
{
	return m_iter;
}

const VectorX2s_iterator& PolyBezierCurve::getIter() const
{
	return m_iter;
}

int& PolyBezierCurve::getNumSegments()
{
	return m_segments;
}

const int& PolyBezierCurve::getNumSegments() const
{
	return m_segments;
}

curvedef::Degree& PolyBezierCurve::getSegmentDegree( int index )
{
	assert( index >= 0 && index < m_degs.rows() );
	return m_degs[index];
}

const curvedef::Degree& PolyBezierCurve::getSegmentDegree( int index ) const
{
	assert( index >= 0 && index < m_degs.rows() );
	return m_degs[index];
}

VectorX1i& PolyBezierCurve::getDegs()
{
	return m_degs;
}

const VectorX1i& PolyBezierCurve::getDegs() const
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