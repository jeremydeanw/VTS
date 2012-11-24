//
//  RenderingUtilities.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/9/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "RenderingUtilities.h"

namespace renderingutils
{

bool checkGLErrors()
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR) 
	{
		errString = gluErrorString(errCode);
		std::cout << outputmod::startred << "OpenGL Error:" << outputmod::endred << std::flush;
		fprintf(stderr, " %s\n", errString);
		return false;
	}
	return true;
}
  
Color::Color()
: r(0.0), g(0.0), b(0.0), a(1.0)
{}

Color::Color( double r, double g, double b, double a )
: r(r), g(g), b(b), a(a)
{
	assert( r >= 0.0 ); assert( r <= 1.0 );
	assert( g >= 0.0 ); assert( g <= 1.0 );
	assert( b >= 0.0 ); assert( b <= 1.0 );
	assert( a >= 0.0 ); assert( a <= 1.0 );
}

}
