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
		friend std::ostream & operator<<( std::ostream &os, const Color & c)
		{
			return os << "(" << c.r << "," << c.g << "," << c.b << "," << c.a << ")";
		}
	
	public:

		Color();

		Color( double r, double g, double b, double a = 1.0);
		
		Color( const Color & c);

		double r;
		double g;
		double b;
		double a;
	};

}


#endif /* defined(__BezierDisplay__RenderingUtilities__) */
