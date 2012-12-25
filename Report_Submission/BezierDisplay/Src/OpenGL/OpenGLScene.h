//
//  OpenGLScene.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/9/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef SCENE_H
#define SCENE_H

//#include "Imported.h"
#include "MathDef.h"
#include "RenderingUtilities.h"
#include "PolyBezierSceneRenderer.h"
#include "OpenGLSceneData.h"

// OpenGL rendering and UI functions
void reshape (int w, int h);
void setOrthographicProjection();
void drawHUD();

void display();

void findSceneBoundary(scalar & min_x, scalar & min_y, scalar & max_x, scalar & max_y);
void centerCamera();
void centerCamera( scalar cx, scalar cy );
void centerCamera( scalar cx, scalar cy, scalar scale );

void keyboard( unsigned char key, int x, int y );
void special( int key, int x, int y );
void mouse( int button, int state, int x, int y );
void motion( int x, int y );
void idle();

// initializeOpenGLandGLUT
void initializeOpenGLandGLUT( int argc, char** argv );
void setPolyBezierSceneRenderer( PolyBezierSceneRenderer * sceneRenderer );
void runOpenGL();

//Draw the grid wireframe centered at origin
void initializeGrid( int & gridMaxX, int & gridMaxY, scalar & gridSize);
void drawGrid( int maxX, int maxY, double gridSize );

#endif