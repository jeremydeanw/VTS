/*
	Bezier Curve Test.
	A program that runs basic Bezier curves display. A data structure PolyBezierCurve
	is also added (supporting multiple-segment Bezier curves.
	
	The program would be able to read in XML file and extract Bezier curves from it.
	
	Milestone:
	1. Get the OpenGL running nicely. Orthographic camera. Mouse allowing zoom in-and-out.
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
// Bezier curve inputs

#include "main.h"

//The program runs from this starting point.
int main (int argc, char **argv) {
	
	PolyBezierScene mainScene;
	constructPolyCurveScene6( mainScene );
	
	curvedef::VectorPolyCurve::iterator it;
	for(it = mainScene.getCurvesBegin(); it != mainScene.getCurvesEnd(); it++)
	{
		std::cout << "ID of each curve: " << it->getID() << std::endl;
	}
		
	//mainScene.printDegreeSet();
	
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

