//
//  Point.h
//  VTS
//
//  Created by Papoj Thamjaroenporn on 10/11/12.
//  Copyright (c) 2012 Papoj Thamjaroenporn. All rights reserved.
//

#ifndef VTS_Point_h
#define VTS_Point_h

#include <iostream>
#include <sstream>
#include <cmath>

// A 2D point type.
class TwoDPoint {
	friend ostream &operator<<(ostream &os, const TwoDPoint &p) {
		return os<<"("<< p[0] <<","<< p[1] <<")";
	}
	
public:

	TwoDPoint () {xy[0] = 0;xy[1] = 0;}
	TwoDPoint (float x, float y]) {xy[0] = x;xy[1] = y;}
	TwoDPoint (const TwoDPoint& a) {xy[0] = a[0];xy[1] = a[1]}
	TwoDPoint (float * a) {xy[0] = a[0];xy[1] = a[1]}

	//Index operator for non-const objects returns modifiable lvalue.
	float &operator[] (const int &i) {return xy[i];}

	//Index operator for const objects returns rvalue.
	float operator[] (const int &i) const {return xy[i];}

	bool operator== (const TwoDPoint &p) {return (xy[0]==p.xy[0] && xy[1]==p.xy[1]); }
	bool operator!= (const TwoDPoint &p) {return !(*this == p);}

	void operator= (const TwoDPoint &p) {xy[0] = p.xy[0]; xy[1] = p.xy[1];}
	void operator+= (const TwoDPoint &p) {xy[0] += p.xy[0]; xy[1] += p.xy[1];}
	void operator-= (const TwoDPoint &p) {xy[0] -= p.xy[0]; xy[1] -= p.xy[1];}
	void operator*= (const float s) { xy[0] *= s; xy[1] *= s;}
	void operator/= (const float s) { xy[0] /= s; xy[1] /= s;}
	TwoDPoint operator+ (const TwoDPoint &p) const {
		TwoDPoint temp = *this;
		temp += p;
		return temp;
	}
	TwoDPoint operator- (const TwoDPoint &p) const {
		TwoDPoint temp = *this;
		temp -= p;
		return temp;
	}
	TwoDPoint operator- () const{
		TwoDPoint temp = *this;
		temp *= -1.0;
		return temp;
	}
	TwoDPoint operator* (const float s) const {
		TwoDPoint temp = *this;
		temp *= s;
		return temp;
	}
	TwoDPoint operator/ (const float s) const {
		TwoDPoint temp = *this;
		temp /= s;
		return temp;
	}
	
	// Distance and Norm function
	float distance(const TwoDPoint& b) const {
		TwoDPoint a = *this;
		a -= b;
		float sum_square = sqrt(a[0]*a[0] + a[1]*a[1]);
		return sum_square;
	}
	float norm() {
		return distance(TwoDPoint(0,0));
	}
	
protected:
    float xy[2];
};

// A 2D vector type.
class TwoDVector : public TwoDPoint {

public:
	TwoDVector () : TwoDPoint() {}
	TwoDVector (float x, float y]) : TwoDPoint (x, y]) {}
	TwoDVector (const TwoDVector& a) : TwoDPoint (a) {}
	TwoDVector (float * a) : TwoDPoint (a) {}
	
	float dot(const TwoDVector& b) {
		TwoDVector a = *this;
		return (a[0]*b[0] + a[1]*b[1]);
	}
	
	TwoDVector normalize() const {
		TwoDVector a = *this;
		float sum_square = sqrt(a[0]*a[0] + a[1]*a[1]);

		//Norm of a vector should be applied only if that vector is not (0,0,0).
		if (sum_square == 0.0) {
			std::cerr << "WARNING: ZERO NORMAL" << std::endl;
			return *this;
		}
		
		a[0] /= sum_square;
		a[1] /= sum_square;
		return a;
	}	

};

//
//class ThreeDPoint: public TwoDPoint {
//	friend ostream &operator<<(ostream &os, const ThreeDPoint &p) {
//		return os<<"("<< p[0] <<","<< p[1] <<","<< w <<")";
//	}
//	
//public:
//	
//
//private:
//	float w;
//}
//
//class FourDpoint : public TwoDPoint {
//	friend ostream &operator<<(ostream &os, const FourDpoint &p) {
//		return os<<"("<<p[0]<<","<<p[1]<<","<<p[2]<<","<<p[3]<<")";
//	}
//	
//public:
//	FourDpoint() {TwoDPoint(); w = 0;};
//	FourDpoint(float x0, float y0, float z0, float w0) : TwoDPoint(x0, y0, z0) { w = w0;};
//	FourDpoint(const FourDpoint& b) : TwoDPoint(b) { w = b[3];};
//	FourDpoint(const TwoDPoint& b, const float w0) : TwoDPoint(b) { w = w0;};
//	
//	//Index operator for non-const objects returns modifiable lvalue.
//    float &operator[] (const int &i) {return ((i != 3) ? xy[i] : w);};
//	
//	//Index operator for const objects returns rvalue.
//	float operator[] (const int &i) const {return ((i != 3) ? xy[i] : w);};
//	
//	bool operator== (const FourDpoint &p) {return (TwoDPoint::operator==(p) && w == p.w) ; };
//	bool operator!= (const FourDpoint &p) {return !(*this == p);};
//	
//	void operator= (const FourDpoint &p) { TwoDPoint::operator=(p); w = p.w; };
//	void operator+= (const FourDpoint &p) { TwoDPoint::operator+=(p); w += p.w; };
//	void operator-= (const FourDpoint &p) { TwoDPoint::operator-=(p); w -= p.w; };
//	void operator*= (const float s) { TwoDPoint::operator*=(s); w *= s; };
//	void operator/= (const float s) { TwoDPoint::operator/=(s); w /= s; };
//	FourDpoint operator+ (const FourDpoint &p) {		
//		FourDpoint temp = *this;
//		temp += p;
//		return temp;
//	}
//	FourDpoint operator- (const FourDpoint &p) {
//		FourDpoint temp = *this;
//		temp -= p;
//		return temp;
//	}
//	FourDpoint operator- () {
//		FourDpoint temp = *this;
//		temp *= -1.0;
//		return temp;
//	}
//	FourDpoint operator* (const float s) {
//		FourDpoint temp = *this;
//		temp *= s;
//		return temp;
//	}
//	FourDpoint operator/ (const float s) {
//		FourDpoint temp = *this;
//		temp /= s;
//		return temp;
//	}
//	
////	FourDpoint cross(const TwoDPoint& b) {
////		TwoDPoint temp;
////		TwoDPoint a = *this;
////		temp[0] = a[1]*b[2] - a[2]*b[1];
////		temp[1] = a[2]*b[0] - a[0]*b[2];
////		temp[2] = a[0]*b[1] - a[1]*b[0];
////		return temp;
////	}
//	
//	FourDpoint normalize() {
//		FourDpoint a = *this;
//		float sum_square = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2] + a[3]*a[3]);
//		
//		//Norm of a vector should be applied only if that vector is not (0,0,0).
//		assert(sum_square != 0.0);
//		// return TwoDPoint (0,0,0);
//		
//		a[0] /= sum_square;
//		a[1] /= sum_square;
//		a[2] /= sum_square;
//		a[3] /= sum_square;
//		return a;
//	}
//	
//	float distance(const FourDpoint& b) {
//		FourDpoint a = *this;
//		a -= b;
//		float sum_square = sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2] + a[3]*a[3]);
//		return sum_square;
//	}
//	
//	TwoDPoint projectBackTo3D() {
//		assert(w != 0);
//		
//		FourDpoint a = *this;
//		a /= w;
//		return TwoDPoint (a[0], a[1], a[2]);
//	}
//	
//protected:
//	float w;
//	
//};

#endif
