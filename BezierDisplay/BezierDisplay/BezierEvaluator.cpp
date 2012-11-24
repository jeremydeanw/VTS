//
//  BezierEvaluator.cpp
//  BezierDisplay
//
//  Created by Papoj Thamjaroenporn on 11/14/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#include "BezierEvaluator.h"

BezierEvaluator::BezierEvaluator( )
: m_deg( 0 )
, m_rate( 0 )
, m_basesComputed(false)
{
	// TODO: Call initialize later??
}

BezierEvaluator::BezierEvaluator( curvedef::Degree deg )
: m_deg( deg )
, m_rate(0)
, m_basesComputed(false)
{
//	//mathdef::resize( m_bases, m_deg + 1 );
//	mathdef::resize( m_combinations, m_deg + 1 );
//
////	std::cout << "m_bases: " << m_bases << std::endl;
////	std::cout << "m_comb: " << m_combinations << std::endl;
//	
	initializeCombinations();
}

BezierEvaluator::BezierEvaluator( const BezierEvaluator & b )
: m_deg( b.m_deg )
, m_rate( b.m_rate )
, m_basesComputed( b.m_basesComputed )
{
//	mathdef::resize( m_combinations, m_deg + 1 );
	initializeCombinations();
}

void BezierEvaluator::initializeCombinations()
{
	mathdef::resize( m_combinations, m_deg + 1 );

	for (int i = 0; i <= m_deg; i++)
	{
		m_combinations[i] = computeNchooseR( m_deg, i );
	}
	
//	std::cout << "m_comb: " << m_combinations << std::endl;
}

VectorX1s BezierEvaluator::computeBernsteinBases( scalar t ) const
{
	VectorX1s bases;
	mathdef::resize(bases, m_deg + 1);
	
	for (int i = 0; i <= m_deg; i++)
	{
		bases[i] = m_combinations[i] * (std::pow(t, i)) * (std::pow(1 - t, m_deg - i));
//		std::cout << "m_bases[i] at t = " << t << ": " << m_bases[i] << std::endl;
	}
	
	return bases;
}

void BezierEvaluator::generateBernsteinMap( const int & rate )
{
	if (m_rate == rate) {
		return;
	}
	
	m_rate = rate;
	clearBernsteinMap();
	
	for (int i = 0; i <= m_rate; i++) {
		m_bernsteinMap[i] = computeBernsteinBases( computeParameterT( i, m_rate) );
		
		std::cout << "m_bernsteinMap[" << i << "]:\n" << m_bernsteinMap[i] << std::endl;
		
	}
	
	m_basesComputed = true;
	
}

// Tell us if the evaluator is ready to be used
bool BezierEvaluator::isBasesComputed() const
{
	return m_basesComputed;
}

void BezierEvaluator::clearBernsteinMap()
{
	m_bernsteinMap.clear();
	m_basesComputed = false;
}

Vector12s BezierEvaluator::eval( const VectorX2s & controlPoints, const scalar & t ) const
{
	assert( t >= 0.0 && t <= 1.0);
	assert( controlPoints.rows() == m_deg + 1);
	
	VectorX1s bases = computeBernsteinBases( t );
	
	return (bases.transpose() * controlPoints);
}

Vector12s BezierEvaluator::eval( const VectorX2s & controlPoints, const int & index ) const
{
	assert( index >= 0 && index <= m_rate);
	assert( controlPoints.rows() == m_deg + 1);
	assert( !m_bernsteinMap.empty() );		// Ensure that the bernstein bases map is already computed
	
	return ((m_bernsteinMap.find(index)->second).transpose() * controlPoints);
}