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
		segCtrlPts = m_controlPoints.block( m_iter.getCurrentRowIndex(), 0, deg + 1, 2 );

*/

#ifndef __BezierDisplay__PolyBezierCurve__
#define __BezierDisplay__PolyBezierCurve__

#include <iostream>
#include <map>
#include "MathDef.h"
#include "BezierEvaluator.h"

class PolyBezierCurve
{

// Constructors, initialization, and evaluation methods
public:
	PolyBezierCurve( const int & cid = -1 );
	PolyBezierCurve( const VectorX1i & degs, const VectorX2s & points, const int & cid = -1);

	// Evaluate a sample point on the poly curve, given segment index and arbitrary parameter t
	// (Should not be needed if only curve display)
	Vector12s eval( const int & segment, scalar t );
	
	// Generate sample points
	void generateSamplePoints( const curvedef::MapEvaluators & evalMap, const int & rate );


// Iterator methods
public:

	// Return iterator pointing the beginning segment, or the end segment, of the curve
	// beginSegments() points to the first control points of polycurve
	// endSegments() points to the last control points of polycurve (passed last segment)
	VectorX2s_iterator beginSegments();
	VectorX2s_iterator endSegments();
	
	// Helper methods for segment iterator m_iter.
	
	// IMPORTANT: after getting iterator from beginSegments() or endSegments(),
	// make sure to re-align the m_currentRow through either resetToFirstSegment() or
	// resetToFirstSegment() method too.
	void resetToFirstSegment( VectorX2s_iterator & it );
	void resetToEndSegment( VectorX2s_iterator & it );
	
	void advanceSegment( VectorX2s_iterator & it );
	void gotoSegment(const int & segment, VectorX2s_iterator & it );	// Not cheap. Use advanceSegment if can
	bool isIterAtBegin( const VectorX2s_iterator & it ) const;
	bool isIterAtEnd( const VectorX2s_iterator & it ) const;
	
// Helper methods (Get and Set)
public:
	// To return points, they may not have reference
	// to original copy since temporary function like block() and row() do not allow us to
	// return its modifiable copy. Therefore, return indices to control points instead.
	VectorX1s getSegmentCtrlPtsIndices( const int & segment );
	VectorX1s getSegmentCtrlPtsIndices( const VectorX2s_iterator & it );
	
	// Set samplePoint, given one index
	void setSamplePoint( const int & row, const Vector12s & v);
	// Set controlPoint, given an entire VectorX2s
	void setControlPoints( const VectorX2s & ctrlPoints );
	
	// Set ID
	void setID( const int & cid );
	int getID() const;
	
	int getNumSegments() const;
	int getNumControlPoints() const;
	int getNumSamplePoints() const;
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

	// Series of degrees for each Bezier segment
	// For example, a poly curve that comes with linear, linear, quad, linear segments
	// respectively will have m_degs = transpose of [1,1,2,1]
	VectorX1i m_degs;
	
	// Array of control points
	VectorX2s m_controlPoints;
	
	// Segment iterator (iterating through control points though) and segment count.
	// Useful for drawing segments iteratively.
	int m_currentSegment;				// Should only be used internally, and temporarily
	
	// Array of sample points
	VectorX2s m_samplePoints;
	
};




#endif /* defined(__BezierDisplay__PolyBezierCurve__) */
