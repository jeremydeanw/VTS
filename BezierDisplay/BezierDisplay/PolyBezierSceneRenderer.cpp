//
//  PolyBezierSceneRenderer.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/15/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "PolyBezierSceneRenderer.h"

PolyBezierSceneRenderer::PolyBezierSceneRenderer( PolyBezierScene & scene )
: m_curveScene( scene )
, m_curveColor( renderingutils::Color(0,0,0,1) )
, m_curveHistoryColor( renderingutils::Color(0,0,0,1) )
, m_curveHullColor( renderingutils::Color(0,0,0,1) )
, m_curveThickness(1.0)
, m_curveHistoryThickness(1.0)
, m_curveHullThickness(1.0)
, m_controlPointRadius(1.5)
, m_renderCurve( true )
, m_renderCurveHistory( true )
, m_renderCurveHull( true )
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Helper methods
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void PolyBezierSceneRenderer::setCurveColor( double r, double g, double b, double a )
{
	m_curveColor = renderingutils::Color(r,g,b,a);
}

void PolyBezierSceneRenderer::setCurveColor( const renderingutils::Color & color )
{
	m_curveColor = color;
}

void PolyBezierSceneRenderer::setCurveHistoryColor( double r, double g, double b, double a )
{
	m_curveHistoryColor = renderingutils::Color(r,g,b,a);
}


void PolyBezierSceneRenderer::setCurveHistoryColor( const renderingutils::Color & color )
{
	m_curveHistoryColor = color;
}

void PolyBezierSceneRenderer::setCurveHullColor( double r, double g, double b, double a )
{
	m_curveHullColor = renderingutils::Color(r,g,b,a);
}

void PolyBezierSceneRenderer::setCurveHullColor( const renderingutils::Color & color )
{
	m_curveHullColor = color;
}

void PolyBezierSceneRenderer::setCurveThickness( double t )
{
	m_curveThickness = t;
}

void PolyBezierSceneRenderer::setCurveHistoryThickness( double t )
{
	m_curveHistoryThickness = t;
}

void PolyBezierSceneRenderer::setCurveHullThickness( double t, double r )
{
	m_curveHullThickness = t;
	m_controlPointRadius = r;
}

void PolyBezierSceneRenderer::setRenderCurve( bool r )
{
	m_renderCurve = r;
}

void PolyBezierSceneRenderer::setRenderCurveHistory( bool r )
{
	m_renderCurveHistory = r;
}

void PolyBezierSceneRenderer::setRenderCurveHull( bool r )
{
	m_renderCurveHull = r;
}

const PolyBezierScene & PolyBezierSceneRenderer::getScene() const
{	
	return m_curveScene;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Controllers methods
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void PolyBezierSceneRenderer::keyboard( unsigned char key, int x, int y )
{
	// Toggle Bezier curves on or off
	if( key == 'b' || key == 'B')
	{
		m_renderCurve = !m_renderCurve;
	}
	
	// Toggle hull on or off
	else if( key == 'h' || key == 'H' )
	{
		m_renderCurveHull = !m_renderCurveHull;
	}
	
	
	if( key == 'A')
	{
		m_curveScene.connectPolycurveThree(0,1,true, true, 0.5, 0.5);
		m_curveScene.evalPolyCurveSamples();
		glutPostRedisplay();
	}
	
}

void PolyBezierSceneRenderer::processSelection( int xPos, int yPos, SceneControllerData & scd)
{
	// TODO: Process this
}

void PolyBezierSceneRenderer::mouse( int button, int state, int x, int y, SceneControllerData & scd )
{
	processSelection(x, y, scd);
}


void PolyBezierSceneRenderer::motion( int x, int y, SceneControllerData & scd)
{
//		bool left_drag, bool right_drag,
//		int & last_x, int & last_y)

// TODO: Process this

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Scene rendering methods
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void PolyBezierSceneRenderer::renderScene() const
{
	const curvedef::VectorPolyCurve & curves = m_curveScene.getCurves();
	
	// const_iterator used to iterate through const STL container
	curvedef::VectorPolyCurve::const_iterator curvesIt;
	
	if (m_renderCurve)
	{
		for (curvesIt = curves.begin(); curvesIt != curves.end(); ++curvesIt)
		{
			const VectorX2s & samplePoints = curvesIt->getSamplePoints();
			renderCurve( samplePoints );
		}
	}
	
	// TODO: Render curve history
	
	if (m_renderCurveHull)
	{
		for (curvesIt = curves.begin(); curvesIt != curves.end(); ++curvesIt)
		{
			const VectorX2s & controlPoints = curvesIt->getControlPoints();
			renderCurveHull( controlPoints );
		}
	}
	
//	for (curvesIt = curves.begin(); curvesIt != curves.end(); ++curvesIt)
//	{
//		if (m_renderCurve)
//		{
//			const VectorX2s & samplePoints = curvesIt->getSamplePoints();
//			renderCurve( samplePoints );
//		}
//		
//		// TODO: Render curve history
//		
//		if (m_renderCurveHull)
//		{
//			const VectorX2s & controlPoints = curvesIt->getControlPoints();
//			renderCurveHull( controlPoints );
//		}
//	}
	
}

void PolyBezierSceneRenderer::renderCurve( const VectorX2s & samplePoints ) const
{
	glColor4d( m_curveColor.r, m_curveColor.g, m_curveColor.b, m_curveColor.a);
	glLineWidth( m_curveThickness );
	
	glPushMatrix();
	
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBegin(GL_LINE_STRIP);
	{
		for (int i = 0; i < samplePoints.rows(); ++i) {
			glVertex2d( samplePoints(i, 0), samplePoints(i, 1) );
		}
	}
	glEnd();
	glPopMatrix();
	
	glColor4d(0, 0, 0, 1.0);
	glLineWidth( 1.0 );
}

// TODO: Do THIS
void PolyBezierSceneRenderer::renderCurveHistory() const
{

}

void PolyBezierSceneRenderer::renderCurveHull( const VectorX2s & controlPoints ) const
{
	glColor4d( m_curveHullColor.r, m_curveHullColor.g, m_curveHullColor.b, m_curveHullColor.a);
	glLineWidth( m_curveHullThickness );
	
	glPushMatrix();
	
	// Hull lines
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glBegin(GL_LINE_STRIP);
	{
		for (int i = 0; i < controlPoints.rows(); ++i) {
			glVertex2d( controlPoints(i, 0), controlPoints(i, 1) );
		}
	}
	glEnd();
	
	// Control points (rendered in circles)
	// TODO: Use point sprites to make points zoom-independent
	// Currently just a quad
	
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	{
		for (int i = 0; i < controlPoints.rows(); ++i)
		{
			glVertex2d(controlPoints(i, 0), controlPoints(i, 1));
		}
	}
	glEnd();

	
	
//	glBegin(GL_QUADS);
//	{
//		for (int i = 0; i < controlPoints.rows(); ++i) {
//			glVertex2d( controlPoints(i, 0) + 0.1, controlPoints(i, 1) + 0.1 );
//			glVertex2d( controlPoints(i, 0) - 0.1, controlPoints(i, 1) + 0.1 );
//			glVertex2d( controlPoints(i, 0) - 0.1, controlPoints(i, 1) - 0.1 );
//			glVertex2d( controlPoints(i, 0) + 0.1, controlPoints(i, 1) - 0.1 );
//		}
//	}
//	glEnd();
//	
	glPopMatrix();
	
	glColor4d(0, 0, 0, 1.0);
	glLineWidth( 1.0 );
}


//void PolyBezierSceneRenderer::CheckForExtension(void)
//{
//	char *ext = (char*)glGetString( GL_EXTENSIONS );
//	/////////////////////////////////////////////////////////////////
//	//Looking for GL_ARB_point_parameters extension
//	/////////////////////////////////////////////////////////////////
//	if( strstr( ext, "GL_ARB_point_parameters" ) != NULL )
//	{
//		glPointParameterfARB  = (PFNGLPOINTPARAMETERFEXTPROC)wglGetProcAddress("glPointParameterfARB");
//		glPointParameterfvARB = (PFNGLPOINTPARAMETERFVEXTPROC)wglGetProcAddress("glPointParameterfvARB");
//		if( !glPointParameterfARB || !glPointParameterfvARB )
//		{
//			std::cout	<< outputmod::startred
//						<< "ARB Error: One or more GL_EXT_point_parameters functions were not found"
//						<< outputmod::endred << std::flush;
//			m_PointARBEnable=false;
//		}
//		m_PointARBEnable=true;
//	}
//}
