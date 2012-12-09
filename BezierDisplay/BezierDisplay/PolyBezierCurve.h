//
//  PolyBezierCurve.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

/*
	PolyBezierCurve is a data structure that contains all data about a poly curve:
	* Array of curve segments
	* Sequence of curve degrees in parallel to segments
	* (Shared among all segments) array of control points
	* (Shared among all segments) array of sample points
	
	PolyBezierCurve also provides a segment iterator. This iterator iterates through an
	array of CONTROL POINTS, so it can be used to fetch the list of control points, as seen in
	generateSamplePoints():
		segCtrlPts = m_controlPoints.block( m_iter.getCurrentRow(), 0, deg + 1, 2 );

*/

#ifndef __BezierDisplay__PolyBezierCurve__
#define __BezierDisplay__PolyBezierCurve__

#include <iostream>
#include <map>
#include "MathDef.h"
#include "BezierEvaluator.h"

//TODO: Add index to each poly curve

class PolyBezierCurve
{

// Constructors, initialization, and evaluation methods
public:
	PolyBezierCurve();
//	PolyBezierCurve( const int & segments );
	PolyBezierCurve( const VectorX1i & degs, const VectorX2s & points );
	
	// Evaluate a sample point on the poly curve, given segment index and arbitrary parameter t
	// (Should not be needed if only curve display)
	Vector12s eval( const int & segment, scalar t );
	
//	// Set sample rate. --> Not necessary. All sample rate in the scene file.
//	void setSampleRate( const int & rate );
	
	// Generate sample points
	void generateSamplePoints( const curvedef::MapEvaluators & evalMap, const int & rate );
	
	// Set samplePoint
	void setSamplePoint( const int & row, const Vector12s & v);
	
	void printTest();

// Iterator and helper methods
public:

	// Return iterator pointing the beginning segment, or the end segment, of the curve
	VectorX2s_iterator segmentsBegin();		// Points to the first control points of polycurve
	VectorX2s_iterator segmentsEnd();		// Points to the last control points of polycurve (passed last segment)
	
	// Helper methods for segment iterator m_iter.
	// TODO : Should I still keep the pointers, since I need to use pointer?
	VectorX2s_iterator& resetToFirstSegment();
	VectorX2s_iterator& advanceSegment();
	VectorX2s_iterator& gotoSegment(const int & segment);	// Not cheap. Use advanceSegment if can
	VectorX2s_iterator& getIter();
	const VectorX2s_iterator& getIter() const;
	bool isIterAtBegin() const;
	bool isIterAtEnd() const;
	
	
	// To return points, they may not have reference
	// to original copy since temporary function like block() and row() do not allow us to
	// return its modifiable copy. Therefore, return indices to control points instead.
	VectorX1s getSegmentCtrlPtsIndices();
	VectorX1s getSegmentCtrlPtsIndices( const int & segment );
	
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

	// ID number of this polycurve
	int m_id;

	// Number of segments
	int m_segments;

	// Series of degrees for each Bezier segment
	// For example, a poly curve that comes with linear, linear, quad, linear segments
	// respectively will have m_degs = transpose of [1,1,2,1]
	VectorX1i m_degs;
	
	// Array of control points
	VectorX2s m_controlPoints;
	
	// Segment iterator (iterating through control points though) and segment count.
	// Useful for drawing segments iteratively.
	VectorX2s_iterator m_iter;			// Should only be used internally, and temporarily
	int m_currentSegment;				// Should only be used internally, and temporarily
	
	// Array of sample points
	VectorX2s m_samplePoints;
	
};




#endif /* defined(__BezierDisplay__PolyBezierCurve__) */
