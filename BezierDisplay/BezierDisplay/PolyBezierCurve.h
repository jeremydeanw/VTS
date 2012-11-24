//
//  PolyBezierCurve.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef __BezierDisplay__PolyBezierCurve__
#define __BezierDisplay__PolyBezierCurve__

#include <iostream>
#include <map>
#include "MathDef.h"
#include "BezierEvaluator.h"

//typedef std::map<BezierEvaluator::Degree, BezierEvaluator> MapEvaluators;

class PolyBezierCurve
{
//public:
//	typedef std::map<BezierEvaluator::Degree, BezierEvaluator> MapEvaluators;

public:
	PolyBezierCurve();
//	PolyBezierCurve( const int & segments );
	PolyBezierCurve( const VectorX1i & degs, const VectorX2s & points );
	
	// Evaluate a sample point on the poly curve, given segment index and parameter t
	Vector12s eval( const int & segment, scalar t );
	
	// Set sample rate
	void setSampleRate( const int & rate );
	
	// Generate sample points
	void generateSamplePoints( const curvedef::MapEvaluators & evalMap, const int & rate );
	
	// Set samplePoint
	void setSamplePoint( const int & row, const Vector12s & v);
	
	void printTest();
	
public:

	// Return iterator pointing the beginning segment, or the end segment, of the curve
	VectorX2sIterator pointsBegin();
	VectorX2sIterator pointsEnd();
	
	// Helper methods for segment iterator m_iter.
	VectorX2sIterator& resetToFirstSegment();
	VectorX2sIterator& advanceSegment();
	VectorX2sIterator& gotoSegment(const int & segment);	// Not cheap. Use advanceSegment if can
	VectorX2sIterator& getIter();
	const VectorX2sIterator& getIter() const;
	bool isIterAtBegin() const;
	bool isIterAtEnd() const;
	
	
	// To return points, they may not have reference
	// to original copy since temporary function like block() and row() do not allow us to
	// return its modifiable copy. Therefore, return indices to control points instead.
	VectorX1s getSegmentIndices();
	VectorX1s getSegmentIndices( const int & segment );
	
	int& getNumSegments();
	const int& getNumSegments() const;
	curvedef::Degree& getSegmentDegree( int index );
	const curvedef::Degree& getSegmentDegree( int index ) const;
	VectorX1i& getDegs();
	const VectorX1i& getDegs() const;
	VectorX2s& getControlPoints();
	const VectorX2s& getControlPoints() const;
	VectorX2s& getSamplePoints();
	const VectorX2s& getSamplePoints() const;
	
private:

	// Number of segments
	int m_segments;

	// Series of degrees for each Bezier segment
	// For example, a poly curve that comes with linear, linear, quad, linear segments
	// respectively will have m_degs = transpose of [1,1,2,1]
	VectorX1i m_degs;
	
	// Array of control points
	VectorX2s m_controlPoints;
	
	// Segment iterator and segment count. Useful for drawing segments iteratively.
	VectorX2sIterator m_iter;
	int m_currentSegment;
	
	// Array of sample points
	VectorX2s m_samplePoints;
	
	// TODO: Have a method that evaluates all sample points
};




#endif /* defined(__BezierDisplay__PolyBezierCurve__) */
