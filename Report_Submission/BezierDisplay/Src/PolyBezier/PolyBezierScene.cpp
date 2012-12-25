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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Initialization methods
//
////////////////////////////////////////////////////////////////////////////////////////////////////

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

void PolyBezierScene::initializeEvaluators()
{
	assert( !m_degreeSet.empty() );
	
	m_evaluators.clear();
	
	curvedef::SetDegree::iterator m_degreeSetIt;
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Helpers methods
//
////////////////////////////////////////////////////////////////////////////////////////////////////


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
	Connect two curves, assumed to have all segments of degree three.
	1. Identify the curve pair to be connected
	2. Construct a matrix with proper size
	3. Start to fill in the matrix 
*/

void PolyBezierScene::connectPolycurveThree( const int & id1, const int & id2,
											const bool & lastPointOrFirst1, const bool & lastPointOrFirst2,
											const scalar & weight1, const scalar & weight2 )
{

	curvedef::SetDegree::iterator m_degreeSetIt;
	for (m_degreeSetIt = m_degreeSet.begin(); m_degreeSetIt != m_degreeSet.end(); ++m_degreeSetIt)
	{
		if (*m_degreeSetIt != 3) {
			std::cout	<< outputmod::startred
				<< "PolyBezierScene Error:"
				<< " This connection only supports scene with degree 3 segments only\n"
				<< "Found degree " << *m_degreeSetIt
				<< outputmod::endred << std::flush;
			return;
		}
	}

	PolyBezierCurve & curve1 = m_curves[ findCurveIndexGivenID(id1) ];
	PolyBezierCurve & curve2 = m_curves[ findCurveIndexGivenID(id2) ];
	
	int numSegments1 = curve1.getNumSegments();
	int numSegments2 = curve2.getNumSegments();
	int solverSize1 = 3 * numSegments1 + 1;
	int solverSize2 = 3 * numSegments2 + 1;

	IndexConverter ic1 ( curve1.getNumControlPoints(), lastPointOrFirst1 );
	IndexConverter ic2 ( curve2.getNumControlPoints(), lastPointOrFirst2 );

	int totalSize = solverSize1 + solverSize2;

	MatrixXs A (totalSize, totalSize);
	MatrixXs b_coeff (totalSize, totalSize);
	MatrixXs b_ctrlPoints (totalSize, 2);		// 2 for x and y coordinates
	A = MatrixXs::Zero(totalSize, totalSize);
	b_coeff = MatrixXs::Zero(totalSize, totalSize);
	b_ctrlPoints.block(0, 0, solverSize1, 2) = curve1.getControlPoints();
	b_ctrlPoints.block(solverSize1, 0, solverSize2, 2) = curve2.getControlPoints();
	
	// Start filling in the matrix now!
	
	// current solver row
	int srow = 0;
	
	// Iterate through each curve segment and
	// fill in constraints based on the curvature of the first curve
	for(int i = 0; i < numSegments1; ++i)
	{
		// First set of constraints for segment i-th
		if (i == 0) {
			// First criterion
			A(srow, i) = 1;
			b_coeff(srow, i) = 1;
			++srow;
			
			// Second criterion
			A(srow, 3*i + 0) = 1;
			A(srow, 3*i + 1) = -2;
			A(srow, 3*i + 2) = 1;
			b_coeff(srow, 3*i + 0) = 1;
			b_coeff(srow, 3*i + 1) = -2;
			b_coeff(srow, 3*i + 2) = 1;
			++srow;
		} else if (i != 0) {
			// Only one criteria here
			// b_coeff part is simply zeros
			A(srow, 3*i - 1) = 1;
			A(srow, 3*i + 0) = -2;
			A(srow, 3*i + 1) = 1;
			++srow;
		}
		
		// Second set of constraints for segment i-th
		if (i != numSegments1 - 1) {
			// First criterion
			A(srow, 3*i + 1) = 1;
			A(srow, 3*i + 2) = -2;
			A(srow, 3*i + 4) = 2;
			A(srow, 3*i + 5) = -1;
			b_coeff(srow, 3*i + 1) = 1;
			b_coeff(srow, 3*i + 2) = -2;
			b_coeff(srow, 3*i + 4) = 2;
			b_coeff(srow, 3*i + 5) = -1;
			++srow;
			
			// Second criterion
			A(srow, 3*i + 0) = 1;
			A(srow, 3*i + 2) = -3;
			A(srow, 3*i + 3) = 4;
			A(srow, 3*i + 4) = -3;
			A(srow, 3*i + 6) = 1;
			b_coeff(srow, 3*i + 0) = 1;
			b_coeff(srow, 3*i + 2) = -3;
			b_coeff(srow, 3*i + 3) = 4;
			b_coeff(srow, 3*i + 4) = -3;
			b_coeff(srow, 3*i + 6) = 1;
			++srow;
		} else if (i == numSegments1 - 1) {
			int x = numSegments1 - 1;
			int y = numSegments2 - 1;
		
			// First criterion
			A(srow, 3*x + 1) = 1;
			A(srow, 3*y + 1 + solverSize1) = -1;		// Replace to ic1.index(3*y + 1) + solverSize1
			A(srow, 3*x + 2) = -2;
			A(srow, 3*y + 2 + solverSize1) = 2;
			A(srow, 3*x + 3) = 1;
			A(srow, 3*y + 3 + solverSize1) = -1;						
			b_coeff(srow, 3*x + 1) = 1;
			b_coeff(srow, 3*y + 1 + solverSize1) = -1;		// Replace to ic1.index(3*y + 1) + solverSize1
			b_coeff(srow, 3*x + 2) = -2;
			b_coeff(srow, 3*y + 2 + solverSize1) = 2;
			b_coeff(srow, 3*x + 3) = 1;
			b_coeff(srow, 3*y + 3 + solverSize1) = -1;
			++srow;
			
			// Second criterion
			A(srow, 3*x + 3) = 1;
			b_coeff(srow, 3*x + 3) = weight1;
			b_coeff(srow, 3*y + 3 + solverSize1) = weight2;
			++srow;
		}
	}
	
	// Iterate through each curve segment and
	// fill in constraints based on the curvature of the second curve.
	// Notice the index shift in matrix A; most of the columns now are related to the second curve
	// control points, so we need to shift all indices to the RIGHT HALF of the matrix.
	for(int i = 0; i < numSegments2; ++i)
	{
		// First set of constraints for segment i-th
		if (i == 0) {
			// First criterion
			A(srow, i + solverSize1) = 1;
			b_coeff(srow, i + solverSize1) = 1;
			++srow;
			
			// Second criterion
			A(srow, 3*i + 0 + solverSize1) = 1;
			A(srow, 3*i + 1 + solverSize1) = -2;
			A(srow, 3*i + 2 + solverSize1) = 1;
			b_coeff(srow, 3*i + 0 + solverSize1) = 1;
			b_coeff(srow, 3*i + 1 + solverSize1) = -2;
			b_coeff(srow, 3*i + 2 + solverSize1) = 1;
			++srow;
		} else if (i != 0) {
			// Only one criteria here
			// b_coeff part is simply zeros
			A(srow, 3*i - 1 + solverSize1) = 1;
			A(srow, 3*i + 0 + solverSize1) = -2;
			A(srow, 3*i + 1 + solverSize1) = 1;
			++srow;
		}
		
		// Second set of constraints for segment i-th
		if (i != numSegments2 - 1) {
			// First criterion
			A(srow, 3*i + 1 + solverSize1) = 1;
			A(srow, 3*i + 2 + solverSize1) = -2;
			A(srow, 3*i + 4 + solverSize1) = 2;
			A(srow, 3*i + 5 + solverSize1) = -1;
			b_coeff(srow, 3*i + 1 + solverSize1) = 1;
			b_coeff(srow, 3*i + 2 + solverSize1) = -2;
			b_coeff(srow, 3*i + 4 + solverSize1) = 2;
			b_coeff(srow, 3*i + 5 + solverSize1) = -1;
			++srow;
			
			// Second criterion
			A(srow, 3*i + 0 + solverSize1) = 1;
			A(srow, 3*i + 2 + solverSize1) = -3;
			A(srow, 3*i + 3 + solverSize1) = 4;
			A(srow, 3*i + 4 + solverSize1) = -3;
			A(srow, 3*i + 6 + solverSize1) = 1;
			b_coeff(srow, 3*i + 0 + solverSize1) = 1;
			b_coeff(srow, 3*i + 2 + solverSize1) = -3;
			b_coeff(srow, 3*i + 3 + solverSize1) = 4;
			b_coeff(srow, 3*i + 4 + solverSize1) = -3;
			b_coeff(srow, 3*i + 6 + solverSize1) = 1;
			++srow;
			
		} else if (i == numSegments2 - 1) {
			int x = numSegments1 - 1;
			int y = numSegments2 - 1;
		
			// First criterion (Smooth joining)
			A(srow, 3*x + 3) = 1;
			A(srow, 3*y + 3 + solverSize1) = 1;
			A(srow, 3*x + 2) = -1;
			A(srow, 3*y + 2 + solverSize1) = -1;
			++srow;
			
			// Second criterion (Joining two end points)
			A(srow, 3*y + 3 + solverSize1) = 1;
			b_coeff(srow, 3*x + 3) = weight1;
			b_coeff(srow, 3*y + 3 + solverSize1) = weight2;
			++srow;
		}
	}

	// Solve!
	VectorX2s X = A.colPivHouseholderQr().solve(b_coeff * b_ctrlPoints);
	
	// Set the two curve control points to the solver result
	curve1.getControlPoints() = X.block(0, 0, solverSize1, 2);
	curve2.getControlPoints() = X.block(solverSize1, 0, solverSize2, 2);
	
}

int PolyBezierScene::findCurveIndexGivenID( const int & ID )
{
	assert( m_curves.size() > 0 );

	// Perform binary search on m_id inside m_curves.
	int left = 0;
	int right = m_curves.size() - 1;
	int mid;
	
	while (right >= left) {
		mid = (left + right) / 2;
		
		if (m_curves[mid].getID() < ID)
			left = mid + 1;
		else if (m_curves[mid].getID() > ID)
			right = mid - 1;
		else
		{
			return mid;
		}
	}
	
	std::cout	<< outputmod::startred
				<< "PolyBezierScene Error: No curve with ID " << ID << "found"
				<< outputmod::endred << std::flush;
	return -1;
}


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
