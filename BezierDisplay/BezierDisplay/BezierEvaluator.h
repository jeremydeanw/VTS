//
//  BezierEvaluator.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef __BezierDisplay__BezierEvaluator__
#define __BezierDisplay__BezierEvaluator__

#include "MathDef.h"
#include "CurveDef.h"
#include "BezierUtils.h"

#include <iostream>
#include <map>
#include <cmath>

// TODO: Make a map of value (t) to a m_bases to reduce redundant computation
// (Make a global "LoD" variable that control the set of t's being computed.)

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
	
	// Compute Bernstein Bases
	VectorX1s computeBernsteinBases( scalar t ) const;
	
private:
	
	// Maps from a parameter index to a set of Bernstein bases B(deg, i, t)
	std::map<ParamIndex, Bases> m_bernsteinMap;
	int m_rate;
	
	// Todo: Take this out.
	// Bernstein Bases for each control point
	// VectorX1s m_bases;
	
	bool m_basesComputed;
	
	// Combination part in Bernstein Bases
	VectorX1s m_combinations;
	
	// Degree of curve to be evaluated
	curvedef::Degree m_deg;
	
	

};

#endif /* defined(__BezierDisplay__BezierEvaluator__) */
