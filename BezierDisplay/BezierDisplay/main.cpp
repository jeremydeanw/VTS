/*
	Bezier Curve Test.
	A program that runs basic Bezier curves display. A data structure PolyBezierCurve
	is also added (supporting multiple-segment Bezier curves.
	
	The program would be able to read in XML file and extract Bezier curves from it.
	
	Milestone:
	1. Get the OpenGL running nicely. Orthographic camera. Mouse allowing zoom in-and-out.
 */
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
// OpenGL grid parameters
int gridMaxX = 5;
int gridMaxY = 5;
scalar gridSize = 0.5;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Bezier curve inputs


//The program runs from this starting point.
int main (int argc, char **argv) {
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
		
	
	for (int i = 0; i < degs.rows(); ++i) {
		myPolyCurve1.gotoSegment(i);
		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve1.getSegmentCtrlPtsIndices() << std::endl;
		std::cout << "First Control Points: " << *(myPolyCurve1.getIter()) << std::endl;
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
	
	PolyBezierScene mainScene;
	mainScene.addPolyCurve( myPolyCurve1 );
	mainScene.addPolyCurve( myPolyCurve2 );
	mainScene.addPolyCurve( myPolyCurve3 );
		
	mainScene.printDegreeSet();
	
	mainScene.setLoD(30);
	mainScene.initializeEvaluators();
	mainScene.computeEvaluatorCoefficients();
	mainScene.evalPolyCurveSamples();
	
	PolyBezierSceneRenderer sceneRenderer( mainScene );
	sceneRenderer.setCurveThickness(2.0);
	sceneRenderer.setCurveHullColor(1.0, 0.0, 0.0, 1.0);
	PolyBezierSceneRenderer * sceneRendererPtr = &sceneRenderer;

	initializeOpenGLandGLUT( argc, argv );
	initializeGrid(gridMaxX, gridMaxY, gridSize);
	setPolyBezierSceneRenderer( sceneRendererPtr );

	runOpenGL();
	
    return 0;
}

