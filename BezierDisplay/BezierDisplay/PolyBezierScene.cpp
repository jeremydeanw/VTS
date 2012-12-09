//
//  PolyBezierScene.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/15/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "PolyBezierScene.h"

PolyBezierScene::PolyBezierScene()
: m_curves(0)
, m_curvesHistory(0)
, m_LoD(1)
, m_evaluatorInitialized(false)
, m_resample(true)
{}

PolyBezierScene::PolyBezierScene( const int & n )
: m_curves(n)
, m_curvesHistory(0)
, m_LoD(1)
, m_evaluatorInitialized(false)
, m_resample(true)
{}

PolyBezierScene::PolyBezierScene( const int & n, const int & rate )
: m_curves(n)
, m_curvesHistory(0)
, m_LoD(rate)
, m_evaluatorInitialized(false)
, m_resample(true)
{}

// All the constructors should prepare the Bezier Evaluators according to rate

void PolyBezierScene::mergeDegreeSet( const VectorX1i & degs )
{
	for (int i = 0; i < degs.rows(); ++i) {
		m_degreeSet.insert(degs[i]);
	}
}

void PolyBezierScene::addPolyCurve( const VectorX1i & degs, const VectorX2s & points, int & currentGlobalID)
{
	PolyBezierCurve newPolyCurve( degs, points, currentGlobalID );
	
	mergeDegreeSet( degs );
	
	m_curves.push_back( newPolyCurve );
	currentGlobalID++;
}

void PolyBezierScene::addPolyCurve( PolyBezierCurve & newCurve, int & currentGlobalID )
{
	mergeDegreeSet( newCurve.getDegs() );

	newCurve.setID( currentGlobalID );
	m_curves.push_back( newCurve );
	currentGlobalID++;
}

//void PolyBezierScene::setPolyCurve( const int & i, const VectorX1i & degs, const VectorX2s & points )
//{
//	assert(i >= 0 && i < getNumCurves() );
//	
//	mergeDegreeSet( degs );
//
//	m_curves[i] = PolyBezierCurve ( m_globalID, degs, points );
//	m_globalID++;
//	
//	// TODO: SORT m_curves!
//}
//
//void PolyBezierScene::setPolyCurve( const int & i, const PolyBezierCurve & newCurve )
//{
//	assert(i >= 0 && i < getNumCurves() );
//	
//	mergeDegreeSet( newCurve.getDegs() );
//	
//	m_curves[i] = newCurve;
//}

//const curvedef::VectorPolyCurve::iterator & PolyBezierScene::getCurveIterator() const
//{
//	return m_curvesIt;
//}
//
//curvedef::VectorPolyCurve::iterator & PolyBezierScene::getCurveIterator()
//{
//	return m_curvesIt;
//}

void PolyBezierScene::initializeEvaluators()
{
	assert( !m_degreeSet.empty() );
	
	m_evaluators.clear();
	
	for (m_degreeSetIt = m_degreeSet.begin(); m_degreeSetIt != m_degreeSet.end(); ++m_degreeSetIt)
	{
		BezierEvaluator newE ( *m_degreeSetIt );
		m_evaluators[*m_degreeSetIt] = newE;
	}
	
	m_evaluatorInitialized = true;
	
}

void PolyBezierScene::computeEvaluatorCoefficients()
{
	if ( !m_resample ) return;

	assert( m_LoD >= 0 );
	assert( m_evaluatorInitialized );
	
	curvedef::MapEvaluators::iterator evaluatorsIt;
	
	for (evaluatorsIt = m_evaluators.begin(); evaluatorsIt != m_evaluators.end(); ++evaluatorsIt)
	{
		(*evaluatorsIt).second.generateBernsteinMap(m_LoD);
	}
	
}


void PolyBezierScene::evalPolyCurveSamples()
{
	if (m_curves.size() == 0) return;
	
	curvedef::VectorPolyCurve::iterator curvesIt;
	
	for (curvesIt = m_curves.begin(); curvesIt != m_curves.end(); ++curvesIt)
	{
		PolyBezierCurve & curve = (*curvesIt);
		curve.generateSamplePoints(m_evaluators, m_LoD);
	}
}
	
// TODO
void PolyBezierScene::evalPolyCurveHistorySamples()
{
	
}


void PolyBezierScene::setLoD( const int & rate )
{
	if ( rate != m_LoD)
	{
		m_LoD = rate;
		m_resample = true;

//		computeEvaluatorCoefficients();
//		evalPolyCurveSamples();
	}
	else
	{
		m_resample = false;
	}
	
}

const int & PolyBezierScene::getLoD() const
{
	return m_LoD;
}

curvedef::VectorPolyCurve & PolyBezierScene::getCurves()
{
	return m_curves;
}

const curvedef::VectorPolyCurve & PolyBezierScene::getCurves() const
{
	return m_curves;
}

curvedef::VectorPolyCurve::iterator PolyBezierScene::getCurvesBegin()
{
	return m_curves.begin();
}

curvedef::VectorPolyCurve::iterator PolyBezierScene::getCurvesEnd()
{
	return m_curves.end();
}


int PolyBezierScene::getNumCurves() const
{
	return m_curves.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Curve connection methods
//
////////////////////////////////////////////////////////////////////////////////////////////////////

/*

1. Find the curve to connect
2. 
*/

void connectPolycurveThree(  );

PolyBezierCurve & findCurveGivenID( const int & ID );

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Debugging methods
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void PolyBezierScene::printDegreeSet() const
{
	
	std::cout << "m_degreeSet\n" << std::endl;
	curvedef::SetDegree::iterator m_degreeSetIt;
	for (m_degreeSetIt = m_degreeSet.begin(); m_degreeSetIt != m_degreeSet.end(); ++m_degreeSetIt)
	{
		std::cout << *m_degreeSetIt << std::endl;
	}
}
