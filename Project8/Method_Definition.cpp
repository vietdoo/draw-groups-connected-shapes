#include "Class_Declare.h"
#include "framework.h"
#include "Project8.h"
#define PI 3.14159265



/* CPoint method definition */


CPoint::CPoint(int a, int b) {
	x = a;	y = b;
}

void CPoint::SetCPoint(int a, int b) {
	x = a;	y = b;
}


int CPoint::getX() const {
	return x;
}

int CPoint::getY() const {
	return y;
}

//
/* Equation part definition */
//

/* LinearEquationIn2Var method definition */

LinearEquationIn2Var::LinearEquationIn2Var(int A0, int a0, int B0, int b0) {
	// Linear form: A*(x - a) + B*(y - b) = 0
	A = A0;		B = B0;
	a = a0;		b = b0;
	xlim_l = 0;		xlim_r = 0;
	ylim_d = 0;		ylim_u = 0;
};

LinearEquationIn2Var::LinearEquationIn2Var(CPoint p1, CPoint p2) {
	setLinearEquationIn2Var(p1, p2);
}

void LinearEquationIn2Var::setLinearEquationIn2Var(CPoint p1, CPoint p2)
{
	A = p1.getY() - p2.getY();		a = p1.getX();
	B = p2.getX() - p1.getX();		b = p1.getY();
	xlim_l = p1.getX() <= p2.getX() ? p1.getX() : p2.getX();
	xlim_r = p1.getX() >= p2.getX() ? p1.getX() : p2.getX();
	ylim_d = p1.getY() <= p2.getY() ? p1.getY() : p2.getY();
	ylim_u = p1.getY() >= p2.getY() ? p1.getY() : p2.getY();
}

bool CheckSolution(const LinearEquationIn2Var Equa_1, const LinearEquationIn2Var Equa_2)
{
	//	
	config cfg;
	cfg.set("auto_config", true);
	context c(cfg);
	solver s(c);

	// Linear form: A*(x - a) + B*(y - b) = 0

	expr x = c.real_const("x");
	expr y = c.real_const("y");
	s.add(Equa_1.A * (x - Equa_1.a) + Equa_1.B * (y - Equa_1.b) == 0);
	s.add(Equa_2.A * (x - Equa_2.a) + Equa_2.B * (y - Equa_2.b) == 0);

	s.add(x >= Equa_1.xlim_l && x <= Equa_1.xlim_r);
	s.add(y >= Equa_1.ylim_d && y <= Equa_1.ylim_u);

	s.add(x >= Equa_2.xlim_l && x <= Equa_2.xlim_r);
	s.add(y >= Equa_2.ylim_d && y <= Equa_2.ylim_u);
	if (!s.check())
		return false;
	else
		return true;
}

/* QuadraticEquationIn2Var method definition */

QuadraticEquationIn2Var::QuadraticEquationIn2Var(int A0, int a0, int B0, int b0, int r, int limit) {
	A = A0;		B = B0;		Limit = limit;
	a = a0;		b = b0;		R = r;
};

bool CheckSolution(const QuadraticEquationIn2Var Equa_1, const QuadraticEquationIn2Var Equa_2)
{
	config cfg;
	cfg.set("auto_config", true);
	context c(cfg);

	expr x = c.real_const("x");
	expr y = c.real_const("y");

	solver s(c);

	// Quadratic form: A*(x - a) + B*(y - b) = R 
	// Circle form: (x - a)^2 + (y - b)^2 = R^2		<limit = 0>
	// CElipse form: (x - a)*(x - a)/Ra*Ra + (y - b)*(y - b)/Rb*Rb = 1		<limit = 3>

	if (Equa_1.Limit == 3 and Equa_2.Limit == 3) {
		s.add((x - Equa_1.a) * (x - Equa_1.a) / (Equa_1.A * Equa_1.A) + (y - Equa_1.b) * (y - Equa_1.b) / (Equa_1.B * Equa_1.B) == 1);
		s.add((x - Equa_2.a) * (x - Equa_2.a) / (Equa_2.A * Equa_2.A) + (y - Equa_2.b) * (y - Equa_2.b) / (Equa_2.B * Equa_2.B) == 1);
	}
	else if (Equa_1.Limit == 3) {
		s.add((x - Equa_1.a) * (x - Equa_1.a) / (Equa_1.A * Equa_1.A) + (y - Equa_1.b) * (y - Equa_1.b) / (Equa_1.B * Equa_1.B) == 1);
		s.add(Equa_2.A * (x - Equa_2.a) * (x - Equa_2.a) + Equa_2.B * (y - Equa_2.b) * (y - Equa_2.b) == Equa_2.R);
	}
	else if (Equa_2.Limit == 3) {
		s.add((x - Equa_2.a) * (x - Equa_2.a) / (Equa_2.A * Equa_2.A) + (y - Equa_2.b) * (y - Equa_2.b) / (Equa_2.B * Equa_2.B) == 1);
		s.add(Equa_1.A * (x - Equa_1.a) * (x - Equa_1.a) + Equa_1.B * (y - Equa_1.b) * (y - Equa_1.b) == Equa_1.R);
	}
	else {
		s.add(Equa_1.A * (x - Equa_1.a) * (x - Equa_1.a) + Equa_1.B * (y - Equa_1.b) * (y - Equa_1.b) == Equa_1.R);
		s.add(Equa_2.A * (x - Equa_2.a) * (x - Equa_2.a) + Equa_2.B * (y - Equa_2.b) * (y - Equa_2.b) == Equa_2.R);
	}

	// case semiCCircle in Equa_1	 <limit = 1 or -1>
	if (Equa_1.Limit != 0)
		if (Equa_1.Limit == 1)
			s.add(y <= Equa_1.b);
		else if (Equa_1.Limit == -1)
			s.add(y >= Equa_1.b);

	// case semiCCircle in Equa_2	 <limit = 1 or -1>
	if (Equa_2.Limit != 0)
		if (Equa_2.Limit == 1)
			s.add(y <= Equa_2.b);
		else if (Equa_2.Limit == -1)
			s.add(y >= Equa_2.b);

	if (!s.check())
		return false;
	else
		return true;
}


// Friend in both class Linear andd Quadra
bool CheckSolution(const LinearEquationIn2Var Equa_1, const QuadraticEquationIn2Var Equa_2)
{
	config cfg;
	cfg.set("auto_config", true);
	context c(cfg);

	expr x = c.real_const("x");
	expr y = c.real_const("y");
	solver s(c);

	// Linear form: A*(x - a) + B*(y - b) = 0
	// Quadratic form: A*(x - a) + B*(y - b) = R 
	// Circle form: (x - a)^2 + (y - b)^2 = R^2		<limit = 0>
	// CElipse form: (x - a)*(x - a)/Ra*Ra + (y - b)*(y - b)/Rb*Rb = 1		<limit = 3>

	s.add(Equa_1.A * (x - Equa_1.a) + Equa_1.B * (y - Equa_1.b) == 0);

	if (Equa_2.Limit == 3)
		s.add((x - Equa_2.a) * (x - Equa_2.a) / (Equa_2.A * Equa_2.A) + (y - Equa_2.b) * (y - Equa_2.b) / (Equa_2.B * Equa_2.B) == 1);
	else
		s.add(Equa_2.A * (x - Equa_2.a) * (x - Equa_2.a) + Equa_2.B * (y - Equa_2.b) * (y - Equa_2.b) == Equa_2.R);

	// add Linear intersect limit
	s.add(x >= Equa_1.xlim_l && x <= Equa_1.xlim_r);
	s.add(y >= Equa_1.ylim_d && y <= Equa_1.ylim_u);

	// case semiCCircle in Equa_2		<limit = 1 or -1>
	if (Equa_2.Limit != 0)
		if (Equa_2.Limit == 1) {
			s.add(y <= Equa_2.b);
		}
		else if (Equa_2.Limit == -1) {
			s.add(y >= Equa_2.b);
		}

	if (!s.check())
		return false;
	else
		return true;
}


//
/* Shape part definition */
//


/* CElipse method definition */

CElipse::CElipse(CPoint center, int a, int b)
{
	setCElipse(center, a, b);
}

void CElipse::setCElipse(CPoint center, int a, int b)
{
	// Quadratic form : A*(x - a)^2  + B*(y - b)^2 = R
	// CElipse form: (x - a)*(x - a)/Ra*Ra + (y - b)*(y - b)/Rb*Rb = 1  <case limit = 3>

	VQuadraEqua.resize(0);
	QuadraticEquationIn2Var Equation;
	Center = center;
	Ra = a;		Rb = b;
	Equation.A = Ra;		Equation.a = Center.getX();		Equation.Limit = 3;
	Equation.B = Rb;		Equation.b = Center.getY();		Equation.R = 1;
	VQuadraEqua.push_back(Equation);
}



void CElipse::draw(HDC hdc)
{
	Ellipse(hdc, Center.getX() - Ra, Center.getY() - Rb, Center.getX() + Ra, Center.getY() + Rb);
}

void CElipse::fill(HDC hdc, int R, int G, int B)
{
	Graphics g(hdc);
	SolidBrush brush(Color(20 /*A*/, R /*R*/, G /*G*/, B /*B*/));
	g.FillEllipse(&brush, Center.getX() - Ra, Center.getY() - Rb, Ra * 2, Rb * 2);

}

CPoint CElipse::getCentroid()
{
	return Center;
}

float CElipse::getArea()
{
	return PI * Ra * Rb;
}

bool CElipse::IsInside(CPoint pcheck)
{
	// check: (x - a)^2/Ra^2 + (y - b)^2/Rb^2 <= 1  <case limit = 0>
	int ra2 = VQuadraEqua[0].A * VQuadraEqua[0].A;
	int x = (pcheck.getX() - VQuadraEqua[0].a) * (pcheck.getX() - VQuadraEqua[0].a);
	int rb2 = VQuadraEqua[0].B * VQuadraEqua[0].B;
	int y = (pcheck.getY() - VQuadraEqua[0].b) * (pcheck.getY() - VQuadraEqua[0].b);
	return x / ra2 + y / rb2 <= 1;
}



/* CCircle method definition */

CCircle::CCircle(CPoint center, int r, int lim) : CElipse(center, r, r)
{
	setCCircle(center, r, lim);
}

void CCircle::setCCircle(CPoint center, int r, int lim)
{
	// Quadratic form : A*(x - a)^2  + B*(y - b)^2 = R
	// Circle form: (x - a)^2 + (y - b)^2 = R^2

	VQuadraEqua.clear();
	QuadraticEquationIn2Var Equation;
	Equation.A = 1;		Equation.a = Center.getX();		limit = lim;	// <Circle limit = 0> / <semiCCircle limit = 1 or -1>
	Equation.B = 1;		Equation.b = Center.getY();		Equation.R = Ra * Ra;
	Equation.Limit = lim;
	VQuadraEqua.push_back(Equation);
}

bool CCircle::IsInside(CPoint pcheck)
{
	// Check A*(x - a) + B*(y - b) <= R^2 with pcheck(x, y)
	bool check = false;

	check = (VQuadraEqua[0].A * pow(pcheck.getX() - VQuadraEqua[0].a, 2) + VQuadraEqua[0].B * pow(pcheck.getY() - VQuadraEqua[0].b, 2)) <= VQuadraEqua[0].R;
	string res = to_string(limit);
	TCHAR* tmp = new TCHAR[res.length()];
	tmp[res.size()] = 0;
	copy(res.begin(), res.end(), tmp);

	if (check && limit != 0)
		if (limit == 1)
			return pcheck.getY() <= Center.getY();
		else
			return pcheck.getY() >= Center.getY();
	else if (check)
		return true;
	return false;
}


/* SemiCCircle */

CSemiCirlce::CSemiCirlce(CPoint center, int r, int slim) :CCircle(center, r, slim) {

	HorizonLine.setLinearEquationIn2Var(CPoint(center.getX() - r, center.getY()), CPoint(center.getX() + r, center.getY()));
	VLinearEqua.push_back(HorizonLine);
}

void CSemiCirlce::draw(HDC hdc)
{
	int x = Center.getX(), y = Center.getY();
	if (limit == 1) {
		Pie(hdc, x - Ra, y - Rb, x + Ra, y + Rb, x + Ra, y, x - Ra, y);
	}
	else {
		Pie(hdc, x - Ra, y - Ra, x + Ra, y + Ra, x - Ra, y, x + Ra, y);
	}
}
void CSemiCirlce::fill(HDC hdc, int R, int G, int B)
{
	int x = Center.getX(), y = Center.getY();
	Graphics g(hdc);
	SolidBrush brush(Color(20 /*A*/, R/*R*/, G /*G*/, B /*B*/));

	if (limit == 1) {
		Rect r(x - Ra, y - Ra, Ra * 2, Ra * 2);
		g.FillPie(&brush, r, 180, 180);
	}
	else {
		Rect r(x - Ra, y - Ra, Ra * 2, Ra * 2);
		g.FillPie(&brush, r, 0, 180);
	}
}
CPoint CSemiCirlce::getCentroid()
{
	if (limit == 1) {
		return CPoint(Center.getX(), Center.getY() - Rb / 2);
	}
	else {
		return CPoint(Center.getX(), Center.getY() + Rb / 2);
	}
}

float CSemiCirlce::getArea()
{
	return PI * Ra * Rb / 2;
}


/* CRectangle method definition */

void CRectangle::setCRectangle(CPoint LUpper, CPoint RLower) {

	// Calc 4 CPoint coordianate
	VCPoint.resize(0);
	width = abs(RLower.getX() - LUpper.getX());
	height = abs(LUpper.getY() - RLower.getY());

	VCPoint.push_back(LUpper);
	VCPoint.push_back(CPoint({ LUpper.getX() + width, LUpper.getY() }));
	VCPoint.push_back(RLower);
	VCPoint.push_back(CPoint({ LUpper.getX(), RLower.getY() }));

	// Calc 4 line equation 
	VLinearEqua.clear();
	for (int i = 0; i < 4; i++) {
		if (i == 3)
			VLinearEqua.push_back(LinearEquationIn2Var(VCPoint[3], (VCPoint[0])));
		else
			VLinearEqua.push_back(LinearEquationIn2Var(VCPoint[i], (VCPoint[i + 1])));
	}
}

void CRectangle::draw(HDC hdc)
{
	Rectangle(hdc, VCPoint[0].getX(), VCPoint[0].getY(), VCPoint[2].getX(), VCPoint[2].getY());
}

void CRectangle::fill(HDC hdc, int R, int G, int B)
{
	Graphics g(hdc);
	SolidBrush brush(Color(20 /*A*/, R/*R*/, G /*G*/, B /*B*/));
	g.FillRectangle(&brush, VCPoint[0].getX(), VCPoint[0].getY(), width, height);
}

CPoint CRectangle::getCentroid()
{
	return CPoint((VCPoint[0].getX() + VCPoint[2].getX()) / 2, (VCPoint[0].getY() + VCPoint[2].getY()) / 2);
}

float CRectangle::getArea()
{
	return width * height;
}

bool CRectangle::IsInside(CPoint pcheck)
{
	// check: point inside CRectangle with a point in the infinite right side create a LinearEquationIn2Var just intersect with only 1 LinearEquationIn2Var in CRectangle
	LinearEquationIn2Var lcheck(pcheck, CPoint(pcheck.getX() + 1000000, pcheck.getY()));
	int through = 0;
	for (auto i : VLinearEqua) {
		through += CheckSolution(lcheck, i);
	}
	return through == 1;
}

CRectangle::CRectangle(CPoint LUpper, CPoint RLower)
{
	setCRectangle(LUpper, RLower);
}


/* CSquare */

CSquare::CSquare(CPoint LUpper, CPoint RLower) : CRectangle(LUpper, RLower) {}

void CSquare::setCSquare(CPoint LUpper, CPoint RLower) {
	CRectangle::setCRectangle(LUpper, RLower);
}





/* CPolygon */

CPolygon::CPolygon(vector<CPoint> vCPoint) {
	setCPolygon(vCPoint);
}

void CPolygon::setCPolygon(vector<CPoint> vCPoint)
{
	// Create n LinearEquationIn2Var from n CPoint by user input
	if (vCPoint.empty() == true) {
		VCPoint.resize(0);
	}
	VLinearEqua.resize(0);
	VCPoint = vCPoint;
	for (int i = 0; i < VCPoint.size() - 1; i++) {
		for (int j = i + 1; j < VCPoint.size(); j++) {
			VLinearEqua.push_back(LinearEquationIn2Var(VCPoint.at(i), VCPoint.at(j)));
		}
	}
}

void CPolygon::draw(HDC hdc)
{
	POINT* P = new POINT[VCPoint.size()];
	for (int i = 0; i < VCPoint.size(); i++) {
		P[i] = { VCPoint[i].getX(), VCPoint[i].getY() };
	}

	Polygon(hdc, P, VCPoint.size());
	delete[]P;
}

void CPolygon::fill(HDC hdc, int R, int G, int B)
{
	Graphics g(hdc);
	SolidBrush brush(Color(20 /*A*/, R/*R*/, G /*G*/, B /*B*/));

	Point* P = new Point[VCPoint.size()];
	for (int i = 0; i < VCPoint.size(); i++) {
		P[i] = { VCPoint[i].getX(), VCPoint[i].getY() };
	}
	g.FillPolygon(&brush, P, VCPoint.size());
}


CPoint CPolygon::getCentroid()
{
	int x = 0, y = 0;
	for (int i = 0; i < VCPoint.size(); i++) {
		x += VCPoint[i].getX();
		y += VCPoint[i].getY();
	}
	return CPoint(x / VCPoint.size(), y / VCPoint.size());
}

float CPolygon::getArea()
{
	// Using Shoelace formula
	float area = 0.0;
	int n = VCPoint.size();
	int j = n - 1;
	for (int i = 0; i < n; i++)
	{
		area += (VCPoint[j].getX() + VCPoint[i].getX()) * (VCPoint[j].getY() - VCPoint[i].getY());
		j = i;
	}

	return abs(area / 2.0);
}

bool CPolygon::IsInside(CPoint pcheck)
{
	// check: point inside CPolygon with a point in the infinite right side create a LinearEquationIn2Var just intersect with only 1 LinearEquationIn2Var in CPolygon
	LinearEquationIn2Var lcheck(pcheck, CPoint(pcheck.getX() + 1000000, pcheck.getY()));
	int through = 0;
	for (auto i : VLinearEqua) {
		through += CheckSolution(lcheck, i);
	}
	return through == 1;
}

/* CTriangle */

CTriangle::CTriangle(vector<CPoint> vCPoint) : CPolygon(vCPoint) {}
void CTriangle::setCTriangle(vector<CPoint> vCPoint)
{
	CPolygon::setCPolygon(vCPoint);
}

float CTriangle::getArea()
{
	float da, db, dc;
	da = Distant(VCPoint[0], VCPoint[1]);
	db = Distant(VCPoint[0], VCPoint[2]);
	dc = Distant(VCPoint[1], VCPoint[2]);
	float p = 0.5 * (da + db + dc);
	return sqrt(p * (p - da) * (p - db) * (p - dc));
}

// Distant from 2 CPoint
float Distant(CPoint a, CPoint b) {
	return sqrt(pow(a.getX() - b.getX(), 2) + pow(a.getY() - b.getY(), 2));
}

// Check convex from n CPoint
bool isConvex(vector <CPoint> points) {
	bool neg = false;
	bool pos = false;
	int n = points.size();
	for (int i = 0; i < n; i++) {
		int a = i;
		int b = (i + 1) % n;
		int c = (i + 2) % n;
		int crossProduct = calc(points[a].getX(), points[a].getY(), points[b].getX(), points[b].getY(), points[c].getX(), points[c].getY());
		if (crossProduct < 0) neg = true;
		else if (crossProduct > 0) pos = true;
		if (neg && pos) return false;
	}
	float sum = 0;
	for (int i = 0; i < points.size() - 1; i++) {
		sum += Distant(points[i], points[i + 1]);
	}
	if (sum == Distant(points[0], points[points.size() - 1]))
		return 0;
	return true;
}

int calc(int ax, int ay, int bx, int by, int cx, int cy) {
	int BAx = ax - bx;
	int BAy = ay - by;
	int BCx = cx - bx;
	int BCy = cy - by;
	return (BAx * BCy - BAy * BCx);
}


/*
	std::string str = to_string(10);
	TCHAR* param = new TCHAR[str.size() + 1];
	param[str.size()] = 0;
	std::copy(str.begin(), str.end(), param);
	MessageBox(NULL, param, L"Lỗi khởi tạo", MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
*/