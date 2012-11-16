//
//  OpenGLDisplayController.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/9/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "OpenGLDisplayController.h"

OpenGLDisplayController::OpenGLDisplayController( int width, int height )
: m_window_width( width )
, m_window_height( height )
, m_scale_factor( 1.0 )
, m_center_x( 0.0 )
, m_center_y( 0.0 )
, m_left_drag( false )
, m_right_drag( false )
, m_last_x( 0 )
, m_last_y( 0 )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Controllers and UI manipulation methods

void OpenGLDisplayController::reshape( int w, int h ) 
{
  assert( renderingutils::checkGLErrors() );
  
  // Record the new width and height
  m_window_width = w;
  m_window_height = h;
  
  // Reset the coordinate system before modifying
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  // Set the coordinate system to achieve the desired zoom level, center
  double ratio = (double)h / (double)w;
  gluOrtho2D( m_center_x - m_scale_factor / ratio,
			  m_center_x + m_scale_factor / ratio,
		  	  m_center_y - m_scale_factor,
			  m_center_y + m_scale_factor);
			 
  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);
  
  // Render the scene
  glutPostRedisplay();
  
  assert( renderingutils::checkGLErrors() );
}

void OpenGLDisplayController::keyboard( unsigned char key, int x, int y )
{
	if( key == '-')
	{
		m_scale_factor += 0.1;
		reshape( m_window_width, m_window_height );
	}
	else if( key == '+' )
	{
		m_scale_factor = std::max(0.1,m_scale_factor-0.1);
		reshape( m_window_width, m_window_height );
	}
}

void OpenGLDisplayController::special( int key, int x, int y )
{
	if( GLUT_KEY_UP == key ) 
	{
		m_center_y += 0.1;
		reshape( m_window_width, m_window_height );
	}
	else if( GLUT_KEY_DOWN == key ) 
	{
		m_center_y -= 0.1;
		reshape( m_window_width, m_window_height );
	}
	else if( GLUT_KEY_LEFT == key ) 
	{
		m_center_x -= 0.1;
		reshape( m_window_width, m_window_height );
			}
	else if( GLUT_KEY_RIGHT == key ) 
	{
		m_center_x += 0.1;
		reshape( m_window_width, m_window_height );
	}
}

void OpenGLDisplayController::mouse( int button, int state, int x, int y )
{
	if( !m_right_drag && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		m_left_drag = true;
		m_last_x = x;
		m_last_y = y;
	}
	if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
	{
		m_left_drag = false;
	}

	if( !m_left_drag && button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
	{
		m_right_drag = true;
		m_last_x = x;
		m_last_y = y;
	}
	if( button == GLUT_RIGHT_BUTTON && state == GLUT_UP )
	{
		m_right_drag = false;
	}
}

void OpenGLDisplayController::translateView( double dx, double dy )
{
	double percent_x = dx / ((double)m_window_width);
	double percent_y = dy / ((double)m_window_height);
	double translate_x = percent_x * 2.0 * m_scale_factor *
						((double)m_window_width) / ((double)m_window_height);
	double translate_y = percent_y * 2.0 * m_scale_factor;
	m_center_x -= translate_x;
	m_center_y += translate_y;
	reshape( m_window_width, m_window_height );
}

void OpenGLDisplayController::zoomView( double dx, double dy )
{
	double percent_x = dx / ((double)m_window_width);
	double percent_y = dy / ((double)m_window_height);

	double scale;
	if( std::fabs(percent_x) > std::fabs(percent_y) )
		scale = -percent_x;
	else scale = percent_y;

	m_scale_factor += 2.0*scale;

	reshape( m_window_width, m_window_height );
}

void OpenGLDisplayController::motion( int x, int y ) 
{
	if( m_left_drag ) 
	{
		double dx = x - m_last_x;
		double dy = y - m_last_y;
		m_last_x = x;
		m_last_y = y;
		translateView( dx, dy );
	}
	if( m_right_drag ) 
	{
		double dx = x - m_last_x;
		double dy = y - m_last_y;
		m_last_x = x;
		m_last_y = y;
		zoomView( dx, dy );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Little helper methods

int OpenGLDisplayController::getWindowWidth() const
{
  return m_window_width;
}

int OpenGLDisplayController::getWindowHeight() const
{
  return m_window_height;
}

int OpenGLDisplayController::getWorldWidth() const
{
	double ratio = m_window_height / m_window_width;
	return 2 * m_scale_factor / ratio;
}

int OpenGLDisplayController::getWorldHeight() const
{
	return 2 * m_scale_factor;
}

double OpenGLDisplayController::getCenterX() const
{
	return m_center_x;
}

double OpenGLDisplayController::getCenterY() const
{
	return m_center_y;
}


void OpenGLDisplayController::setCenterX( double x )
{
	m_center_x = x;
}

void OpenGLDisplayController::setCenterY( double y )
{
	m_center_y = y;
}

void OpenGLDisplayController::setScaleFactor( double scale )
{
	m_scale_factor = scale;
}

double OpenGLDisplayController::getScaleFactor() const
{
	return m_scale_factor;
}
