//
//  BezierCurve.h
//  VTS
//
//  Created by Papoj Thamjaroenporn on 10/11/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef __VTS__BezierCurve__
#define __VTS__BezierCurve__

#include <iostream>
#include <vector>

vector<int> myVector;

#include "MathDef.h"
#include "splineParser.h"

class BezierCurve {
public:
	
	BezierCurve( const VectorXs &controlPoints, const int &deg)
	: m_c( controlPoints )
	, m_deg( deg )
	{}
	
	// constructs from a parsed definition stored in a curveOrSurface definition
    BezierCurve (const curveOrSurface &cOS)
	: knots_(cOS.knotsU)
	, degree_(cOS.degreeU)
	, points_(cOS.controlPoints)
	, weights_(cOS.weights)
	{
		//cout << "weight and point size: " << weights_.size() << " " << points_.size() << endl;
		assert (weights_.size() == points_.size());
		setup4DControlPoints();
	};
	
	// constructs a curve from a list of controlpoints in 4D.
	BezierCurve (const vector< FourDpoint >& ctrlPts4D, const vector< float >& knots,
				  const int deg)
	: degree_(deg)
	, points4D_(ctrlPts4D)
	, knots_(knots)
	{
		points_.resize(points4D_.size());
		weights_.resize(points4D_.size());
        //		cout << "point4D size: " << points4D_.size() << endl;
        
		for (int i = 0; i < points4D_.size(); i++) {
            //			cout << "curve 4Dpoint: " << i << "\t" << points4D_[i] << endl;
			points_[i] = points4D_[i].projectBackTo3D();
			weights_[i] = points4D_[i][3];
		}
	};
    
	// Transform given 3D control points (x, y, z) to (wx, wy, wz, w).
	void setup4DControlPoints() {
		points4D_.resize(points_.size());
		for (int i = 0; i < points_.size(); i++) {
			points4D_[i] = FourDpoint ((points_[i] * weights_[i]), weights_[i]);
            //			cout << "(in curve) point, weight: " << points_[i] << " " << weights_[i] << endl;
            //			cout << "(in curve) rationalPoint: " << points4D_[i] << endl;
		}
	};
    
	// project samplePoints4D_ and sampleDervVectors4D_ to points_ and sampleDervVectors_
	// For the samplePoints4D_ we simply divide all components [x,y,z,w] the homogeneous part (w).
	// For the sampleDervVectors4D_, we find [(P + U)' - P'] where the ' operation means projecting
	// 4D back to 3D, P is a sampled 4D point on the curve and U is a derivative 4D vector at P.
	void project4DPointsAndVectorsTo3D(const int i, bool computeDerv);
	
    // sample this curve and store the points so they can be used for rendering later:
    void sampleCurveWithBasisSpline (const int rate, bool computeDerv, bool projectBack);
	
    // general evaluation: expects u to be on the correct domain from the knot vector:
    void eval (const float u, FourDpoint &p);
    void evalWithDerv (const float u, FourDpoint &p, FourDpoint &derv);
    
    // sometimes we will just want to evaluate on [0 1], without knowing
    // what the real parameter range is. this is ok for simple situations,
    // like drawing:
    void evalNormalized (const float u, FourDpoint &p);
	void evalNormalizedWithDerv (const float u, FourDpoint &p, FourDpoint &derv);
    
    // compute basis functions:
    int computeAllBasis (const float u, rangeVector< float > &bases);
	int computeAllBasisAndDerivatives (const float u, rangeVector< float > &bases, rangeVector< float > &basesDerv);
    
	// get the start and end of the curve's parameter domain:
    void getDomain (float &strt, float &end);
	void getDomainIndices (int &startindex, int &endindex);
	
    // simple support functions:
    int numVerts (void) { return points_.size(); };
	int numKnots (void) { return knots_.size(); };
	int getDegree (void) { return degree_; };
    apoint vert (const int i) { return points_[i]; };
	FourDpoint vert4D (const int i) { return points4D_[i]; };
	vector< float >& getKnots () { return knots_; };
	float getWeight (const int i) {return weights_[i]; };
	
	// print out an error:
	static void splineError (const char *s, bool exitProgram) {
		cout << s << "\n";
		if(exitProgram) exit(0);
	};
	
	
	// a place to store sampels for rendering, which will then be used by OpenGL:
//    vector< apoint > samplePoints_;
//	vector< apoint > sampleDervVectors_;
//	vector< FourDpoint> samplePoints4D_;
//	vector< FourDpoint> sampleDervVectors4D_;
	
protected:
    
	// Class member: four control points contained
	VectorXs m_c;
	
	int m_deg;
	
//    // returns the span that u is in, with special handling when u is the
//    // last point in the domain.
//    int findInterval (float u);
//	
//	//Check if 0 <= i < k
//	bool outRange(int i, int k);
//    
//    int degree_;
//    vector< float > knots_;
//    vector< apoint > points_;
//	vector< FourDpoint > points4D_;
//	
//	vector< float > weights_;
};

typedef std::vector< BezierCurve > PolyBezierCurve;



#endif /* defined(__VTS__BezierCurve__) */