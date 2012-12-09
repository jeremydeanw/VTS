//
//  PolyBezierScene.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/15/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

/*
	PolyBezierScene acts as a manager of an entire scene of poly Bezier curves.
	
	Main functions:
	* Encapsulates all curves in the scene in a vector of poly curves
	* Encapsulates curves history; i.e., the state of the curves right before they are joined
	* Encapsulates all Bezier evaluators. Each evaluator has an associated curve degree. Knowing
	that the same set of parameters (t) will be used globally for all curve evaluations, and having
	the evaluators coefficients for each degree pre-computed, this will give computational
	efficiency if the scene has large number of segments with the same curve degree (most likely
	to be 3)

*/

#ifndef __BezierDisplay__PolyBezierScene__
#define __BezierDisplay__PolyBezierScene__

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "CurveDef.h"
#include "PolyBezierCurve.h"
#include "BezierEvaluator.h"

class PolyBezierScene
{
//public:
//	typedef std::vector<PolyBezierCurve> VectorPolyBezierCurve;
//	typedef std::set<curvedef::Degree> SetDegree;

// TODO: Write basic instruction regarding how to set up the scene, with code example.

// Constructors, initialization, and evaluation methods of the scene
public:
	PolyBezierScene();
	PolyBezierScene( const int & n );
	PolyBezierScene( const int & n, const int & rate );

	// Given the series of degrees of a poly curve, merge all appearing degrees to m_degreeSet
	void mergeDegreeSet( const VectorX1i & degs );

	// Add new poly curve to the scene
	void addPolyCurve( const VectorX1i & degs, const VectorX2s & points, int & currentGlobalID);
	void addPolyCurve( PolyBezierCurve & newCurve, int & currentGlobalID );
	
//	// Set poly curve of a given index
//	void setPolyCurve( const int & i, const VectorX1i & degs, const VectorX2s & points );
//	void setPolyCurve( const int & i, const PolyBezierCurve & newCurve );
	
	// Initialize Bezier evaluators
	// DO THIS AFTER ALL POLY CURVES ARE SET / ADDED
	void initializeEvaluators();
	
	// Compute Bernstein coefficients based on the the sample rate, which
	// leads to different group of parameters t used for display.
	// See also: BezierUtils::computeParameterT
	void computeEvaluatorCoefficients();
	
	// Iterate through all curves and sample them
	void evalPolyCurveSamples();
	void evalPolyCurveHistorySamples();
	
	// Level of Details of the curve display
	void setLoD( const int & rate );			// DO THIS AFTER initializeEvaluators
	const int & getLoD() const;
	
	// Get curves and curve iterators
	curvedef::VectorPolyCurve & getCurves();
	const curvedef::VectorPolyCurve & getCurves() const;
	
	curvedef::VectorPolyCurve::iterator getCurvesBegin();
	curvedef::VectorPolyCurve::iterator getCurvesEnd();
	
	// Get number of curves in the scene
	int getNumCurves() const;

// Debugging methods
public:
	void printDegreeSet() const;
	
// Connection methods
// TODO: When this gets big, separate this into another class?
public:
	// Connect two curves, given by IDs. Basically, create a new curve
	// by solving new coefficients from an optimization matrix, push that into the list
	// of m_curves, while popping out the previous two curves out, and put it into m_curvesHistory.
	//
	// Input:
	// id1, id2: IDs of the two curves being connected
	// beginOrEnd1, beginOrEnd2: tells us which side of the curve to connect. If true, use the first
	//							 control point stored in the curve for connection. If false, use last.
	// weight1, weight2: weights for calculating new connecting point -- we connect p(id1) and p(id2)
	//					 by the formula newP = weight1 * p1 + weight2 * p2.
	//					 Recommended: the weights should sum up to 1.0
	void connectPolycurveThree( const int & id1, const int & id2,
								const bool & beginOrEnd1, const bool & beginOrEnd2,
								const scalar & weight1, const scalar & weight2 );
	
	PolyBezierCurve & findCurveGivenID( const int & ID );
	
	
//	// TODO:
//	void connectSelectedPolyCurveThree( )
//	{
//		connectPolycurveThree(id1, id2, side1, side2, weight1, weight2);
//	}

private:

	// Present curves in the scene
	// To get access to m_curves, use getCurvesIterator()
	curvedef::VectorPolyCurve m_curves;
	//curvedef::VectorPolyCurve::iterator m_curvesIt;			// Should only be used internally, and temporarily
	
	// Curves in the history, primarily meaning the curves before connection
	// TODO: Do this!
	// To get access to m_curvesHistory, use getHistoryCurvesIterator()
	curvedef::VectorPolyCurve m_curvesHistory;
	//curvedef::VectorPolyCurve::iterator m_curvesHistoryIt;	// Should only be used internally, and temporarily
	
	// Bezier evaluators for each distinct segment degree
	// To get access to m_evaluators, use getEvalIterator()
	curvedef::MapEvaluators m_evaluators;
	//curvedef::MapEvaluators::iterator m_evaluatorsIt;		// Should only be used internally, and temporarily
	bool m_evaluatorInitialized;
	
	// Set of all curve degrees that appear in the poly curve segments
	// For example, if the entire scene of poly curves has only cubic and quadratic segments,
	// the set should be: {2,3}
	curvedef::SetDegree m_degreeSet;
	curvedef::SetDegree::iterator m_degreeSetIt;
	
	// Level of Details dictates how fine the curve will be sampled and displayed
	int m_LoD;
	bool m_resample;
};

#endif /* defined(__BezierDisplay__PolyBezierScene__) */
