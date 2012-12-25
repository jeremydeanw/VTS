/*
	Bezier Curve Test.
	A program that runs and connects Bezier curves display based on minimization of curvature
	change constraint.
	
	Currently we have 6 test cases hardcoded in main.h. One can run the following:
		constructPolyCurveScene1( mainScene );
		constructPolyCurveScene2_1( mainScene );
		constructPolyCurveScene2_2( mainScene );
		constructPolyCurveScene3_1( mainScene );
		constructPolyCurveScene3_2( mainScene );
		constructPolyCurveScene4( mainScene );
		constructPolyCurveScene5( mainScene );
	to see results of different test cases.
	
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
// Bezier curve inputs

#include "main.h"

//The program runs from this starting point.
int main (int argc, char **argv) {
	
	std::cout	<< outputmod::startgreen
				<< "Vector Texture Synthesis"
				<< outputmod::endgreen << std::flush;
	
	PolyBezierScene mainScene;
	constructPolyCurveScene5( mainScene );
	
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

