//
//  RenderingUtilities.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/9/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef __BezierDisplay__RenderingUtilities__
#define __BezierDisplay__RenderingUtilities__

#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <GL/glut.h>
#endif

#include <list>
#include <iostream>
#include <cstdio>
#include <cassert>

#include "StringUtilities.h"

//#include "MathDefs.h"

namespace renderingutils
{

bool checkGLErrors();
  
class Color
{
public:

	Color();

	Color( double r, double g, double b );

	double r;
	double g;
	double b;
};

//struct Viewport
//{
//public:
//	Viewport() : cx(0.0), cy(0.0), size(0.) {}
//	double cx;
//	double cy;
//	double rx;
//	double ry;
//	double size;
//};

}


#endif /* defined(__BezierDisplay__RenderingUtilities__) */
