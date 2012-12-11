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

//#include "BezierUtils.h"
//
//list<BezierCurve> curves;
//list<BezierSurface> surfaces;
//int total = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Global variables

// Current available number for new curve ID assignment
int g_globalID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL grid parameters

int gridMaxX = 5;
int gridMaxY = 5;
scalar gridSize = 0.5;

void constructPolyCurveScene1( PolyBezierScene & scene )
{
	//	string curveFile (argv[1]);
//	readBezierFile(curveFile, curves, surfaces, total);

//	getBezierInput(curves, surfaces);
	
	BezierEvaluator myEve1 (3);
	VectorX2s controlPoints1;
	mathdef::resize(controlPoints1, 4);
	controlPoints1 <<
	0,0,
	1,0,
	1,1,
	1,2;



//	for( scalar t = 0.0; t <= 1.0; t += 0.1) {
//		std::cout << "eval at t = " << t << ": " << std::endl;
//		std::cout << myEve1.eval(controlPoints1, t) << std::endl;
//	}
	
	BezierEvaluator myEve2 (2);
	VectorX2s controlPoints2;
	mathdef::resize(controlPoints2, 2);
	controlPoints2 <<
	1,3,
	2,3;

//	for( scalar t = 0.0; t <= 1.0; t += 0.1) {
//		std::cout << "eval at t = " << t << ": " << std::endl;
//		std::cout << myEve2.eval(controlPoints2, t) << std::endl;
//	}
	
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
		
	
	VectorX2s_iterator curve1_It = myPolyCurve1.beginSegments();
	for (int i = 0; i < degs.rows(); ++i) {
		myPolyCurve1.gotoSegment(i, curve1_It);
		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve1.getSegmentCtrlPtsIndices(i) << std::endl;
		std::cout << "Segment " << i << " point indices: (same)\n" << myPolyCurve1.getSegmentCtrlPtsIndices(curve1_It) << std::endl;
		std::cout << "First Control Points: " << *(curve1_It) << std::endl;
	}

//	int i;
//	
//	for (i = degs.rows()-1; i >= 0; i--) {
//		myPolyCurve.gotoSegment(i);
//		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve.getSegmentIndices() << std::endl;
//	}
//	
//	myPolyCurve.gotoSegment(2); i =2;
//		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve.getSegmentIndices() << std::endl;
//		
//	myPolyCurve.advanceSegment(); i++;
//		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve.getSegmentIndices() << std::endl;
//	
//	myPolyCurve.resetToFirstSegment(); i=0;
//		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve.getSegmentIndices() << std::endl;

	scene.addPolyCurve( myPolyCurve1, g_globalID );
	//scene.addPolyCurve( myPolyCurve2, g_globalID );
	scene.addPolyCurve( degs_2, controlPoints_2, g_globalID);
	scene.addPolyCurve( myPolyCurve3, g_globalID );
}

// Connect one 1-segment curve and one 1-segment curve together.
// Both connecting at "Last" point.
void constructPolyCurveScene2( PolyBezierScene & scene )
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
		-3, 0,
		-4, -1,
		-5, -1,
		-6, 0;

	scene.addPolyCurve( degs1, controlPoints1, g_globalID);
	scene.addPolyCurve( degs2, controlPoints2, g_globalID);
}

// Connect one 2-segment curve and one 1-segment curve together.
// Both connecting at "Last" point.
void constructPolyCurveScene3( PolyBezierScene & scene )
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

// Connect one 2-segment curve and one 2-segment curve together.
// Both connecting at "Last" point.
void constructPolyCurveScene4( PolyBezierScene & scene )
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

// Connect one 3-segment curve and one 3-segment curve together.
// Both connecting at "Last" point.
void constructPolyCurveScene5( PolyBezierScene & scene )
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
		
	std::cout << "controlPoints2.colwise().reverse();\n" << controlPoints2.colwise().reverse() << std::endl;
	
//		VectorX1i degs1 (1);
//	degs1 << 3;
//	VectorX2s controlPoints1;
//	mathdef::resize( controlPoints1, 4);
//	controlPoints1 <<
//		-10, 0,
//		-9, 1,
//		-8, 1,
//		-7, 0;
//	PolyBezierCurve myPolyCurve1 (degs1, controlPoints1);

	scene.addPolyCurve( degs1, controlPoints1, g_globalID);
	scene.addPolyCurve( degs2, controlPoints2.colwise().reverse(), g_globalID);
}

// Connect one 4-segment curve and one 5-segment curve together.
// Both connecting at "Last" point.
void constructPolyCurveScene6( PolyBezierScene & scene )
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
		
	std::cout << "controlPoints2.colwise().reverse();\n" << controlPoints2.colwise().reverse() << std::endl;
	
//		VectorX1i degs1 (1);
//	degs1 << 3;
//	VectorX2s controlPoints1;
//	mathdef::resize( controlPoints1, 4);
//	controlPoints1 <<
//		-10, 0,
//		-9, 1,
//		-8, 1,
//		-7, 0;
//	PolyBezierCurve myPolyCurve1 (degs1, controlPoints1);

	scene.addPolyCurve( degs1, controlPoints1, g_globalID);
	scene.addPolyCurve( degs2, controlPoints2.colwise().reverse(), g_globalID);
}

#endif
