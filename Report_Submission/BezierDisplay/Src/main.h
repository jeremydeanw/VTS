//
//  main.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 12/11/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef BezierDisplay_main_h
#define BezierDisplay_main_h

//#include "Imported.h"
#include "MathDef.h"
#include "OpenGLScene.h"
#include "BezierUtils.h"
#include "BezierEvaluator.h"
#include "PolyBezierCurve.h"
#include "PolyBezierScene.h"
#include "PolyBezierSceneRenderer.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
// Global variables

// Current available number for new curve ID assignment
int g_globalID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL grid parameters

int gridMaxX = 5;
int gridMaxY = 5;
scalar gridSize = 0.5;

/*
	This scene illustrates three generic Poly-Bezier curves.
	The segments of each curve have various degrees, ranging from
	degree 1 to 4.
*/
void constructPolyCurveScene1( PolyBezierScene & scene )
{
	
	BezierEvaluator myEve1 (3);
	VectorX2s controlPoints1;
	mathdef::resize(controlPoints1, 4);
	controlPoints1 <<
	0,0,
	1,0,
	1,1,
	1,2;
	
	BezierEvaluator myEve2 (2);
	VectorX2s controlPoints2;
	mathdef::resize(controlPoints2, 2);
	controlPoints2 <<
	1,3,
	2,3;
	
	VectorX1i degs (4);
	degs << 3, 2, 1, 3;
	VectorX2s controlPoints;
	mathdef::resize( controlPoints, controlPoints1.rows() + controlPoints2.rows() + 4);
	controlPoints <<
		controlPoints1,
		controlPoints2,
		3,3,
		4,3,
		4,2,
		4,1;
	PolyBezierCurve myPolyCurve1 (degs, controlPoints);
	
	
	VectorX1i degs_2 (5);
	degs_2 << 2, 2, 2, 3, 1;
	VectorX2s controlPoints_2;
	mathdef::resize( controlPoints_2, 11);
	controlPoints_2 <<
		0, -1,
		-5, -1,
		-5, -6,
		0, -6,
		0, -2,
		-4, -2,
		-4, -5,
		-2, -5,
		-2, -3,
		-3, -3,
		-3, -4;
	PolyBezierCurve myPolyCurve2 (degs_2, controlPoints_2);
	
	VectorX1i degs_3 (1);
	degs_3 << 4;
	VectorX2s controlPoints_3;
	mathdef::resize( controlPoints_3, 5);
	controlPoints_3 <<
		-1, 1,
		-2, 1,
		-3, 2,
		-1, 4,
		-3, 4;
	PolyBezierCurve myPolyCurve3 (degs_3, controlPoints_3);

	scene.addPolyCurve( myPolyCurve1, g_globalID );
	scene.addPolyCurve( degs_2, controlPoints_2, g_globalID);
	scene.addPolyCurve( myPolyCurve3, g_globalID );
}

/*
	This scene illustrates a scene with two degree-3 curves, both simply
	with only one segment. This scene demonstrate an ideal scenario where
	the endpoints have precisely the opposite direction to each other. 
	This case 2.1 has two curves lined up close to each other.
	Connection works smoothly on this scene.
*/
void constructPolyCurveScene2_1( PolyBezierScene & scene )
{
	
	VectorX1i degs1 (1);
	degs1 << 3;
	VectorX2s controlPoints1;
	mathdef::resize( controlPoints1, 4);
	controlPoints1 <<
		-1, 0,
		0, 1,
		2, 1,
		3, 0;
	
	VectorX1i degs2 (1);
	degs2 << 3;
	VectorX2s controlPoints2;
	mathdef::resize( controlPoints2, 4);
	controlPoints2 <<
		7, 0,
		6, -1,
		5, -1,
		4, 0;

	scene.addPolyCurve( degs1, controlPoints1, g_globalID);
	scene.addPolyCurve( degs2, controlPoints2, g_globalID);
}

/*
	This scene illustrates a scene with two degree-3 curves, both simply
	with only one segment. This scene demonstrate an ideal scenario where
	the endpoints have precisely the opposite direction to each other. 
	This case 2.1 has two curves lined up close to each other.
	Connection works smoothly on this scene.
*/
void constructPolyCurveScene2_2( PolyBezierScene & scene )
{
	
	VectorX1i degs1 (1);
	degs1 << 3;
	VectorX2s controlPoints1;
	mathdef::resize( controlPoints1, 4);
	controlPoints1 <<
		-10, 0,
		-9, 1,
		-8, 1,
		-7, 0;
	
	VectorX1i degs2 (1);
	degs2 << 3;
	VectorX2s controlPoints2;
	mathdef::resize( controlPoints2, 4);
	controlPoints2 <<
		10, 0,
		9, -1,
		8, -1,
		7, 0;

	scene.addPolyCurve( degs1, controlPoints1, g_globalID);
	scene.addPolyCurve( degs2, controlPoints2, g_globalID);
}

/*
	This scene illustrates a scene with two degree-3 curves, where
	the curves are not close to each other and they do not line
	up almost at the same tangent. We use this test case to see
	how drastic the curve connection changes the control points.
*/
void constructPolyCurveScene3_1( PolyBezierScene & scene )
{
	
	VectorX1i degs1 (2);
	degs1 << 3, 3;
	VectorX2s controlPoints1;
	mathdef::resize( controlPoints1, 3*degs1.rows() + 1);
	controlPoints1 <<
		-10, 0,
		-9, 1,
		-8, 1,
		-7, 0,
		-6, -1,
		-5, -1,
		-4, 0;
	
	VectorX1i degs2 (1);
	degs2 << 3;
	VectorX2s controlPoints2;
	mathdef::resize( controlPoints2, 3*degs2.rows() + 1);
	controlPoints2 <<
		1, 0,
		1, -1,
		2, -1,
		3, 0;

	scene.addPolyCurve( degs1, controlPoints1, g_globalID);
	scene.addPolyCurve( degs2, controlPoints2, g_globalID);
}

/*
	This scene illustrates a scene with two degree-3 curves, where
	the curves are not close to each other and they do not line
	up almost at the same tangent. We use this test case to see
	how drastic the curve connection changes the control points.
*/
void constructPolyCurveScene3_2( PolyBezierScene & scene )
{
	
	VectorX1i degs1 (2);
	degs1 << 3, 3;
	VectorX2s controlPoints1;
	mathdef::resize( controlPoints1, 3*degs1.rows() + 1);
	controlPoints1 <<
		-10, 0,
		-9, 1,
		-8, 1,
		-7, 0,
		-6, -1,
		-5, -1,
		-4, 0;
	
	VectorX1i degs2 (2);
	degs2 << 3, 3;
	VectorX2s controlPoints2;
	mathdef::resize( controlPoints2, 3*degs2.rows() + 1);
	controlPoints2 <<
		1, 0,
		1, -1,
		2, -1,
		2, 0,
		2, 1,
		3, 1,
		4, 0;
	controlPoints2 = controlPoints2.colwise().reverse();

	scene.addPolyCurve( degs1, controlPoints1, g_globalID);
	scene.addPolyCurve( degs2, controlPoints2, g_globalID);
}

/*
	This scene illustrates a scene with two degree-3 curves both with
	3 segments, and with close tangent directions at end points.
	Notice how smoothly connection applies to these curves
*/
void constructPolyCurveScene4( PolyBezierScene & scene )
{
	
	VectorX1i degs1 (3);
	degs1 << 3, 3, 3;
	VectorX2s controlPoints1;
	mathdef::resize( controlPoints1, 3*degs1.rows() + 1);
	controlPoints1 <<
		-10, 0,
		-9, 1,
		-8, 1,
		-7, 0,
		-6, -1,
		-5, -2,
		-4, 0,
		-3, 2,
		-2, 2,
		-1, 0;
	
	VectorX1i degs2 (3);
	degs2 << 3, 3, 3;
	VectorX2s controlPoints2;
	mathdef::resize( controlPoints2, 3*degs2.rows() + 1);
	controlPoints2 <<
		1, 0,
		1, -1,
		2, -1,
		2, 0,
		2, 1,
		3, 1,
		4, 0,
		5, -1,
		5, 0,
		6, 0;
		
	scene.addPolyCurve( degs1, controlPoints1, g_globalID);
	scene.addPolyCurve( degs2, controlPoints2.colwise().reverse(), g_globalID);
}

/*
	This scene illustrates a scene with two degree-3 curves with
	4 and 5 segments respectively and close tangent directions at end points.
	Notice how smoothly connection applies to these curves
*/
void constructPolyCurveScene5( PolyBezierScene & scene )
{
	
	VectorX1i degs1 (4);
	degs1 << 3, 3, 3, 3;
	VectorX2s controlPoints1;
	mathdef::resize( controlPoints1, 3*degs1.rows() + 1);
	controlPoints1 <<
		-10, 1,
		-11, 1,
		-11, -1,
		-10, 0,
		-9, 1,
		-8, 1,
		-7, 0,
		-6, -1,
		-5, -2,
		-4, 0,
		-3, 2,
		-2, 2,
		-1, 0;
	
	VectorX1i degs2 (5);
	degs2 << 3, 3, 3, 3, 3;
	VectorX2s controlPoints2;
	mathdef::resize( controlPoints2, 3*degs2.rows() + 1);
	controlPoints2 <<
		1, 0,
		1, -1,
		2, -1,
		2, 0,
		2, 1,
		3, 1,
		4, 0,
		5, -1,
		5, 0,
		6, 0,
		7, 0,
		5, 1,
		4, 2,
		3, 3,
		3, 5,
		4, 4;

	scene.addPolyCurve( degs1, controlPoints1, g_globalID);
	scene.addPolyCurve( degs2, controlPoints2.colwise().reverse(), g_globalID);
}

#endif
