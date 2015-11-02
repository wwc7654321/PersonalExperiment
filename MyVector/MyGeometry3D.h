#ifndef MYGEOMETRY3D_H
#define MYGEOMETRY3D_H
#include <math.h>
namespace MyGeo3D {
#define SMALLDDD 0.0000000001
	template<typename VT>
	inline bool MyEqual(VT x, VT y) {
		return x-SMALLDDD <= y && x+SMALLDDD >= y;
	}
	inline bool MyEqual(int x, int y) {
		return x == y;
	}
	template<typename VT = int>
	class Point3D;
	template<typename VT = int>
	class Line3D;
	template<typename VT = int>
	class Vector3D;
	template<typename VT = int>
	class Surface3D;


#ifndef MYABS
#define MYABS(x) ((x)<(-x)?(-x):(x))
#endif
	template<typename VT>
	class Point3D
	{
		typedef Point3D<VT> Point3;
		typedef Vector3D<VT> Vector3;
		typedef Line3D<VT> Line3;
		typedef Surface3D<VT> Surface3;
	public:
		Point3D() :x(0), y(0) ,z(0), invalid(true) {}
		explicit Point3D(const Vector3& a) :x(a.x), y(a.y),z(a.z) {}
		Point3D(VT xx, VT yy,VT zz) :x(xx), y(yy) ,z(zz), invalid(false) {}
		// 点到点距离
		VT Dist(const Point3 & point) const { return Line3(*this, point).Length(); }
		// 点到线距离
		VT Dist(const Line3& line) const {
			// 向量叉乘长度可看成以向量为相邻两边的平行四边形面积
			// 除以其底边长等于底边上的高，即点到直线距离
			if (!IsValid() || !line.IsValid()) return 0;
			const Vector3 v = line.ToVector(),
				u(line.P1(), *this);
			return v.CrossLength(u) / v.Length();
		}
		Point3  operator+ (const Point3 &a)const { Point3 tmp(*this); tmp += a; return tmp; }
		Point3 & operator+=(const Point3 &a) { x += a.x; y += a.y; z+=a.z; invalid = false; return *this; }
		Point3  operator- (const Point3 &a)const { Point3 tmp(*this); tmp -= a; return tmp; }
		Point3 & operator-=(const Point3 &a) { x -= a.x; y -= a.y; z-=a.z; invalid = false; return *this; }

		Point3  operator* (VT a)const { Point3 tmp(*this); tmp *= a; return tmp; }
		Point3 & operator*=(VT a) { x *= a; y *= a; z *= a; invalid = false; return *this; }
		Point3  operator/ (VT a)const { Point3 tmp(*this); tmp /= a; return tmp; }
		Point3 & operator/=(VT a) { x /= a; y /= a; z /= a; invalid = false; return *this; }
		bool operator==(const Point3 &a)const { return MyEqual(x ,a.x) && MyEqual(y, a.y) && MyEqual(z, a.z); }
		bool operator!=(const Point3 &a)const { return !operator==(a); }

		Point3 & operator=(const Point3 &a) { Set(a); return *this; }
		void Set(const Point3 &a) { x = a.x; y = a.y; z = a.z; invalid = a.invalid; }
		void Set(VT xx, VT yy) { x = xx; y = yy; z = zz; invalid = false; }
		VT X()const { return x; }
		VT Y()const { return y; }
		VT Z()const { return z; }
		VT X(VT xx) { invalid = false; return x = xx; }
		VT Y(VT yy) { invalid = false; return y = yy; }
		VT Z(VT zz) { invalid = false; return z = zz; }
		bool IsValid() const { return !invalid; }
		void SetValid(bool b){ invalid = !b; }
		void SetZero() { x = 0; y = 0; z=0; invalid = false; }
		bool IsZero() { return MyEqual(x, 0) && MyEqual(y, 0) && MyEqual(x, 0); }
	protected:
		Point3 & operator=(const Vector3 &a);	// 禁止Vector和Point之间隐式转换
		friend class Line3;
		VT x;
		VT y;
		VT z;
		bool invalid;
	};

	template<typename VT>
	class Vector3D :public Point3D<VT> {
		typedef Point3D<VT> Point3;
		typedef Vector3D<VT> Vector3;
		typedef Line3D<VT> Line3;
		typedef Surface3D<VT> Surface3;
	public:
		Vector3D() :Point3D() {}
		explicit Vector3D(const Point3& point) :Point3D(point) {} // 禁止Vector和Point之间隐式转换
		Vector3D(const Point3& point1, const Point3& point2) :Point3D(point2 - point1) {}
		Vector3D(VT xx, VT yy,VT zz) :Point3D(xx, yy,zz) {}
		// 向量乘
		VT operator* (const Vector3 &a)const { return a.x*x + a.y*y + a.z*z; }
		Vector3 operator- ()const { Vector3 tmp(-x,-y); return tmp; }
		Vector3  operator+ (const Vector3 &a)const { Vector3 tmp(*this); tmp += a; return tmp; }
		Vector3 & operator+=(const Vector3 &a) { Point3::operator+=(a); return *this; }
		Vector3  operator- (const Vector3 &a)const { Vector3 tmp(*this); tmp -= a; return tmp; }
		Vector3 & operator-=(const Vector3 &a) { Point3::operator-=(a); return *this; }
		Point3  operator+ (const Point3 &a)const { return a.operator+(*this); }
		Point3  operator- (const Point3 &a)const { return a.operator-(*this); }

		Vector3  operator* (VT a)const { Vector3 tmp(*this); tmp *= a; return tmp; }
		Vector3 & operator*=(VT a) { Point3::operator*=(a); return *this; }
		Vector3  operator/ (VT a)const { Vector3 tmp(*this); tmp /= a; return tmp; }
		Vector3 & operator/=(VT a) { Point3::operator/=(a); return *this; }
		// 叉乘
		Vector3 Cross(const Vector3& a)const { return Vector3(y*a.z - a.y*z, z*a.x - a.z*x, x*a.y - a.x*y); }
		// 叉乘结果的长度
		VT CrossLength(const Vector3& a)const { return Cross(a).Length(); }
		// 向量长度
		VT Length()const { return (VT)sqrt(x*x + y*y + z*z); }
		// 叉乘并设置
		Vector3 & CrossAndSet(const Vector3& a) { return *this = Cross(a); }
		//平行   parallel 
		bool Parallel(const Vector3&a)const { return MyEqual(x*a.y , y*a.x) && MyEqual(y*a.z,z*a.y); }
		//垂直   perpendicular
		bool Perp(const Vector3&a)const { return MyEqual(*this * a,0) ; }
		// 规格化
		void Normalize() { operator /=(Length()); }
		// 获得单位向量
		Vector3 ToNormalized() { return *this / Length(); }
		// 投影长度，可能小于0
		VT ProjectLength(const Vector3& obj) { return *this * obj / Length(); }
		// 投影向量
		Vector3 ProjectVector(const Vector3& obj) 
		{ 
			VT i=(operator * (obj))/(x*x+y*y+z*z);
			return Vector3(	x*i,y*i,z*i	);
		}
	};

	template<typename VT>
	class Line3D
	{
		typedef Point3D<VT> Point3;
		typedef Vector3D<VT> Vector3;
		typedef Line3D<VT> Line3;
		typedef Surface3D<VT> Surface3;
	public:
		Line3D() :p1(), p2() {}
		explicit Line3D(const Point3& point) :p1(0, 0，0), p2(point) {}
		Line3D(const Point3& point1, const Point3& point2) :p1(point1), p2(point2) {}
		Line3D(VT x1, VT y1,VT z1, VT x2, VT y2,VT z2) :p1(x1, y1,z1), p2(x2, y2,z2) {}

		Point3 MidPoint()const { return MidPoint(p1, p2); }
		static Point3 MidPoint(const Point3 & p1, const Point3& p2) { Point3 tmp(p1); tmp += p2; tmp /= 2; return tmp; }
		VT Dist(const Point3 & p1) const { return p1.Dist(*this); }
		//平行   parallel 
		bool Parallel(const Line3&a)const { a.ToVector().Parallel(ToVector()); }
		bool Parallel(const Vector3&a)const { a.Parallel(ToVector()); }
		//垂直   perpendicular
		bool Perp(const Line3&a)const { a.ToVector().Parallel(ToVector()); }
		bool Perp(const Vector3&a)const { a.Parallel(ToVector()); }
		// 点在直线上
		bool PointOnLine(const Point3&a)const {
			if (!IsValid())return false;
			return MyEqual(DX()* a.y - DY()* a.x , p2.x*p1.y - p1.x*p2.y) &&
				MyEqual(DY()*a.z - DZ()*a.y, p2.y * p1.z - p1.y*p2.z);
		}
		// 点在线段上
		bool PointOnSegLine(const Point3&a)const {
			return PointInRect(a) && PointOnLine(a);
		}
		// 点在以两点为对角线，六边平行坐标轴的长方体范围内
		bool PointInRect(const Point3&a)const {
			if (!IsValid())return false;
			return (p1.x<p2.x ? p1.x : p2.x) <= a.x && (p1.x>p2.x ? p1.x : p2.x) >= a.x &&
				(p1.y<p2.y ? p1.y : p2.y) <= a.y && (p1.y>p2.y ? p1.y : p2.y) >= a.y &&
				(p1.z<p2.z ? p1.z : p2.z) <= a.z && (p1.z>p2.z ? p1.z : p2.z) >= a.z;
		}
		Point3 CrossAt(const Line3& line) {
			if (!line.IsValid()||!IsValid())return Point3();
			//bool b1 = PointOnLine(line.p1), b2 = PointOnLine(line.p2);
			if (Parallel(line)) { return Point3(); }
			/*if (b1 || b2)
			{
				if (!b1)
				{
					if (PointInRect(line.p2))return Point3(line.p2);
					else return Point3();
				}
				else if (!b2) 
				{
					if (PointInRect(line.p1))return Point3(line.p1);
					else return Point3();
				}
				else
				{
					return Point3();
				}
			}*/
			if (!Surface3(p1, p2, line.p1).PointOnSurface(line.p2))return Point3();
			// 四点共面，两线不重合

		}
		Line3 & operator=(const Line3 &line) { Set(line); return *this; }
		void Set(const Line3&line) { p1 = line.p1; p2 = line.p2; }
		void Set(const Point3&point1, const Point3&point2) { p1 = point1; p2 = point2; }
		void Set(VT x1, VT y1,VT z1, VT x2, VT y2,VT z2) { p1.Set(x1, y1,z1); p2.Set(x2, y2,z2); }
		bool operator==(const Line3 &a)const { return (a.p1 == p1 && a.p2 == p2) || (a.p1 == p2 && a.p2 == p1); }
		bool operator!=(const Line3 &a)const { return (a.p1 != p1 || a.p2 != p2) && (a.p1 != p2 || a.p2 != p1); }
		Line3 operator+(const Vector3 &v) { Line3 l(*this); l.Move(v); return l; }
		Line3 & operator+=(const Vector3 &v) {Move(v); return *this; }

		VT Length() const { return sqrt(SqLength()); }
		VT SqLength() const { VT i = (p1.x - p2.x), j = (p1.y - p2.y),k=(p1.z - p2.z);	return i*i + j*j + k*k; }
		VT ManhattanLength() const { VT i = (p1.x - p2.x), j = (p1.y - p2.y), k = (p1.z - p2.z); return MYABS(i) + MYABS(j)+ MYABS(k); }
		Point3 P1()const { return p1; }
		Point3 P2()const { return p2; }
		void P1(const Point3& point) { p1 = point; }
		void P2(const Point3& point) { p2 = point; }
		VT DX()const { return p2.x - p1.x; }
		VT DY()const { return p2.y - p1.y; }
		VT DZ()const { return p2.z - p1.z; }
		bool IsValid() const { return p1.IsValid() && p2.IsValid() && p1 != p2; }
		void SetZero() { p1.SetZero(); p2.SetZero(); }


		void Move(const Vector3& a) { p1 += a; p2 += a; }

		Vector3 ToVector()const { return Vector3(p1, p2); }
	private:
		Point3 p1, p2;
	};


	template<typename VT>
	class Surface3D
	{
		typedef Point3D<VT> Point3;
		typedef Vector3D<VT> Vector3;
		typedef Line3D<VT> Line3;
		typedef Surface3D<VT> Surface3;
	public:
		Surface3D():p1(),p2(),p3(), iIsValid(0) {}
		Surface3D(const Line3 &a, const Line3 &b):p1(a.P1()),p2(a.P2()),p3(a.P2()+b.ToVector()), iIsValid(0) {	};
		Surface3D(const Vector3 &a, const Vector3 &b, const Point3 &c=Point3(0,0,0)) :p1(c), p2(c+a), p3(c+b), iIsValid(0) {	};
		Surface3D(const Point3 &a, const Point3 &b, const Point3 &c) :p1(a),p2(b),p3(c), iIsValid(0) {};

		// 法向量，a b c 逆时针为正面 
		Vector3 LawVector() { Vector3 v1(p1, p2), v2(p1, p3); return v1.Cross(v2); }
		Vector3 LawVectorNormal() { Vector3 v1(p1, p2), v2(p1, p3); return v1.Cross(v2).ToNormalized(); }

		bool PointOnSurface(const Point3 & p) {
			if (!IsValid()) return false;
			if (p == p1 || p == p2 || p == p3)return true;
			Vector3 v1(p, p1), v2(p, p2),v3(p,p3);
			return v1.Cross(v2).Perp(v3);
		}
		bool LineOnSurface(const Line3 &l) {
			if (!IsValid() || !l.IsValid()) return false;
			if (l.p1 == p1 || l.p1 == p2 || l.p1 == p3) return PointOnSurface(l.p2);
			if (l.p2 == p1 || l.p2 == p2 || l.p2 == p3) return PointOnSurface(l.p1);
			Vector3 v1(p1, p2), v2(p1, p3), v3(l.p1, p3)
				,v4= v1.Cross(v2);
			return v4.Perp(v3) && v4.Perp(l.ToVector());
		}
		bool SurfaceOnSurface(const Surface3 &l) {
			if (!IsValid() || !l.IsValid()) return false;
			if (l.p1 == p1 || l.p1 == p2 || l.p1 == p3) return LineOnSurface(l.ToLine2());
			if (l.p2 == p1 || l.p2 == p2 || l.p2 == p3) return LineOnSurface(l.ToLine3());
			if (l.p3 == p1 || l.p3 == p2 || l.p3 == p3) return LineOnSurface(l.ToLine1());
			Vector3 v1(p1, p2), v2(p1, p3), v3(l.p1, p3)
				, v4 = v1.Cross(v2);
			return v4.Perp(v3) && v4.Perp(l.ToLine1().ToVector()) && v4.Perp(l.ToLine2().ToVector());
		}

		void Set(const Line3 &a, const Line3 &b) { p1 = a.P1(); p2 = a.P2(); p3 = a.P2() + b.ToVector(); iIsValid = 0;}
		void Set(const Vector3 &a, const Vector3 &b, const Point3 &c = Point3(0, 0, 0)) { p1 = c; p2 = c + a; p3 = c + b; iIsValid = 0;	}
		void Set(const Point3 &a, const Point3 &b, const Point3 &c) { p1 = a; p2 = b; p3 = c; iIsValid = 0;	}

		Line3 ToLine1()const { return Line3(p1,p2); }
		Line3 ToLine2()const { return Line3(p2, p3); }
		Line3 ToLine3()const { return Line3(p3, p1); }
		Point3 P1()const { return p1; }
		Point3 P2()const { return p2; }
		Point3 P3()const { return p3; }
		void P1(const Point3& point) { p1 = point; iIsValid = 0; }
		void P2(const Point3& point) { p2 = point; iIsValid = 0;	}
		void P3(const Point3& point) { p3 = point; iIsValid = 0;	}
		bool IsValid() {
			if (iIsValid == 0) {
				iIsValid=( p1.IsValid() && p2.IsValid() && p3.IsValid() &&
					p1 != p2 && p2 != p3 && !Line3(p1, p2).PointOnLine(p3)) ? 1 : 2;
			}
			return iIsValid == 1;
		}
	private:
		int iIsValid; // 0 not checked  1 valid 2 invalid
		Point3 p1, p2, p3;
	};

	template<typename VT=int>
	class Geo3D {
	public:
		typedef VT Vt;
		typedef Point3D<VT> Point;
		typedef Vector3D<VT> Vector;
		typedef Line3D<VT> Line;
		typedef Surface3D<VT> Surface;
	};

};


#ifdef MYGEO_AUTO_DEFINE_3D_TYPE
typedef MYGEO_AUTO_DEFINE_3D_TYPE Vt3D;
typedef MyGeo3D::Geo3D<AUTO_DEFINE_3D_TYPE>::Point Point3D;
typedef MyGeo3D::Geo3D<AUTO_DEFINE_3D_TYPE>::Line Line3D;
typedef MyGeo3D::Geo3D<AUTO_DEFINE_3D_TYPE>::Vector Vector3D;
typedef MyGeo3D::Geo3D<AUTO_DEFINE_3D_TYPE>::Surface Surface3D;
#endif

#endif // MYGEOMETRY3D