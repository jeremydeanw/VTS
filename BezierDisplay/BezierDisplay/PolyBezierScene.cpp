//
//  PolyBezierScene.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/15/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "PolyBezierScene.h"

PolyBezierScene::PolyBezierScene()
: curves(0)
, curvesHistory(0)
{}

PolyBezierScene::PolyBezierScene(int n)
: curves(n)
, curvesHistory(0)
{}

void PolyBezierScene::addPolyCurve( const VectorX1s & degs, const VectorX2s & points )
{
	PolyBezierCurve newPolyCurve( degs, points );
	
	curves.push_back( newPolyCurve );
}

void PolyBezierScene::addPolyCurve( const PolyBezierCurve & newCurve )
{
	curves.push_back( newCurve );
}