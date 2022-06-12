#pragma once
#ifndef CLASS_DECLARE_H
#define CLASS_DECLARE_H

// Library call
#include <iostream>
#include <cmath>
#include <vector>
#include <z3++.h>
#include "framework.h"
#include <objidl.h>
#include <gdiplus.h>
#include <set>

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
using namespace std;
using namespace z3;

/*	Class declare	*/
// CPoint
class CPoint
{
private:
	int x, y;
public:
	CPoint(int a = 0, int b = 0);
	void SetCPoint(int a, int b);
	int getX() const;
	int getY() const;
};

float Distant(CPoint a, CPoint b);
bool isConvex(vector <CPoint> points);
int calc(int ax, int ay, int bx, int by, int cx, int cy);


class QuadraticEquationIn2Var;

// Equation
// Linear Equation Class in 2 variables
class LinearEquationIn2Var {
private:
	// Linear form : A*(x - a)  + B*(y - b) = 0
	int A, a, B, b;
	int xlim_l, xlim_r, ylim_d, ylim_u;
public:
	LinearEquationIn2Var(int A0 = 0, int a0 = 0, int B0 = 0, int b0 = 0);
	LinearEquationIn2Var(CPoint p1, CPoint p2);
	void setLinearEquationIn2Var(CPoint p1, CPoint p2);

	friend bool CheckSolution(const LinearEquationIn2Var Equa_1, const QuadraticEquationIn2Var Equa_2);
	friend bool CheckSolution(const LinearEquationIn2Var Equa_1, const LinearEquationIn2Var Equa_2);
};

// Quadratic Equation Class in 2 variables
class QuadraticEquationIn2Var {
private:
	// Quadratic form : A*(x - a)^2  + B*(y - b)^2 = R
	int Limit;
	int A, a, B, b, R;
public:
	QuadraticEquationIn2Var(int A0 = 0, int a0 = 0, int B0 = 0, int b0 = 0, int r = 0, int limit = 0);
	int getLimit() { return Limit; };
	friend class CElipse;
	friend class CCircle;

	friend bool CheckSolution(const LinearEquationIn2Var Equa_1, const QuadraticEquationIn2Var Equa_2);
	friend bool CheckSolution(const QuadraticEquationIn2Var Equa_1, const QuadraticEquationIn2Var Equa_2);
};



// Geometry
class Geometry
{
protected:
	vector<LinearEquationIn2Var> VLinearEqua;
	vector<QuadraticEquationIn2Var> VQuadraEqua;
public:
	virtual CPoint getCentroid() = 0;
	virtual float getArea() = 0;
	virtual bool IsInside(CPoint) = 0;
	virtual void draw(HDC hdc) = 0;
	virtual void fill(HDC hdc, int R, int G, int B) = 0;
	friend set<int> checkBelong();
};

// CElipse
class CElipse : public Geometry
{
protected:
	int Ra, Rb;
	CPoint Center;

public:
	CElipse(CPoint center = CPoint(0, 0), int a = 0, int b = 0);
	void setCElipse(CPoint center = CPoint(0, 0), int a = 0, int b = 0);
	virtual CPoint getCentroid();
	virtual float getArea();
	virtual bool IsInside(CPoint);
	virtual void draw(HDC hdc);
	virtual void fill(HDC hdc, int R, int G, int B);
};


// CCircle
class CCircle : public CElipse
{
protected:
	int limit;
public:
	int getLimit() { return limit; };
	CCircle(CPoint center = CPoint(0, 0), int r = 0, int lim = 0);
	void setCCircle(CPoint center = CPoint(0, 0), int r = 0, int lim = 0);
	virtual bool IsInside(CPoint);
};

// SemiCCircle
class CSemiCirlce : public CCircle
{
private:
	LinearEquationIn2Var HorizonLine;
public:
	CSemiCirlce(CPoint center = CPoint(0, 0), int r = 0, int slim = 1);
	virtual CPoint getCentroid();
	virtual float getArea();
	virtual void draw(HDC hdc);
	virtual void fill(HDC hdc, int R, int G, int B);
};


//CPolygon
class CPolygon : public Geometry
{
protected:
	vector<CPoint> VCPoint;
public:
	CPolygon(vector<CPoint> vCPoint = vector<CPoint>{ CPoint(0,0) });
	void setCPolygon(vector<CPoint> vCPoint);
	virtual void draw(HDC hdc);
	virtual void fill(HDC hdc, int R, int G, int B);
	virtual CPoint getCentroid();
	virtual float getArea();
	virtual bool IsInside(CPoint);
};



// CRectangle
class CRectangle : public CPolygon
{
protected:
	int width, height;
public:
	CRectangle(CPoint LUpper = CPoint(0, 0), CPoint RLower = CPoint(0, 0));
	void setCRectangle(CPoint LUpper, CPoint RLower);
	virtual void draw(HDC hdc);
	virtual void fill(HDC hdc, int R, int G, int B);
	virtual CPoint getCentroid();
	virtual float getArea();
	virtual bool IsInside(CPoint);
};

// CSquare
class CSquare : public CRectangle
{
public:
	CSquare(CPoint LUpper = CPoint(0, 0), CPoint RLower = CPoint(0, 0));
	void setCSquare(CPoint LUpper, CPoint RLower);
};


// CTriangle
class CTriangle : public CPolygon
{
public:
	CTriangle(vector<CPoint> vCPoint);
	void setCTriangle(vector<CPoint> vCPoint);
	virtual float getArea();
};


#endif // !CLASS_DECLARE_H
