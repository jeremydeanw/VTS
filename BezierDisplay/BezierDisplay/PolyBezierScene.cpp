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

void PolyBezierScene::mergeDegreeSet( const VectorX1s & degs )
{
	for (int i = 0; i < degs.rows(); i++) {
		m_degreeSet.insert(degs[i]);
	}
}

void PolyBezierScene::addPolyCurve( const VectorX1s & degs, const VectorX2s & points )
{
	PolyBezierCurve newPolyCurve( degs, points );
	
	mergeDegreeSet( degs );
	
	m_curves.push_back( newPolyCurve );
}

void PolyBezierScene::addPolyCurve( const PolyBezierCurve & newCurve )
{
	mergeDegreeSet( newCurve.getDegs() );

	m_curves.push_back( newCurve );
}

void PolyBezierScene::setPolyCurve( const int & i, const VectorX1s & degs, const VectorX2s & points )
{
	assert(i >= 0 && i < getNumCurves() );
	
	mergeDegreeSet( degs );

	m_curves[i] = PolyBezierCurve ( degs, points );
}

void PolyBezierScene::setPolyCurve( const int & i, const PolyBezierCurve & newCurve )
{
	assert(i >= 0 && i < getNumCurves() );
	
	mergeDegreeSet( newCurve.getDegs() );
	
	m_curves[i] = newCurve;
}

void PolyBezierScene::initializeEvaluators()
{
	assert( m_degreeSet.empty() );
	
	for (m_degreeSetIt = m_degreeSet.begin(); m_degreeSetIt != m_degreeSet.end(); m_degreeSetIt++)
	{
		m_evaluators.push_back( BezierEvaluator( *m_degreeSetIt ) );
	}
	
	m_evaluatorInitialized = true;
	
}

void PolyBezierScene::computeEvaluatorCoefficients()
{
	assert( m_LoD >= 0 );
	assert( m_evaluatorInitialized );
	
	for (m_evaluatorsIt = m_evaluators.begin(); m_evaluatorsIt != m_evaluators.end(); m_evaluatorsIt++)
	{
		(*m_evaluatorsIt).generateBernsteinMap(m_LoD);
	}
	
}

// Iterate through all curves and sample them
void PolyBezierScene::evalPolyCurveSamples()
{
	if (m_curves.size() == 0) return;
	
	for (m_curvesIt = m_curves.begin(); m_curvesIt != m_curves.end(); m_curvesIt++)
	{
		PolyBezierCurve & curve = (*m_curvesIt);
		
		curve.pointsBegin()
		
		// Make sure m_curvesIt data gets modified as well.
		
	}
	

	
//	for (int i = 0; i < degs.rows(); i++) {
//		myPolyCurve.gotoSegment(i);
//		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve.getSegmentIndices() << std::endl;
//	}

//	int i;
//	
//	for (i = degs.rows()-1; i >= 0; i--) {
//		myPolyCurve.gotoSegment(i);
//		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve.getSegmentIndices() << std::endl;
//	}
//	
//	myPolyCurve.gotoSegment(2); i =2;
//		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve.getSegmentIndices() << std::endl;
//		
//	myPolyCurve.advanceSegment(); i++;
//		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve.getSegmentIndices() << std::endl;
//	
//	myPolyCurve.resetToFirstSegment(); i=0;
//		std::cout << "Segment " << i << " point indices: \n" << myPolyCurve.getSegmentIndices() << std::endl;
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
		
		computeEvaluatorCoefficients();
	}
	else
	{
		m_resample = false;
	}
	
}

int PolyBezierScene::getNumCurves() const
{
	return m_curves.size();
}
