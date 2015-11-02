#include <stdio.h>

#include "MyGeometry2D.h"
#include "MyGeometry3D.h"
typedef MyGeo2D::Geo2D<int> Geo2D;
typedef Geo2D::Point Point;
typedef Geo2D::Line Line;
typedef Geo2D::Vector Vector;

typedef MyGeo3D::Geo3D<int> Geo3D;
typedef Geo3D::Point Point3D;
typedef Geo3D::Line Line3D;
typedef Geo3D::Vector Vector3D;
typedef Geo3D::Surface Surface3D;

typedef MyGeo3D::Geo3D<float> Geo3Df;
typedef Geo3Df::Point Point3Df;
typedef Geo3Df::Line Line3Df;
typedef Geo3Df::Vector Vector3Df;
typedef Geo3Df::Surface Surface3Df;
#include <stdio.h>
void Print(const Point3Df & p)
{
	printf("(%f,%f,%f)", p.X(), p.Y(), p.Z());
}
void Print(const Point3D & p)
{
	printf("(%d,%d,%d)", p.X(), p.Y(), p.Z());
}
void Print(const Line3D & p)
{
	Print(p.P1());
	putchar('-');
	Print(p.P2());
}


void gen1() {
	Point3Df p1(0, 2, 0), p2(2, 0, 0), p3(1, 0, 0), p4(3, 2, 0);
	Vector3Df v1(p1, p3), v2(p1, p4), v3(p2, p3), v4(p2, p4);
	float f1 = v1.Cross(v2).Z(), f2 = v3.Cross(v4).Z();
	printf("%f:%f =%f", f1, f2, f1 / f2); 
}

bool Gen(float a, float b, float c, float d, float e, float f,float &x,float &y) {
	
	if (a*e == b*d)return false;
	x = (b*f - e*c) / (a*e - b*d);
	y = (a*f - d*c) / (b*d - a*e);
	return true;
}
Point3Df gen(Point3Df p1, Point3Df p2, Point3Df p3, Point3Df p4) {
	Vector3Df u(p1, p2), v(p3, p4), t(p4,p2);
	int i = 0;
	float x, y;
	if (Gen(u.X(), v.X(), t.X(), u.Y(), v.Y(), t.Y(), x, y))
	{ 
		Point3Df p = u*x + p2;

		if ( p== p4-v*y )
		{
			return p;
		}
		else return Point3Df();
	}
	if (Gen(u.Y(), v.Y(), t.Y(), u.Z(), v.Z(), t.Z(), x, y))
	{
		Point3Df p = u*x + p2;

		if (p == p4 - v*y)
		{
			return p;
		}
		else return Point3Df();
	}
	return Point3Df();
}

int main() {
	Point3D pp;
	Surface3D sur;
	Vector3D vect(1,3,5),vecto(2,3,4);
	Point p1(200,200);
	Vector vec;
	Line l1(200, 300, 400, 500);
	vec.X(100);
	p1 += vec;
	vect *= 2; 
	vect += vecto;
	Print(vect);
	sur.PointOnSurface(pp);
	p1 == p1;
	printf("%d,%d  %d\n",p1.X(),p1.Y(),p1.Dist(l1));
	printf("200,300 %d\n",l1.PointOnLine(Point(200,300)));
	printf("200,300 %d\n", l1.PointOnSegLine(Point(200, 300)));

	printf("300,300 %d\n", l1.PointOnLine(Point(300, 300)));
	printf("300,300 %d\n", l1.PointOnSegLine(Point(300, 300)));

	printf("600,700 %d\n", l1.PointOnLine(Point(600, 700)));
	printf("600,700 %d\n", l1.PointOnSegLine(Point(600, 700)));

	//p1 = vec;
	//vec = p1;
	Point3Df po1(0, 2, 0), po2(2, 0, 0), po3(1, 0, 0), po4(3, 2, 0);
	Point3Df p = gen(po1, po3, po2, po4);
	if (p.IsValid()) Print(p);
	else printf("no point");
	putchar('\n');
	getchar();
	return 0;
}