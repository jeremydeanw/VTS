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

public:
	PolyBezierScene();
	PolyBezierScene( const int & n );
	PolyBezierScene( const int & n, const int & rate );
	
	// Given the series of degrees of a poly curve, merge all appearing degrees to m_degreeSet
	void mergeDegreeSet( const VectorX1i & degs );

	// Add new poly curve to the scene
	void addPolyCurve( const VectorX1i & degs, const VectorX2s & points );
	void addPolyCurve( const PolyBezierCurve & newCurve );
	
	// Set poly curve of a given index
	void setPolyCurve( const int & i, const VectorX1i & degs, const VectorX2s & points );
	void setPolyCurve( const int & i, const PolyBezierCurve & newCurve );
	
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
	
	// Get number of curves in the scene
	int getNumCurves() const;

// Debugging methods
public:
	void printDegreeSet() const;


private:

	// Present curves in the scene
	curvedef::VectorPolyCurve m_curves;
	curvedef::VectorPolyCurve::iterator m_curvesIt;			// Should only be used internally, and temporarily
	
	// Curves in the history, primarily meaning the curves before connection
	// TODO: Do this!
	curvedef::VectorPolyCurve m_curvesHistory;
	curvedef::VectorPolyCurve::iterator m_curvesHistoryIt;	// Should only be used internally, and temporarily
	
	// Bezier evaluators for each distinct segment degree
	curvedef::MapEvaluators m_evaluators;
	curvedef::MapEvaluators::iterator m_evaluatorsIt;		// Should only be used internally, and temporarily
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
