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
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
using namespace std;
using namespace z3;

/*	Class declare	*/

// CPoint
class CPoint
{
private:
	
public:
	int x, y;
	CPoint(int a = 0, int b = 0);
	void SetCPoint(int a, int b);
	void Input(istream& inDevice);
	int getX() const;
	int getY() const;

	friend ostream& operator << (ostream& outDevice, const CPoint& p);
	friend istream& operator >> (istream& inDevice, CPoint& p);
};
float Distant(CPoint a, CPoint b);
bool isConvex(vector <CPoint> points);
int calc(int ax, int ay, int bx, int by, int cx, int cy);
// Equation
class Equation
{
public:
	Equation() {};
	virtual void Input(istream&) = 0;
	virtual CPoint getCentroid() = 0;
	virtual float getArea() = 0;
	virtual bool IsInside(CPoint) = 0;
};

class QuadraticEquationIn2Var;
class LinearEquationIn2Var {
private:
	int A, a, B, b;
	int xlim_l, xlim_r, ylim_d, ylim_u;
	// General form : A*(x - a)  + B*(y - b) = 0
public:
	LinearEquationIn2Var(int A0 = 0, int a0 = 0, int B0 = 0, int b0 = 0);
	LinearEquationIn2Var(CPoint p1, CPoint p2);
	void setLinearEquationIn2Var(CPoint p1, CPoint p2);
	friend bool CheckSolution(const LinearEquationIn2Var Equa_1, const QuadraticEquationIn2Var Equa_2);
	friend bool CheckSolution(const LinearEquationIn2Var Equa_1, const LinearEquationIn2Var Equa_2);

};

// Quadratic function in 2 variables
	// General form: A*(x - a)^2 + B*(y - b)^2 = R
class QuadraticEquationIn2Var {
private:
public:
	int A, a, B, b, R;
	int Limit;


	QuadraticEquationIn2Var(int A0 = 0, int a0 = 0, int B0 = 0, int b0 = 0, int r = 0, int limit = 0);
	int getLimit() { return Limit; };
	friend class CElipse;
	friend class CCircle;
	friend bool CheckSolution(const LinearEquationIn2Var Equa_1, const QuadraticEquationIn2Var Equa_2);
	friend bool CheckSolution(const QuadraticEquationIn2Var Equa_1, const QuadraticEquationIn2Var Equa_2);
};



// Figure
class Figure
{
public:
	//Figure() {};
	vector<LinearEquationIn2Var> VLinearEqua;
	vector<QuadraticEquationIn2Var> VQuadraEqua;
	virtual void draw(HDC hdc) = 0;
	virtual void fill(HDC hdc, int R, int G, int B) = 0;
	virtual CPoint getCentroid() = 0;
	virtual float getArea() = 0;
	virtual bool IsInside(CPoint) = 0;
};

// CElipse
class CElipse : public Figure
{
protected:
	int Ra, Rb;
	CPoint Center;

public:
	CElipse(CPoint center = CPoint(0, 0), int a = 0, int b = 0);
	void setCElipse(CPoint center = CPoint(0, 0), int a = 0, int b = 0);
	operator QuadraticEquationIn2Var();
	virtual void draw(HDC hdc);
	virtual void fill(HDC hdc, int R, int G, int B);
	virtual CPoint getCentroid();
	virtual float getArea();
	virtual bool IsInside(CPoint);
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
	operator QuadraticEquationIn2Var();
	virtual bool IsInside(CPoint);
};

// CRectangle

class CRectangle : public Figure
{
protected:
	int width, height;
public:
	vector<CPoint> VCPoint;
	CRectangle(CPoint LUpper = CPoint(0, 0), CPoint RLower = CPoint(0, 0));
	void setCRectangle(CPoint LUpper, CPoint RLower);
	virtual void draw(HDC hdc);
	virtual void fill(HDC hdc, int R, int G, int B);
	virtual CPoint getCentroid();
	virtual float getArea();
	virtual bool IsInside(CPoint);
};


// SemiCCircle

class CSemiCirlce : public CCircle
{

public:
	LinearEquationIn2Var HorizonLine;
	CSemiCirlce(CPoint center = CPoint(0, 0), int r = 0, int slim = 1);
	virtual void draw(HDC hdc);
	virtual void fill(HDC hdc, int R, int G, int B);
	virtual CPoint getCentroid();
	virtual float getArea();
	//virtual bool IsInside(CPoint);
};


// CSquare
class CSquare : public CRectangle
{

public:
	CSquare(CPoint LUpper = CPoint(0, 0), CPoint RLower = CPoint(0, 0));
	void setCSquare(CPoint LUpper, CPoint RLower);
};

//CPolygon

class CPolygon : public Figure
{
protected:
	vector<CPoint> VCPoint;
	vector<LinearEquationIn2Var> VEqua;
	
public:
	//CPolygon() {};
	CPolygon(vector<CPoint> vCPoint);
	void setCPolygon(vector<CPoint> vCPoint);
	virtual void draw(HDC hdc);
	virtual void fill(HDC hdc, int R, int G, int B);
	virtual CPoint getCentroid();
	virtual float getArea();
	virtual bool IsInside(CPoint);
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
