//
//  BezierUtil.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef BEZIER_H
#define BEZIER_H

#include "MathDef.h"

// A really simple class that can "flip" index of an array, given
// the array size, and the direction.
// If forward is true, return original index.
// If forward is false, flip.
class IndexConverter
{
public:
	IndexConverter(const int & size, const bool & forward)
	: m_size( size )
	, m_forward( forward )
	{}
	
	int index(const int & i) { return (m_forward ? i : (m_size - i - 1)); }
	
private:
	int m_size;
	int m_forward;
};

// Compute combinations C(n,r).
int computeNchooseR (const int & n, const int & r);

// Given the index of parameter subdivision, compute corresponding parameter t
scalar computeParameterT( const int & index , const int & rate);

#endif