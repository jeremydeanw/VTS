//
//  BezierEvaluator.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

/*
	BezierEvaluator computes and encapsulates Bernstein bases associated with a curve degree.
	Given a curve degree n, we end up having n + 1 bases we will need for Bezier curve evaluation.
	These Bernstein bases depends on not only the degree, but also the evaluating parameter t.
	
	BezierEvaluator increases an efficiency by precomputes the terms independent of parameter t, 
	which is C(n, i) where i is the index of a Bernstein basis. Given a specific t, we can then use
	a provided function to generate complete set of Bernstein bases.
	
	To increase efficiency further, we assume that the set of parameters t are often dependent
	of a level of details, or sample rate "rate". Given such variable rate, BezierEvaluator can
	precompute a full set of Bernstein bases for ALL possible parameters t (again, notice that
	each t will also be associated with a set of Bernstein bases which depends on degree n and t), 
	then stored them in a map. This map, mapping from parameter index to a set of proper Bernstein
	bases, increase efficiency when we have a scene that consists of large number of curve segments 
	with a small variation in curve degree.

*/

#ifndef __BezierDisplay__BezierEvaluator__
#define __BezierDisplay__BezierEvaluator__

#include "MathDef.h"
#include "CurveDef.h"
#include "BezierUtils.h"

#include <iostream>
#include <map>
#include <cmath>

class BezierEvaluator {
public:
	// An array of Bernstein bases B(deg, i, t). Size of array should be n + 1.
	typedef VectorX1s Bases;

	// Split range [0,1] using rate variable. ParamIndex corresponds to each subdivided parameter.
	typedef int ParamIndex;

public:
	BezierEvaluator();
	BezierEvaluator( curvedef::Degree deg );
	BezierEvaluator( const BezierEvaluator & b );
	
	// Reset evaluator's degree
	void resetEvaluatorDegree( const int & deg );
	
	// Generate Bernstein map for optimizing the curve display evaluation
	void generateBernsteinMap( const int & rate );
	
	// Clear Bernstein map
	void clearBernsteinMap();
	
	// Evaluate a sample point without using pre-computed Bernstein bases. Used for arbitrary t
	Vector12s eval( const VectorX2s & controlPoints, const scalar & t ) const;
	
	// Evaluate a sample point using pre-computed Bernstein bases. Used when t values are
	// based on sample rate, meaning t values will likely be reused for several curves display.
	Vector12s eval( const VectorX2s & controlPoints, const int & index ) const;
	
	// Tell us if the evaluator is ready to be used
	bool isBasesComputed() const;
	
private:
	
	// Compute C(n,r) part of Bernstein terms; we don't know what parameter t is yet
	void initializeCombinations();
	
	// Compute and return Bernstein Bases. Can be used for arbitrary t.
	VectorX1s computeBernsteinBases( scalar t ) const;
	
private:
	
	// Maps from a parameter index to a set of Bernstein bases B(deg, i, t)
	std::map<ParamIndex, Bases> m_bernsteinMap;
	int m_rate;
	
	bool m_basesComputed;
	
	// Combination part in Bernstein Bases
	VectorX1s m_combinations;
	
	// Degree of curve to be evaluated
	curvedef::Degree m_deg;
	

};

#endif /* defined(__BezierDisplay__BezierEvaluator__) */
