/*
 *  HalfEdgeUtil.h
 *  Theme 5, Milestone 1.
 *  Bezier Curve.
 *
 *  Created by Papoj Thamjaroenporn on 10/21/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BEZIER_H
#define BEZIER_H

#include "MathDef.h"

// Compute combinations C(n,r).
int computeNchooseR (const int & n, const int & r);

// Given the index of parameter subdivision, compute corresponding parameter t
scalar computeParameterT( const int & index , const int & rate);

#endif