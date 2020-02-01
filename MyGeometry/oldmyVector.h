


#ifndef _MYVECTOR_H
#define _MYVECTOR_H
#include<math.h>

#define t_double int//float
/*
点  P  x,y,z

向量V  x,y,z

线  L  p1 p2

面  S  p1 p2 p3


  平移		OK

  旋转

  反转
*/




class Vector3;
class Line3;
class Surface3;
class Point3;

typedef Point3 Point;
typedef Line3 Line;
typedef	Surface3 Surface;
typedef Vector3 Vector;

class Point3
{
	public:
		Point3();
		Point3(const Point3 &);
		Point3(t_double,t_double,t_double);
	
	//t_double operator-(const Point3 & a);
		t_double awayfrom(const Point3& a)const;
		t_double awayfrom(const Line3& a)const;
		t_double awayfrom(const Surface3& a)const;
		Point3 & operator+ (const Vector3 &a)const;
		Point3 & operator+=(const Vector3 &a);
		Point3 & operator- (const Vector3 &a)const;
		Point3 & operator-=(const Vector3 &a);
		bool     operator==(const Point3 &a)const;
		Point3 & operator=(const Point3 &a);
		Point3 & operator=(const Vector3 &a);

		Line3 & lineto (const Point3 &a)const;

		Vector3& tovector()const;
		Point3 & midpointto(const Point3& p2)const;
		t_double x,y,z;
};





class Line3
{
public:
		Line3();
		Line3(t_double x,t_double y);
		Line3(t_double x,t_double y,const Vector3& v);
		Line3(const Point3& p,const Vector3& v);
		Line3(const Point3& p1,const Point3& p2);
		Line3(t_double x,t_double y,t_double x2,t_double y2);

		bool par(const Line3&a)const ;//平行 
		bool ape(const Line3&a)const ;//垂直 
		bool par(const Surface3&a)const ;//平行 
		bool ape(const Surface3&a)const ;//垂直 
		bool par(const Vector3&a)const ;//平行 
		bool ape(const Vector3&a)const ;//垂直 
		t_double awayfrom(const Point3& a)const;
		t_double awayfrom(const Line3& a)const;
		t_double awayfrom(const Surface3& a)const;
		Line3 & operator+ (const Vector3 &a)const;
		Line3 & operator+=(const Vector3 &a);
		Line3 & operator- (const Vector3 &a)const;
		Line3 & operator-=(const Vector3 &a);
		Line3 & operator=(const Line3 &a);
		bool     operator==(const Line3 &a)const;
		Vector3 & getvector()const ;
		void set(t_double,t_double,const Vector3&);
		void set(const Point3&,const Point3&);
		void set(const Point3&,const Vector3&);
		void set(t_double,t_double,t_double,t_double);
		Point3  getp1()const;
		Point3  getp2()const;
		Point3  getminpoint()const;
private:
	Point3 p1,p2;
};

class Surface3
{
	public :
		Surface3();
		Surface3(const Line3 &a,const Line3 &b);
		Surface3(const Point3 &a,const Vector3 &b);
		Surface3(const Vector3 &a,const Vector3 &b);
		Surface3(const Vector3 &a,const Vector3 &b,const Point3 &c);
		Surface3(const Point3 &a,const Point3 &b,const Point3 &c);
		t_double awayfrom(const Point3& a)const;
		t_double awayfrom(const Line3& a)const;
		t_double awayfrom(const Surface3& a)const;
		//Surface3();
		Point3   getapoint()const;
		Vector3 & getlawvector()const;
		Surface3 & operator+ (const Vector3 &a)const;
		Surface3 & operator+=(const Vector3 &a);
		Surface3 & operator- (const Vector3 &a)const;
		Surface3 & operator-=(const Vector3 &a);
		Surface3 & operator=(const Surface3 &a);
		bool     operator==(const Surface3 &a)const;

		bool par(const Line3&a)const ;//平行 
		bool ape(const Line3&a)const ;//垂直 
		bool par(const Surface3&a)const ;//平行 
		bool ape(const Surface3&a)const ;//垂直 
		bool par(const Vector3&a)const ;//平行 
		bool ape(const Vector3&a)const ;//垂直 

		 
		void set(const Line3 &a,const Line3 &b);	//两线
		void set(const Point3 &a,const Vector3 &b); //点法
		void set(const Vector3 &a,const Vector3 &b);//＝双线
		void set(const Vector3 &a,const Vector3 &b,const Point3 &c);  
		void set(const Point3 &a,const Point3 &b,const Point3 &c);
 

	private:
		Point3 p1,p2,p3;
		
};




class Vector3 
{
	public:
		Vector3();
		Vector3(const Vector3& b); 
		Vector3(const Point3& a,const Point3& b); 
		Vector3(t_double ,t_double ,t_double  );
		//~Vector3();
		bool par(const Line3&a)const ;//平行 
		bool ape(const Line3&a)const ;//垂直 
		bool par(const Surface3&a)const ;//平行 
		bool ape(const Surface3&a)const ;//垂直 
		bool par(const Vector3&a)const ;//平行 
		bool ape(const Vector3&a)const ;//垂直 
		//t_double value();
		t_double length(void)const;
		Vector3 & operator=(const Vector3&  a); 
		Vector3 & operator=(const Point3&  a); 
		bool  operator==(const Vector3&  a)const; 
		Vector3 & operator+(const Vector3& a)const;
		Vector3 & operator+=(const Vector3& a);
		Vector3 & operator-(const Vector3& a)const;
		Vector3 & operator-=(const Vector3& a);
		Vector3 & operator-()const;
		t_double operator*(const Vector3& a)const;
		Vector3 & operator*(t_double yy)const;
		Vector3 & operator*=(t_double yy);
		Vector3 & operator^(const Vector3& a)const;		
		Vector3 & operator^=(const Vector3& a);	

		void settounit(void);
		Vector3 & getunit(void)const;
		void set(t_double=0,t_double=0,t_double=0);
		void set(const Point3& a,const Point3& b);
		void set(const Vector3& b);
		Point3& topoint()const;
 		t_double * arrv();
		t_double x,y,z;
		 
};
























/*Vector3 getapevector(Vector3 &a,Vector3 &b)
{
  
	Vector3 c(1,1,1);
	
	if(a.par(b)) throw "par vectors";
	if(!(a.x||a.y||a.z)||!(b.x||b.y||b.z)) throw "0 vector!";
	if (a.x * b.y != a.y*b.x)
	{
		c.x=(b.z * a.y-a.z *b.y) / (a.x *b.y -b.x *a.y);
		c.y=(b.z * a.x-a.z *b.x) / (a.y *b.x -b.y *a.x);
	}else if(a.x * b.z != a.z *b.x)
	{
		c.x=(b.y * a.z-a.y *b.z) / (a.x *b.z -b.x *a.z);
		c.z=(b.y * a.x-a.y *b.x) / (a.z *b.x -b.z *a.x);
	}else if(a.y *b.z != a.z *b.y)
	{
		c.y=(b.x * a.z-a.x *b.z) / (a.y *b.z -b.y *a.z);
		c.z=(b.x * a.y-a.x *b.y) / (a.z *b.y -b.z *a.y);
	}else throw "err?";
	return c;
}*/

#endif
