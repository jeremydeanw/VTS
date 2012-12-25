//
//  OpenGLSceneData.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 12/8/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef __BezierDisplay__OpenGLSceneData__
#define __BezierDisplay__OpenGLSceneData__

#include <iostream>
#include "MathDef.h"
#include "RenderingUtilities.h"

// Forward declaration
class PolyBezierSceneRenderer;

// SceneObjectData
typedef struct SceneObjectData
{
	SceneObjectData();

	// Pointer to the curve scene renderer
	PolyBezierSceneRenderer * g_bezier_renderer;

	// The size of grid table to be displayed, and the size of one grid
	int * g_gridMaxX;
	int * g_gridMaxY;
	scalar * g_gridSize;
	bool m_drawGrid;
	
} SceneObjectData;

// SceneControllerData
typedef struct SceneControllerData
{
	SceneControllerData();
	SceneControllerData( const scalar & rescale );
	
	// True if the user is dragging the display left
	bool m_left_drag;
	// True if the user is dragging the display right
	bool m_right_drag;
	// True if SHIFT key is being held down during mouse callback
	bool m_activeshift;

	// Last position of the cursor in a drag, x coord
	int m_last_x;
	// Last position of the cursor in a drag, y coord
	int m_last_y;
	
	// Additional factor after zoom scaling
	scalar m_recenterScale;
	
} SceneControllerData;

// SceneMiscData
typedef struct SceneMiscData
{
	SceneMiscData();
	SceneMiscData( const renderingutils::Color & bg );
	
	// Color
	renderingutils::Color m_bgColor;
	
} SceneMiscData;

#endif /* defined(__BezierDisplay__OpenGLSceneData__) */
