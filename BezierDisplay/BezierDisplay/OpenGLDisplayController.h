//
//  OpenGLDisplayController.h
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/9/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef __BezierDisplay__OpenGLDisplayController__
#define __BezierDisplay__OpenGLDisplayController__

#include <cmath>
#include <iostream>
#include <cassert>

#include "RenderingUtilities.h"

class OpenGLDisplayController
{
public:
	OpenGLDisplayController( int width, int height );
	
	void reshape( int w, int h );
	void keyboard( unsigned char key, int x, int y );
	void special( int key, int x, int y );
	void mouse( int button, int state, int x, int y );
	void translateView( double dx, double dy );
	void zoomView( double dx, double dy );
	void motion( int x, int y );

	int getWindowWidth() const;  
	int getWindowHeight() const;

	int getWorldWidth() const;
	int getWorldHeight() const;

	double getCenterX() const;
	double getCenterY() const;
	void setCenterX( double x );
	void setCenterY( double y );

	void setScaleFactor( double scale );
	double getScaleFactor() const;
	
	
private:
	
	// Width of the window in pixels
	int m_window_width;
	// Height of the window in pixels
	int m_window_height;
	
	// Factor to 'zoom' in or out by
	// 1.0 is default. The smaller number means zoom-in; bigger out.
	double m_scale_factor;

	// Center of the display, x coord
	double m_center_x;
	// Center of the display, y coord
	double m_center_y;

	// True if the user is dragging the display left
	bool m_left_drag;
	// True if the user is dragging the display right
	bool m_right_drag;

	// Last position of the cursor in a drag, x coord
	int m_last_x;
	// Last position of the cursor in a drag, y coord
	int m_last_y;

};

#endif /* defined(__BezierDisplay__OpenGLDisplayController__) */
