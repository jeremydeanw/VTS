/*
	Bezier Curve Test.
	A program that runs basic Bezier curves display. A data structure PolyBezierCurve
	is also added (supporting multiple-segment Bezier curves.
	
	The program would be able to read in XML file and extract Bezier curves from it.
	
	Milestone:
	1. Get the OpenGL running nicely. Orthographic camera. Mouse allowing zoom in-and-out.
 */
//#include "Imported.h"
#include "OpenGLScene.h"
#include "BezierUtils.h"
#include "BezierEvaluator.h"
#include "PolyBezierCurve.h"

#include <iostream>
//#include "BezierUtils.h"
//
//list<BezierCurve> curves;
//list<BezierSurface> surfaces;
//int total = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL grid parameters
int gridSizeX = 5;
int gridSizeY = 5;

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

	int rate = 10;
	myEve1.generateBernsteinMap(rate);
	
	for (int i = 0; i <= rate; i++) {
		std::cout << "eval at t = " << computeParameterT(i, rate) << ": " << std::endl;
		std::cout << myEve1.eval(controlPoints1, i) << std::endl;
	}

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
	
	VectorX1s degs (4);
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
	PolyBezierCurve myPolyCurve (degs, controlPoints);
	myPolyCurve.printTest();
	
//	for (int i = 0; i < degs.rows(); i++) {
//		myPolyCurve.gotoSegment(i);
//		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve.getSegmentIndices() << std::endl;
//	}

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
	

	initializeOpenGLandGLUT( argc, argv );
	initializeGrid(gridSizeX, gridSizeY);
	runOpenGL();
	
    return 0;
}

