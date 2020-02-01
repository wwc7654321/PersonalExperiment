#ifndef Ray3DTRACE_H
#define Ray3DTRACE_H

#include "MyGeometry3DBase.h"

template<typename VT = int>
class Ray3D;
template<typename VT = int>
class GeometryShape;
template<typename VT = int>
class Sphere;


 



template<typename VT>
class GeometryShape
{
	//typedef MyGeo3D::Geo3D<VT> Geo3D;
public:
	//typedef typename Geo3D::Point Point3;
	//typedef typename Geo3D::Line Line3;
	//typedef typename Geo3D::Vector Vector3;
	//typedef typename Geo3D::Surface Surface3;

	typedef MyGeo3D::Point3D<VT> Point3;
	typedef MyGeo3D::Line3D<VT> Line3;
	typedef MyGeo3D::Vector3D<VT> Vector3;
	typedef MyGeo3D::Surface3D<VT> Surface3;
	typedef struct IntersectPoint {
		GeometryShape<VT>* object;
		VT distance;
		Point3 point;
		Vector3 normal;
		IntersectPoint() :object(0), distance(0), point(), normal() {}
	}IntersectPoint;
	virtual bool intersect(const Ray3D<VT>&, IntersectPoint & ipoint) { return false; };
};

template<typename VT>
class Ray3D : public GeometryShape<VT>
{
public:
	Ray3D() :origin(), direction() {}
	Ray3D(const Point3& ori, const Vector3& dir) :origin(ori),direction(dir){}
	const Point3& Origin()const { return origin; }
	const Vector3& Direction()const { return origin; }
	Point3 GetAPoint(VT distance) { return origin + direction*distance; }
	bool IsValid()const { return origin.IsValid() && direction.IsValid(); }
private:
	Point3 origin;
	Vector3 direction;
};
/*
template<typename VT >
class Sphere : public GeometryShape<VT>
{
	typedef Ray3D<VT> Ray3;
	typedef MyGeo3D::Point3D<VT> Point3;
	typedef MyGeo3D::Line3D<VT> Line3;
	typedef MyGeo3D::Vector3D<VT> Vector3;
	typedef MyGeo3D::Surface3D<VT> Surface3;
public:
	Sphere() :center(), radius() {}
	Sphere(const Point3& cent, VT r) :center(cent), radius(r){}

	virtual bool intersect(const Ray3& ray, IntersectPoint & ipoint) {
		if (!IsValid() || !ray.IsValid()) return false;  
		Vector3 v(center,ray.Origin());
		VT a0 = v.SqLength() - radius*radius;
		VT DV = ray.Direction() * v;
		if (DV <= 0)
		{
			VT discr = DV*DV - a0;
			if (discr >= 0)
			{
				ipoint.object = this;
				ipoint.distance = -DV-(VT)sqrt(discur);
				ipoint.point = ray.GetAPoint(ipoint.distance);
				ipoint.normal = Vector3(center, ipoint.point).Normalize();
				return true;
			}
		}
		return false;
	}
	bool IsValid()const {
		return center.IsValid() && radius>0;
	}
private:
	Point3 center;
	VT radius;
};*/
#endif
