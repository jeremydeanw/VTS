//
//  PolyBezierSceneRenderer.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/15/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "PolyBezierSceneRenderer.h"

PolyBezierSceneRenderer::PolyBezierSceneRenderer( const PolyBezierScene & scene )
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

void PolyBezierSceneRenderer::renderScene() const
{
	const curvedef::VectorPolyCurve & curves = m_curveScene.getCurves();
	
	// const_iterator used to iterate through const STL container
	curvedef::VectorPolyCurve::const_iterator curvesIt;
	
	for (curvesIt = curves.begin(); curvesIt != curves.end(); ++curvesIt)
	{
		const VectorX2s & samplePoints = curvesIt->getSamplePoints();
		renderCurve( samplePoints );
		
		// TODO: Render curve history
		
		const VectorX2s & controlPoints = curvesIt->getControlPoints();
		renderCurveHull( controlPoints );
	}
	
}

void PolyBezierSceneRenderer::renderCurve( const VectorX2s & samplePoints ) const
{
	glColor4d( m_curveColor.r, m_curveColor.g, m_curveColor.b, m_curveColor.a);
	glLineWidth( m_curveThickness );
	
	glPushMatrix();
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
	glBegin(GL_QUADS);
	{
		for (int i = 0; i < controlPoints.rows(); ++i) {
			glVertex2d( controlPoints(i, 0) + 0.1, controlPoints(i, 1) + 0.1 );
			glVertex2d( controlPoints(i, 0) - 0.1, controlPoints(i, 1) + 0.1 );
			glVertex2d( controlPoints(i, 0) - 0.1, controlPoints(i, 1) - 0.1 );
			glVertex2d( controlPoints(i, 0) + 0.1, controlPoints(i, 1) - 0.1 );
		}
	}
	glEnd();
	
	glPopMatrix();
	
	glColor4d(0, 0, 0, 1.0);
	glLineWidth( 1.0 );
}