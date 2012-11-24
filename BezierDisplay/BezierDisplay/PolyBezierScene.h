//
//  PolyBezierScene.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/15/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef __BezierDisplay__PolyBezierScene__
#define __BezierDisplay__PolyBezierScene__

#include <iostream>
#include <vector>
#include <set>
#include "PolyBezierCurve.h"
#include "BezierEvaluator.h"

class PolyBezierScene
{
public:
	PolyBezierScene();
	PolyBezierScene( const int & n );
	PolyBezierScene( const int & n, const int & rate );
	
	// Given the series of degrees of a poly curve, merge all appearing degrees to m_degreeSet
	void mergeDegreeSet( const VectorX1s & degs );

	// Add new poly curve to the scene
	void addPolyCurve( const VectorX1s & degs, const VectorX2s & points );
	void addPolyCurve( const PolyBezierCurve & newCurve );
	
	// Set poly curve of a given index
	void setPolyCurve( const int & i, const VectorX1s & degs, const VectorX2s & points );
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
	void setLoD( const int & rate );
	
	int getNumCurves() const;

private:

	// Present curves in the scene
	std::vector<PolyBezierCurve> m_curves;
	std::vector<PolyBezierCurve>::iterator m_curvesIt;
	
	// Curves in the history, primarily meaning the curves before connection
	// TODO: Do this!
	std::vector<PolyBezierCurve> m_curvesHistory;
	std::vector<PolyBezierCurve>::iterator m_curvesHistoryIt;
	
	// Bezier evaluators for each distinct segment degree
	std::vector<BezierEvaluator> m_evaluators;
	std::vector<BezierEvaluator>::iterator m_evaluatorsIt;
	bool m_evaluatorInitialized;
	
	// Set of all curve degrees that appear in the poly curve segments
	// For example, if the entire scene of poly curves has only cubic and quadratic segments,
	// the set should be: {2,3}
	std::set<int> m_degreeSet;
	std::set<int>::iterator m_degreeSetIt;
	
	// Level of Details dictates how fine the curve will be sampled and displayed
	int m_LoD;
	bool m_resample;
};

#endif /* defined(__BezierDisplay__PolyBezierScene__) */
