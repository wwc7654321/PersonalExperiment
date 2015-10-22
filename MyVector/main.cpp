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

#include <stdio.h>
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
	printf("%d,%d  %d\n",p1.X(),p1.Y(),p1.Dist(l1));
	printf("200,300 %d\n",l1.PointOnLine(Point(200,300)));
	printf("200,300 %d\n", l1.PointOnSegLine(Point(200, 300)));

	printf("300,300 %d\n", l1.PointOnLine(Point(300, 300)));
	printf("300,300 %d\n", l1.PointOnSegLine(Point(300, 300)));

	printf("600,700 %d\n", l1.PointOnLine(Point(600, 700)));
	printf("600,700 %d\n", l1.PointOnSegLine(Point(600, 700)));

	//p1 = vec;
	//vec = p1;
	getchar();
	return 0;
}