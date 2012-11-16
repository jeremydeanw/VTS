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
#include "BezierUtils.h"

#include <iostream>
#include <map>
#include <cmath>

// An array of Bernstein bases B(deg, i, t). Size of array should be n + 1.
typedef VectorX1s Bases;

// Split range [0,1] using rate variable. ParamIndex corresponds to each subdivided parameter.
typedef int ParamIndex;

// TODO: Make a map of value (t) to a m_bases to reduce redundant computation
// (Make a global "LoD" variable that control the set of t's being computed.)

class BezierEvaluator {

public:

	BezierEvaluator( int deg );
	
	void generateBernsteinMap( const int & rate );
	void clearBernsteinMap();
	
	// Evaluate a sample point without using pre-computed Bernstein bases. Used for arbitrary t
	Vector12s eval( const VectorX2s & controlPoints, const scalar & t );
	
	// Evaluate a sample point using pre-computed Bernstein bases. Used when t values are
	// based on sample rate, meaning t values will likely be reused for several curves display.
	Vector12s eval( const VectorX2s & controlPoints, const int & index );
	
private:
	
	// Compute C(n,r) part of Bernstein terms; we don't know what parameter t is yet
	void initializeCombinations();
	
	// Compute Bernstein Bases
	VectorX1s computeBernsteinBases( scalar t );
	
private:
	
	// Maps from a parameter index to a set of Bernstein bases B(deg, i, t)
	std::map<ParamIndex, Bases> bernsteinMap;
	int m_rate;
	
	// Todo: Take this out.
	// Bernstein Bases for each control point
	// VectorX1s m_bases;
	
	// Combination part in Bernstein Bases
	VectorX1s m_combinations;
	
	// Degree of curve to be evaluated
	int m_deg;
	
	

};

#endif /* defined(__BezierDisplay__BezierEvaluator__) */