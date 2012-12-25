//
//  BezierUtil.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "BezierUtils.h"
#include <iostream>


int computeNchooseR (const int & n, const int & r)
{
	int c;
	
	if(r < 0 || n <= 0 || r > n) c = 0;
	else if (r > (n / 2)) c = computeNchooseR(n, n - r);	// Prevent one-sided overflow
	else
	{
		c = 1;
		int i;
		
		for (i = 0; i <= r - 1; ++i) c *= (n - i);
		for (i = 1; i <= r; ++i) c /= i;
	}

	return c;
	
}

scalar computeParameterT( const int & index , const int & rate)
{
	return ((scalar) index) * (1.0 / (scalar) rate);
}