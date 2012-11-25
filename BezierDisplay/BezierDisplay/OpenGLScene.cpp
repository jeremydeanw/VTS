//
//  OpenGLScene.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/9/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "OpenGLScene.h"
#include "OpenGLDisplayController.h"

#include "PolyBezierScene.h"
#include "CurveDef.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
// Rendering state
//bool g_rendering_enabled = true;
//double g_sec_per_frame;

OpenGLDisplayController g_display_controller( 512, 512 );
SceneData g_scene_data;

SceneData::SceneData()
: recenterScale (1.2)
, g_bgColor( 1.0, 1.0, 1.0 )
{};

////Light enabling variables.
//bool light_0_on = true;  //By default, light 0 will be on.
//bool light_1_on = false;
//
////Shading mode variable. In this assignment, we only use GL_FLAT and GL_SMOOTH.
//bool isFlat = false;  //By default, the shading model is flat mode.
//
////Windows attribute
//int width = 920;
//int height = 690;
//int w, h;	//Dynamic sizes of width and height. Used for reshaping.
//int border = 5;
//int mainWindow;  //"Signature" number for the main window.
//int * viewWindows;
//float near_plane = 1.0;
//float far_plane = 1000.0;
///*
////Represents the camera in each subwindow.
////0 -> Top
////1 -> Perspective
////2 -> Front
////3 -> Side
//int cameraNumber = 0;
//int channels = 4;
//
////Orthographic camera attributes.
//float originalOrthoW = 80.0;
//float originalOrthoH = 60.0;
//int left_s = -originalOrthoW / 2;
//int right_s = originalOrthoW / 2;
//int top_s = originalOrthoH / 2;
//int bottom_s = -originalOrthoH / 2;
// */
////Perspective
//int viewAngle = 60;
//float zoomFactor = 0.1;
//float angle = 0.0;
//
////Main menu signatures for each subwindow.
//int mainMenu;
//int lightMenu;
//int colorMenu;
//
////Floor attributes.
//int floorX1 = -30;
//int floorX2 = 30;
//int floorZ1 = -250;
//int floorZ2 = 250;
//int floorGridW = 5;
//
////Sphere atts.
//int Nslice = 30;
//int Nstack = 40;

//GLfloat ambientGlassMaterial[] = {0.0, 0.0, 0.0};
//
//GLuint theScene;			//For Display Lists.


//typedef list<BezierCurve> * BezierCurveListPtr;
//typedef list<BezierSurface> * BezierSurfaceListPtr;
//BezierCurveListPtr curves_gl;
//BezierSurfaceListPtr surfaces_gl;
//
//int LOD = 1;
//


////////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL rendering and UI functions

//Called when the window size is changed.
void reshape (int w, int h)
{
	g_display_controller.reshape( w, h );
	
	assert( renderingutils::checkGLErrors() );
}

void setOrthographicProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	gluOrtho2D(0, g_display_controller.getWindowWidth(), 0, g_display_controller.getWindowHeight());
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	assert( renderingutils::checkGLErrors() );
}

void drawHUD()
{

}

void display()
{
	glClear( GL_COLOR_BUFFER_BIT );
	
	glMatrixMode( GL_MODELVIEW );
	
	drawGrid(*(g_scene_data.gridMaxX), *(g_scene_data.gridMaxY), *(g_scene_data.gridSize));
	g_scene_data.g_bezier_renderer->renderScene();
	
	//drawHUD();
	
	glutSwapBuffers();
	
	assert( renderingutils::checkGLErrors() );
	
}

void findSceneBoundary(scalar & min_x, scalar & min_y, scalar & max_x, scalar & max_y)
{
	const PolyBezierScene & curveScene = g_scene_data.g_bezier_renderer->getScene();
	const curvedef::VectorPolyCurve & curves = curveScene.getCurves();
	curvedef::VectorPolyCurve::const_iterator curveIt;
	
	// Compute the bounds of the scene	
	max_x = -std::numeric_limits<scalar>::infinity();
	min_x =  std::numeric_limits<scalar>::infinity();
	max_y = -std::numeric_limits<scalar>::infinity();
	min_y =  std::numeric_limits<scalar>::infinity();
	
	VectorX2sIterator controlPointsIt;
	VectorX2sIterator end;
	Vector12s point;
	for( curveIt = curves.begin(); curveIt != curves.end(); ++curveIt )
	{
		const VectorX2s & controlPoints = curveIt->getControlPoints();
		
		controlPointsIt.setVectorX2s( controlPoints );
		end.setVectorX2s( controlPoints, controlPoints.rows() );

		while ( controlPointsIt != end) {
			point = *controlPointsIt;
			
			if (point(0) < min_x) min_x = point(0);
			if (point(0) > max_x) max_x = point(0);
			if (point(1) < min_y) min_y = point(1);
			if (point(1) > max_y) max_y = point(1);
			
			++controlPointsIt;
		}
		
	}
	
//	std::cout << "min_x, max_x: " << min_x << " " << max_x << std::endl;
//	std::cout << "min_y, max_y: " << min_y << " " << max_y << std::endl;

}

void centerCamera()
{
	scalar min_x, min_y, max_x, max_y;
	findSceneBoundary( min_x, min_y, max_x, max_y );
	
	// Find the center of the bound
	scalar cx = 0.5*(min_x + max_x);
	scalar cy = 0.5*(min_y + max_y);
	
	// Set the zoom value so all control points are in view
	scalar radius_x = 0.5*(max_x - min_x);
	if (radius_x == 0.0) radius_x = 1.0;
	scalar radius_y = 0.5*(max_y - min_y);
	if (radius_y == 0.0) radius_y = 1.0;
	scalar ratio = ((scalar)g_display_controller.getWindowHeight()) /
					((scalar)g_display_controller.getWindowWidth());
	
	std::cout << "g_scene_data.recenterScale " <<   g_scene_data.recenterScale << std::endl;
	
	centerCamera( cx, cy, g_scene_data.recenterScale*std::max(ratio*radius_x, radius_y) );

}

void centerCamera( scalar cx, scalar cy )
{
	scalar min_x, min_y, max_x, max_y;
	findSceneBoundary( min_x, min_y, max_x, max_y );
	
	// Set the zoom value so all control points are in view
	scalar radius_x = std::max( std::abs(min_x), std::abs(max_x) );
	if (radius_x == 0.0) radius_x = 1.0;
	scalar radius_y = std::max( std::abs(min_y), std::abs(max_y) );
	if (radius_y == 0.0) radius_y = 1.0;
	scalar ratio = ((scalar)g_display_controller.getWindowHeight()) /
					((scalar)g_display_controller.getWindowWidth());
				
//	std::cout << "radius_x, radius_y: " << radius_x << " " << radius_y << std::endl;
	
	std::cout << "g_scene_data.recenterScale " <<   g_scene_data.recenterScale << std::endl;
	
	centerCamera( cx, cy, g_scene_data.recenterScale*std::max(ratio*radius_x, radius_y) );
}

void centerCamera( scalar cx, scalar cy, scalar scale )
{
	g_display_controller.setCenterX( cx );
	g_display_controller.setCenterY( cy );
	g_display_controller.setScaleFactor( scale );
}

void keyboard( unsigned char key, int x, int y )
{
	g_display_controller.keyboard( key, x, y );
	g_scene_data.g_bezier_renderer->keyboard( key, x, y );
	
	// TODO: Put a controller in polybscenerenderer
	
	// Quit the program
	if( key == 'q' )
	{
		exit(0);
	}
	
	// Re-center the scene with minimal boundary
	else if( key == 'c' )
	{
		centerCamera();
		g_display_controller.reshape( g_display_controller.getWindowWidth(),
									  g_display_controller.getWindowHeight() );
		glutPostRedisplay();
	}
	
	// Re-center the scene with origin at center
	else if( key == 'C' )
	{
		centerCamera(0.0, 0.0);
		g_display_controller.reshape( g_display_controller.getWindowWidth(),
									  g_display_controller.getWindowHeight() );
		glutPostRedisplay();
	}
	
	else if( key == ']')
	{
		*(g_scene_data.gridMaxX) += 1;
		*(g_scene_data.gridMaxY) += 1;
	}
	else if( key == '[')
	{
		*(g_scene_data.gridMaxX) -= 1;
		*(g_scene_data.gridMaxY) -= 1;
		
		if ( *(g_scene_data.gridMaxX) < 0) *(g_scene_data.gridMaxX) = 0;
		if ( *(g_scene_data.gridMaxY) < 0) *(g_scene_data.gridMaxY) = 0;
		
	}
	
	assert( renderingutils::checkGLErrors() );
}

void special( int key, int x, int y )
{
	g_display_controller.special( key, x, y );
	
	assert( renderingutils::checkGLErrors() );
}

void mouse( int button, int state, int x, int y )
{
	g_display_controller.mouse( button, state, x, y );
	
	assert( renderingutils::checkGLErrors() );
}

void motion( int x, int y )
{
	g_display_controller.motion( x, y );
	
	assert( renderingutils::checkGLErrors() );
}

void idle()
{
	glutPostRedisplay();
	
	assert( renderingutils::checkGLErrors() );
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// initializeOpenGLandGLUT and runOpenGL methods
void initializeOpenGLandGLUT ( int argc, char** argv ) {

	// Set camera at center of the scene
	centerCamera( 0.0, 0.0, 1.0 );

	// Initialize GLUT
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );					// Buffer and color
	glutInitWindowSize( g_display_controller.getWindowWidth(),
						g_display_controller.getWindowHeight() );
	glutCreateWindow("Bezier Curve Display");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(idle);
  
	// Initialize OpenGL
	reshape(g_display_controller.getWindowWidth(),g_display_controller.getWindowHeight());
	glClearColor(	g_scene_data.g_bgColor.r,
					g_scene_data.g_bgColor.g,
					g_scene_data.g_bgColor.b,
					1.0);
  
	assert( renderingutils::checkGLErrors() );

		
//    glEnable (GL_LIGHTING);			//Allow lighting for the scene.
//	glEnable (GL_NORMALIZE);		//Recalculate the matrix-transformed normal vectors.
//	glEnable (GL_DEPTH_TEST);		//Allow depth test.
//	glEnable (GL_COLOR_MATERIAL);	//Allow the material to be colored.
//	glEnable (GL_BLEND);			//Enable transparent material.
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	
//	glEnable(GL_LIGHT2);
//	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight3);
//	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight3);
//	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight3);
//	glLightfv(GL_LIGHT2, GL_POSITION, pos3);
	
}

void setPolyBezierSceneRenderer( PolyBezierSceneRenderer * sceneRenderer )
{
	g_scene_data.g_bezier_renderer = sceneRenderer;
}

void runOpenGL ()
{
	glutMainLoop ();
}


void initializeGrid( int & gridMaxX, int & gridMaxY, scalar & gridSize)
{
	g_scene_data.gridMaxX = &gridMaxX;
	g_scene_data.gridMaxY = &gridMaxY;
	g_scene_data.gridSize = &gridSize;
}

void drawGrid(int maxX, int maxY, double gridSize) {

	glPushMatrix();

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBegin(GL_LINES);
	
	// Grid
	{
		glColor4f( 0.7, 0.7, 0.7, 0.5 );
		for ( double x = - maxX; x <= maxX; x += gridSize ) {
			glVertex2d( x, maxY );
			glVertex2d( x, -maxY );
		}
		
		for ( double y = - maxY; y <= maxY; y += gridSize ) {
			glVertex2d( -maxX, y );
			glVertex2d( maxX, y );
		}
	}
	
	glEnd();
	
	glLineWidth( 1.0 );
	
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBegin(GL_LINES);
	
	// Axes
	{
		glColor4f( 0.0, 0.0, 0.0, 0.5 );
		
		glVertex2d( 0, maxY );
		glVertex2d( 0, -maxY );

		glVertex2d( -maxX, 0 );
		glVertex2d( maxX, 0 );
	}
	
	glEnd();
	glLineWidth( 1.0 );
	
	glPopMatrix();
}