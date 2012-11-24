//
//  CurveDef.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/21/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef BezierDisplay_CurveDef_h
#define BezierDisplay_CurveDef_h

#include <vector>
#include <map>
#include <set>

class PolyBezierCurve;
class BezierEvaluator;

namespace curvedef {
	typedef int Degree;
	typedef std::vector<PolyBezierCurve> VectorPolyCurve;
	typedef std::map<Degree, BezierEvaluator> MapEvaluators;
	typedef std::set<Degree> SetDegree;
}

#endif
