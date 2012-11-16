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
#include "PolyBezierCurve.h"

class PolyBezierScene
{
public:
	PolyBezierScene();
	PolyBezierScene(int n);
	

	void addPolyCurve( const VectorX1s & degs, const VectorX2s & points );
	void addPolyCurve( const PolyBezierCurve & newCurve );
	
	

private:
	std::vector<PolyBezierCurve> curves;
	std::vector<PolyBezierCurve> curvesHistory;
	
	// TODO: a variable called "Level of Detail"
};

#endif /* defined(__BezierDisplay__PolyBezierScene__) */
