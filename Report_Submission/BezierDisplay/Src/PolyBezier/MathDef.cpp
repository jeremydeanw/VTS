//
//  MathDef.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "MathDef.h"

void mathdef::resize( VectorX2s & v, int size )
{
	v.resize( size, 2 );
}

void mathdef::resize( VectorX1s & v, int size )
{
	v.resize( size, 1 );
}

void mathdef::resize( VectorX1i & v, int size )
{
	v.resize( size, 1 );
}

void mathdef::setZero( VectorX2s & v )
{
	v = VectorX2s::Zero(v.rows(), 2);
}

void mathdef::setZero( VectorX1s & v )
{
	v = VectorX1s::Zero(v.rows(), 1);
}

void mathdef::setZero( VectorX1i & v )
{
	v = VectorX1i::Zero(v.rows(), 1);
}

