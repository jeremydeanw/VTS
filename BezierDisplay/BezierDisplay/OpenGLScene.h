/*
 *  OpenGLScene.h
 *
 *  Created by Papoj Thamjaroenporn on 10/21/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SCENE_H
#define SCENE_H

//#include "Imported.h"
#include "RenderingUtilities.h"

// SceneData
typedef struct SceneData
{
	// TODO: Put the TwoDScene

	const int * gridSizeX;
	const int * gridSizeY;
} SceneData;

// OpenGL rendering and UI functions
void reshape (int w, int h);
void setOrthographicProjection();
void drawHUD();
void display();
void centerCamera();
void keyboard( unsigned char key, int x, int y );
void special( int key, int x, int y );
void mouse( int button, int state, int x, int y );
void motion( int x, int y );
void idle();

// initializeOpenGLandGLUT
void initializeOpenGLandGLUT( int argc, char** argv );
void runOpenGL();

//Draw the grid wireframe centered at origin
void initializeGrid( const int & gridSizeX, const int & gridSizeY );
void drawGrid( int maxX, int maxY, double gridSize );

#endif