/*
 *  BezierDS.cpp
 *  BezierCurve
 *
 *  Created by Papoj Thamjaroenporn on 11/18/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

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
	
//	std::cout << "n, r: " << n << " " << r << std::endl;
//	std::cout << "c: " << c << std::endl;
	
	return c;
	
}

scalar computeParameterT( const int & index , const int & rate)
{
//	std::cout << "rate" << rate << std::endl;
//	std::cout << "computeParamT(" << index << "): " << ((scalar) index) * (1.0 / (scalar) rate) << std::endl;
	return ((scalar) index) * (1.0 / (scalar) rate);
}