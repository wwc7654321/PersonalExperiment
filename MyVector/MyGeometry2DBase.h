#ifndef MYGEOMETRY2D_H
#define MYGEOMETRY2D_H
#include <math.h>
namespace MyGeo2D {
#define SMALLDD 0.0000000001
	template<typename VT>
	inline bool MyEqual(VT x, VT y) {
		return x - SMALLDD <= y && x + SMALLDD >= y;
	}
	inline bool MyEqual(int x, int y) {
		return x == y;
	}
	template<typename VT = int>
	class Point2D;
	template<typename VT = int>
	class Line2D;
	template<typename VT = int>
	class Vector2D;


#ifndef MYABS
#define MYABS(x) ((x)<(-x)?(-x):(x))
#endif
	template<typename VT>
	class Point2D
	{
	public:
		Point2D() :x(0), y(0), invalid(true) {}
		explicit Point2D(const Vector2D<VT>& a) :x(a.x), y(a.y) {}
		Point2D(VT xx, VT yy) :x(xx), y(yy), invalid(false) {}
		// 点到点距离
		VT Dist(const Point2D<VT> & point) const { return Line2D<VT>(*this, point).Length(); }
		// 点到线距离
		VT Dist(const Line2D<VT>& line) const {
			// 二维向量叉乘长度可看成以向量为相邻两边的平行四边形面积
			// 除以其底边长等于底边上的高，即点到直线距离
			if (!IsValid() || !line.IsValid()) return 0;
			const Vector2D<VT> v = line.ToVector(),
				u(line.P1(), *this);
			return v.CrossLength(u) / v.Length();
		}
		Point2D<VT>  operator+ (const Point2D<VT> &a)const { Point2D<VT> tmp(*this); tmp += a; return tmp; }
		Point2D<VT> & operator+=(const Point2D<VT> &a) { x += a.x; y += a.y; invalid = false; return *this; }
		Point2D<VT>  operator- (const Point2D<VT> &a)const { Point2D<VT> tmp(*this); tmp -= a; return tmp; }
		Point2D<VT> & operator-=(const Point2D<VT> &a) { x -= a.x; y -= a.y; invalid = false; return *this; }
		//		Point2D<VT> & operator* (const Point2D<VT> &a)const { Point2D<VT> tmp(this); tmp *= a; return tmp; }
		//		Point2D<VT> & operator*=(const Point2D<VT> &a) { x *= a.x; y *= a.y; invalid = false; return *this; }
		//		Point2D<VT> & operator/ (const Point2D<VT> &a)const { Point2D<VT> tmp(this); tmp /= a; return tmp; }
		//		Point2D<VT> & operator/=(const Point2D<VT> &a) { x /= a.x; y /= a.y; invalid = false; return *this; }

		Point2D<VT>  operator* (VT a)const { Point2D<VT> tmp(*this); tmp *= a; return tmp; }
		Point2D<VT> & operator*=(VT a) { x *= a; y *= a; invalid = false; return *this; }
		Point2D<VT>  operator/ (VT a)const { Point2D<VT> tmp(*this); tmp /= a; return tmp; }
		Point2D<VT> & operator/=(VT a) { x /= a; y /= a; invalid = false; return *this; }
		bool operator==(const Point2D<VT> &a)const { return MyEqual(x, a.x) && MyEqual(y, a.y); }
		bool operator!=(const Point2D<VT> &a)const { return !operator==(a); }

		Point2D<VT> & operator=(const Point2D<VT> &a) { Set(a); return *this; }
		void Set(const Point2D<VT> &a) { x = a.x; y = a.y; invalid = a.invalid; }
		void Set(VT xx, VT yy) { x = xx; y = yy; invalid = false; }
		VT X()const { return x; }
		VT Y()const { return y; }
		VT X(VT xx) { invalid = false; return x = xx; }
		VT Y(VT yy) { invalid = false; return y = yy; }
		bool IsValid() const { return !invalid; }
		void SetZero() { x = 0; y = 0; invalid = false; }
		bool IsZero() { return MyEqual(x, 0) && MyEqual(y, 0); }
	protected:
		Point2D<VT> & operator=(const Vector2D<VT> &a);	// 禁止Vector和Point之间隐式转换
		friend class Line2D<VT>;
		VT x;
		VT y;
		bool invalid;
	};

	template<typename VT>
	class Vector2D :public Point2D<VT> {
	public:
		Vector2D() :Point2D() {}
		explicit Vector2D(const Point2D<VT>& point) :Point2D(point) {} // 禁止Vector和Point之间隐式转换
		Vector2D(const Point2D<VT>& point1, const Point2D<VT>& point2) :Point2D(point2 - point1) {}
		Vector2D(VT xx, VT yy) :Point2D(xx, yy) {}
		// 向量乘
		VT operator* (const Vector2D<VT> &a)const { return a.x*x + a.y*y; }
		Vector2D<VT>  operator+ (const Vector2D<VT> &a)const { Vector2D<VT> tmp(*this); tmp += a; return tmp; }
		Vector2D<VT> & operator+=(const Vector2D<VT> &a) { Point2D<VT>::operator+=(a); return *this; }
		Vector2D<VT>  operator- (const Vector2D<VT> &a)const { Vector2D<VT> tmp(*this); tmp -= a; return tmp; }
		Vector2D<VT> & operator-=(const Vector2D<VT> &a) { Point2D<VT>::operator-=(a); return *this; }

		Vector2D<VT>  operator* (VT a)const { Vector2D<VT> tmp(*this); tmp *= a; return tmp; }
		Vector2D<VT> & operator*=(VT a) { Point2D<VT>::operator*=(a); return *this; }
		Vector2D<VT>  operator/ (VT a)const { Vector2D<VT> tmp(*this); tmp /= a; return tmp; }
		Vector2D<VT> & operator/=(VT a) { Point2D<VT>::operator/=(a); return *this; }
		// 叉乘
		Vector2D<VT> Cross(const Vector2D<VT>& a)const { return Vector2D<VT>(); }
		// 叉乘结果的长度
		VT CrossLength(const Vector2D<VT>& a)const { VT b = x*a.y - y*a.x; return MYABS(b); }
		// 向量长度
		VT Length()const { return (VT)sqrt(x*x + y*y); }
		// 向量长度平方
		VT SqLength()const { return x*x + y*y; }
		// 叉乘并设置
		Vector2D<VT> & CrossAndSet(const Vector2D<VT>& a) { return *this = Vector2D<VT>(); }
		// 平行   parallel 
		bool Parallel(const Vector2D<VT>&a)const { return MyEqual(x*a.y, y*a.x); }
		// 垂直   perpendicular
		bool Perp(const Vector2D<VT>&a)const { return MyEqual(0, *this * a); }
		// 规格化
		Vector2D<VT>& Normalize() { operator /=(Length()); return *this; }
		// 获得单位向量
		Vector2D<VT> ToNormalized() { return *this / Length(); }
		// 投影长度，可能小于0
		VT ProjectLength(const Vector2D<VT>& obj) { return *this * obj / Length(); }
		// 投影向量
		Vector2D<VT> ProjectVector(const Vector2D<VT>& obj)
		{
			VT i = (operator * (obj)) / (x*x + y*y);
			return Vector2D<VT>(x*i, y*i);
		}
	};

	template<typename VT>
	class Line2D
	{
	public:
		Line2D() :p1(), p2() {}
		explicit Line2D(const Point2D<VT>& point) :p1(0, 0), p2(point) {}
		Line2D(const Point2D<VT>& point1, const Point2D<VT>& point2) :p1(point1), p2(point2) {}
		Line2D(VT x1, VT y1, VT x2, VT y2) :p1(x1, y1), p2(x2, y2) {}

		Point2D<VT> MidPoint()const { return MidPoint(p1, p2); }
		static Point2D<VT> MidPoint(const Point2D<VT> & p1, const Point2D<VT>& p2) { Point2D<VT> tmp(p1); tmp += p2; tmp /= 2; return tmp; }
		VT Dist(const Point2D<VT> & p1) const { return p1.Dist(*this); }
		//平行   parallel 
		bool Parallel(const Line2D<VT>&a)const { a.ToVector().Parallel(ToVector()); }
		bool Parallel(const Vector2D<VT>&a)const { a.Parallel(ToVector()); }
		//垂直   perpendicular
		bool Perp(const Line2D<VT>&a)const { a.ToVector().Parallel(ToVector()); }
		bool Perp(const Vector2D<VT>&a)const { a.Parallel(ToVector()); }
		// 点在直线上
		bool PointOnLine(const Point2D<VT>&a)const {
			if (!IsValid())return false;
			return MyEqual(DX()* a.y - DY()* a.x, p2.x*p1.y - p1.x*p2.y);
		}
		// 点在线段上
		bool PointOnSegLine(const Point2D<VT>&a)const {
			return PointInRect(a) && PointOnLine(a);
		}
		bool PointInRect(const Point2D<VT>&a)const {
			if (!IsValid())return false;
			return (p1.x<p2.x ? p1.x : p2.x) <= a.x && (p1.x>p2.x ? p1.x : p2.x) >= a.x &&
				(p1.y<p2.y ? p1.y : p2.y) <= a.y && (p1.y>p2.y ? p1.y : p2.y) >= a.y;
		}

	protected:
		bool Gen(double a, double b, double c, double d, double e, double f, double &x, double &y) {

			if (a*e == b*d)return false;
			x = (b*f - e*c) / (a*e - b*d);
			y = (a*f - d*c) / (b*d - a*e);
			return true;
		}
	public:

		Point2D<VT> CrossAt(Line2D<VT> line,bool inRect=true) {
			if (!line.IsValid() || !IsValid())return Point2D<VT>();
			Vector2D<VT> u(p1, p2), v(line.P1(), line.P2()), t(line.P2(), p2);
			int i = 0;
			double x, y;
			if (Gen(u.X(), v.X(), t.X(), u.Y(), v.Y(), t.Y(), x, y))
			{
				Point2D<VT> p((VT)(u.X()*x + p2.X()), (VT)(u.Y()*x + p2.Y()));
				Point2D<VT> pp((VT)(line.P2().X() - v.X()*y), (VT)(line.P2().Y() - v.Y()*y));
				if ( p == pp && (!inRect || (PointInRect(p) && line.PointInRect(p))) )
				{
					return p;
				}
				else return Point2D<VT>();
			}
			return Point2D<VT>();
		}

		Line2D<VT> & operator=(const Line2D<VT> &line) { Set(line); return *this; }
		void Set(const Line2D<VT>&line) { p1 = line.p1; p2 = line.p2; }
		void Set(const Point2D<VT>&point1, const Point2D<VT>&point2) { p1 = point1; p2 = point2; }
		void Set(VT x1, VT y1, VT x2, VT y2) { p1.Set(x1, y1); p2.Set(x2, y2); }
		bool operator==(const Line2D<VT> &a)const { return (a.p1 == p1 && a.p2 == p2) || (a.p1 == p2 && a.p2 == p1); }
		bool operator!=(const Line2D<VT> &a)const { return (a.p1 != p1 || a.p2 != p2) && (a.p1 != p2 || a.p2 != p1); }
		Line2D<VT> operator+(const Vector2D<VT> &v) { Line2D<VT> l(*this); l.Move(v); return l; }
		Line2D<VT> & operator+=(const Vector2D<VT> &v) { Move(v); return *this; }

		VT Length() const { return sqrt(SqLength()); }
		VT SqLength() const { VT i = (p1.x - p2.x),j=(p1.y-p2.y);	return i*i + j*j;	}
		VT ManhattanLength() const { VT i = (p1.x - p2.x), j = (p1.y - p2.y); return MYABS(i) + MYABS(j);	}
		const Point2D<VT>& P1()const { return p1; }
		const Point2D<VT>& P2()const { return p2; }
		Point2D<VT> Mid()const { return (p1 + p2) / 2; }
		void P1(const Point2D<VT>& point) { p1 = point; }
		void P2(const Point2D<VT>& point) { p2 = point; }
		VT DX()const { return p2.x-p1.x; }
		VT DY()const { return p2.y -p1.y; }
		bool IsValid() const { return p1.IsValid()&& p2.IsValid()&& p1!=p2; }
		void SetZero() { p1.SetZero(); p2.SetZero(); }
		

		void Move(const Vector2D<VT>& a) { p1 += a; p2 += a; }

		Vector2D<VT> ToVector()const { return Vector2D<VT>(p1 , p2); }
	private: 
		Point2D<VT> p1, p2;
	};

	template<typename VT>
	class Geo2D {
	public:
		typedef VT Vt;
		typedef Point2D<VT> Point;
		typedef Vector2D<VT> Vector;
		typedef Line2D<VT> Line;
	};

};




#ifdef MYGEO_AUTO_DEFINE_2D_TYPE
typedef MYGEO_AUTO_DEFINE_2D_TYPE Vt2D;
typedef MyGeo2D::Geo2D<AUTO_DEFINE_2D_TYPE>::Point Point2D;
typedef MyGeo2D::Geo2D<AUTO_DEFINE_2D_TYPE>::Line Line2D;
typedef MyGeo2D::Geo2D<AUTO_DEFINE_2D_TYPE>::Vector Vector2D;
#endif

#endif // MYGEOMETRY2D